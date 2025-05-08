# 内存的分配

整体参考下这里
https://mp.weixin.qq.com/s?__biz=Mzg2MzU3Mjc3Ng==&mid=2247487111&idx=1&sn=e57371f9c3e6910f4f4721aa0787e537&chksm=ce77c8c0f90041d67b2d344d413a2573f3662a1a64a802b41d4618982fcbff1617d9a5da9f7b&scene=178&cur_album_id=2559805446807928833#rd



每个zone的内存水位


low, high, min 3个内存水位

```
3.10内核
/*
 * This is the 'heart' of the zoned buddy allocator.
 */
struct page *
__alloc_pages_nodemask(gfp_t gfp_mask, unsigned int order,
            struct zonelist *zonelist, nodemask_t *nodemask)


最新master分支
/*
 * This is the 'heart' of the zoned buddy allocator.
 */
struct page *__alloc_pages(gfp_t gfp, unsigned int order, int preferred_nid,
							nodemask_t *nodemask)

```

# 具体流程

```

/*
 * This is the 'heart' of the zoned buddy allocator.
 */
struct page *
__alloc_pages_nodemask(gfp_t gfp_mask, unsigned int order, int preferred_nid,
							nodemask_t *nodemask)
{
	struct page *page;
	unsigned int alloc_flags = ALLOC_WMARK_LOW;
	gfp_t alloc_mask; /* The gfp_t that was actually used for allocation */
	struct alloc_context ac = { };

	/*
	 * There are several places where we assume that the order value is sane
	 * so bail out early if the request is out of bound.
	 */
	if (unlikely(order >= MAX_ORDER)) {
		WARN_ON_ONCE(!(gfp_mask & __GFP_NOWARN));
		return NULL;
	}

	gfp_mask &= gfp_allowed_mask;
	alloc_mask = gfp_mask;

	page = kfence_alloc_page(order, preferred_nid, gfp_mask);
	if (unlikely(page)) {
		set_page_private(page, 0);
		prep_new_page(page, 0, gfp_mask, alloc_mask);
		goto out;
	}

	if (!prepare_alloc_pages(gfp_mask, order, preferred_nid, nodemask, &ac, &alloc_mask, &alloc_flags))
		return NULL;

	/*
	 * Forbid the first pass from falling back to types that fragment
	 * memory until all local zones are considered.
	 */
	alloc_flags |= alloc_flags_nofragment(ac.preferred_zoneref->zone, gfp_mask);

	/* First allocation attempt */
	page = get_page_from_freelist(alloc_mask, order, alloc_flags, &ac);
	if (likely(page))
		goto out;

	/*
	 * Apply scoped allocation constraints. This is mainly about GFP_NOFS
	 * resp. GFP_NOIO which has to be inherited for all allocation requests
	 * from a particular context which has been marked by
	 * memalloc_no{fs,io}_{save,restore}.
	 */
	alloc_mask = current_gfp_context(gfp_mask);
	ac.spread_dirty_pages = false;

	/*
	 * Restore the original nodemask if it was potentially replaced with
	 * &cpuset_current_mems_allowed to optimize the fast-path attempt.
	 */
	ac.nodemask = nodemask;

	page = __alloc_pages_slowpath(alloc_mask, order, &ac);

out:
	if (memcg_kmem_enabled() && (gfp_mask & __GFP_ACCOUNT) && page &&
	    unlikely(__memcg_kmem_charge_page(page, gfp_mask, order) != 0)) {
		__free_pages(page, order);
		page = NULL;
	}

	trace_mm_page_alloc(page, order, alloc_mask, ac.migratetype);

	return page;
}
```

__alloc_pages_nodemask会先尝试调用get_page_from_freelist从伙伴系统的freelist里拿空闲页，如果能拿到就直接返回：

如果拿不到, 则进入慢速路径. __alloc_pages_slowpath,
会唤醒 kswap 内核线程  wake_all_kswapds(order, gfp_mask, ac); 再次尝试从freelist获取页面。如果再次失败，

 如果还是失败，则进入direct_reclaim阶段,  还失败则进入 __alloc_pages_direct_compact 内存规整，

```
/* Try direct reclaim and then allocating */
page = __alloc_pages_direct_reclaim(gfp_mask, order, alloc_flags, ac,
                        &did_some_progress);
if (page)
    goto got_pg;

/* Try direct compaction and then allocating */
page = __alloc_pages_direct_compact(gfp_mask, order, alloc_flags, ac,
                compact_priority, &compact_result);
if (page)
    goto got_pg;
```

如果还失败，则进入oom kill
```
/* Reclaim has failed us, start killing things */
page = __alloc_pages_may_oom(gfp_mask, order, ac, &did_some_progress);
if (page)
    goto got_pg;
```


# __alloc_pages_slowpath 

```
static inline struct page *
__alloc_pages_slowpath(gfp_t gfp_mask, unsigned int order,
						struct alloc_context *ac)
{
	// 在慢速内存分配路径中可能会导致内核进行直接内存回收
    // 这里设置 __GFP_DIRECT_RECLAIM 表示允许内核进行直接内存回收
	bool can_direct_reclaim = gfp_mask & __GFP_DIRECT_RECLAIM;
	// 如果 > 3 order 就是比较大的内存分配行为了
	// 当请求内存页的数量大于 2 ^ 3 = 8 个内存页时，costly_order = true，
	// 内核就认为本次内存分配是一次成本比较大的行为。
	// 后续会根据这个参数 costly_order 来决定是否触发 OOM 。
	const bool costly_order = order > PAGE_ALLOC_COSTLY_ORDER;
	struct page *page = NULL;
	unsigned int alloc_flags;
	// 后续用于记录直接内存回收了多少内存页
	unsigned long did_some_progress;
	enum compact_priority compact_priority;
	enum compact_result compact_result;
	int compaction_retries;
	// 记录重试的次数，超过一定的次数（16次）则内存分配失败
	//no_progress_loops 用于记录内存分配重试的次数，如果内存分配重试的次数超过最大限制 
	// MAX_RECLAIM_RETRIES，则停止重试，开启 OOM。
	int no_progress_loops;
	unsigned int cpuset_mems_cookie;
	// 临时保存调整后的内存分配策略
	int reserve_flags;

	/*
	 * We also sanity check to catch abuse of atomic reserves being used by
	 * callers that are not in atomic context.
	 * 我们还进行了理智检查，以发现滥用原子能储备的行为
	 *  不在原子上下文中的调用者。
	 *   // 流程现在来到了慢速内存分配这里，说明快速分配路径已经失败了
    // 内核需要对 gfp_mask 分配行为掩码做一些修改，修改为一些更可能导致内存分配成功的标识
    // 因为接下来的直接内存回收非常耗时可能会导致进程阻塞睡眠，不适用原子 __GFP_ATOMIC 内存分配的上下文。
	 */
	if (WARN_ON_ONCE((gfp_mask & (__GFP_ATOMIC|__GFP_DIRECT_RECLAIM)) ==
				(__GFP_ATOMIC|__GFP_DIRECT_RECLAIM)))
		gfp_mask &= ~__GFP_ATOMIC;

retry_cpuset:
	compaction_retries = 0;
	no_progress_loops = 0;
	compact_priority = DEF_COMPACT_PRIORITY;
	cpuset_mems_cookie = read_mems_allowed_begin();

	/*
	 * The fast path uses conservative alloc_flags to succeed only until
	 * kswapd needs to be woken up, and to avoid the cost of setting up
	 * alloc_flags precisely. So we do that now.
	 // 在之前的快速内存分配路径下设置的相关分配策略比较保守，不是很激进，
	 用于在 WMARK_LOW 水位线之上进行快速内存分配
    // 走到这里表示快速内存分配失败，此时空闲内存严重不足了
    // 所以在慢速内存分配路径下需要重新设置更加激进的内存分配策略，采用更大的代价来分配内存
	 */
	alloc_flags = gfp_to_alloc_flags(gfp_mask);

	/*
	 * We need to recalculate the starting point for the zonelist iterator
	 * because we might have used different nodemask in the fast path, or
	 * there was a cpuset modification and we are retrying - otherwise we
	 * could end up iterating over non-eligible zones endlessly.
	 */
	// 重新按照新的设置按照内存区域优先级计算 zonelist 的迭代起点（最高优先级的 zone）
    // fast path 和 slow path 的设置不同所以这里需要重新计算
	ac->preferred_zoneref = first_zones_zonelist(ac->zonelist,
					ac->highest_zoneidx, ac->nodemask);
	// 如果没有合适的内存分配区域，则跳转到 nopage , 内存分配失败
	if (!ac->preferred_zoneref->zone)
		goto nopage;
	// 唤醒所有的 kswapd 进程异步回收内存
	if (alloc_flags & ALLOC_KSWAPD)
		wake_all_kswapds(order, gfp_mask, ac);

	/*
	 * The adjusted alloc_flags might result in immediate success, so try
	 * that first
	 */
	 // 此时所有的 kswapd 进程已经被唤醒，正在异步进行内存回收
    // 之前我们已经在 gfp_to_alloc_flags 方法中重新调整了 alloc_flags
    // 换成了一套更加激进的内存分配策略，注意此时是在 WMARK_MIN 水位线之上进行内存分配
    // 调整后的 alloc_flags 很可能会立即成功，因此这里先尝试一下
	page = get_page_from_freelist(gfp_mask, order, alloc_flags, ac);
	if (page)
		goto got_pg;

	/*
	 * For costly allocations, try direct compaction first, as it's likely
	 * that we have enough base pages and don't need to reclaim. For non-
	 * movable high-order allocations, do that as well, as compaction will
	 * try prevent permanent fragmentation by migrating from blocks of the
	 * same migratetype.
	 * Don't try this for allocations that are allowed to ignore
	 * watermarks, as the ALLOC_NO_WATERMARKS attempt didn't yet happen.
	 * // 对于分配大内存来说 costly_order = true (超过 8 个内存页)，
	 * 需要首先进行内存整理，这样内核可以避免直接内存回收从而获取更多的连续空闲内存页
    // 对于需要分配不可移动的高阶内存的情况，也需要先进行内存整理，防止永久内存碎片
	 */
	if (can_direct_reclaim &&
			(costly_order ||
			   (order > 0 && ac->migratetype != MIGRATE_MOVABLE))
			&& !gfp_pfmemalloc_allowed(gfp_mask)) {
		// 进行直接内存整理，获取更多的连续空闲内存防止内存碎片
		page = __alloc_pages_direct_compact(gfp_mask, order,
						alloc_flags, ac,
						INIT_COMPACT_PRIORITY,
						&compact_result);
		if (page)
			goto got_pg;

		/*
		 * Checks for costly allocations with __GFP_NORETRY, which
		 * includes some THP page fault allocations
		 */
		if (costly_order && (gfp_mask & __GFP_NORETRY)) {
			 // 流程走到这里表示经过内存整理之后依然没有足够的内存供分配
            // 但是设置了 NORETRY 标识不允许重试，那么就直接失败，跳转到 nopage
			/*
			 * If allocating entire pageblock(s) and compaction
			 * failed because all zones are below low watermarks
			 * or is prohibited because it recently failed at this
			 * order, fail immediately unless the allocator has
			 * requested compaction and reclaim retry.
			 *
			 * Reclaim is
			 *  - potentially very expensive because zones are far
			 *    below their low watermarks or this is part of very
			 *    bursty high order allocations,
			 *  - not guaranteed to help because isolate_freepages()
			 *    may not iterate over freed pages as part of its
			 *    linear scan, and
			 *  - unlikely to make entire pageblocks free on its
			 *    own.
			 */
			if (compact_result == COMPACT_SKIPPED ||
			    compact_result == COMPACT_DEFERRED)
				goto nopage;

			/*
			 * Looks like reclaim/compaction is worth trying, but
			 * sync compaction could be very expensive, so keep
			 * using async compaction.
			 */
			compact_priority = INIT_COMPACT_PRIORITY;
		}
	}

retry:
	/* Ensure kswapd doesn't accidentally go to sleep as long as we loop */
	// 确保所有 kswapd 进程不要意外进入睡眠状态
	if (alloc_flags & ALLOC_KSWAPD)
		wake_all_kswapds(order, gfp_mask, ac);

	// 流程走到这里，说明在 WMARK_MIN 水位线之上也分配内存失败了
    // 并且经过内存整理之后，内存分配仍然失败，说明当前内存容量已经严重不足
    // 接下来就需要使用更加激进的非常手段来尝试内存分配（忽略掉内存水位线），继续修改 alloc_flags 保存在 reserve_flags 中
	reserve_flags = __gfp_pfmemalloc_flags(gfp_mask);
	if (reserve_flags)
		alloc_flags = current_alloc_flags(gfp_mask, reserve_flags);

	/*
	 * Reset the nodemask and zonelist iterators if memory policies can be
	 * ignored. These allocations are high priority and system rather than
	 * user oriented.
	 */
	// 如果内存分配可以任意跨节点分配（忽略内存分配策略），这里需要重置 nodemask 以及 zonelist。
	if (!(alloc_flags & ALLOC_CPUSET) || reserve_flags) {
		ac->nodemask = NULL;
		ac->preferred_zoneref = first_zones_zonelist(ac->zonelist,
					ac->highest_zoneidx, ac->nodemask);
	}

	/* Attempt with potentially adjusted zonelist and alloc_flags */
	   // 这里使用重新调整的 zonelist 和 alloc_flags 在尝试进行一次内存分配
    // 注意此次的内存分配是忽略内存水位线的 ALLOC_NO_WATERMARKS
	page = get_page_from_freelist(gfp_mask, order, alloc_flags, ac);
	if (page)
		goto got_pg;

	/* Caller is not willing to reclaim, we can't balance anything */
	    // 在忽略内存水位线的情况下仍然分配失败，现在内核就需要进行直接内存回收了
	if (!can_direct_reclaim)
		goto nopage;

	/* Avoid recursion of direct reclaim */
	if (current->flags & PF_MEMALLOC)
		goto nopage;

	/* Try direct reclaim and then allocating */
	// 开始直接内存回收
	page = __alloc_pages_direct_reclaim(gfp_mask, order, alloc_flags, ac,
							&did_some_progress);
	if (page)
		goto got_pg;

	/* Try direct compaction and then allocating */
	// 直接内存回收之后仍然无法满足分配需求，则再次进行直接内存整理
	page = __alloc_pages_direct_compact(gfp_mask, order, alloc_flags, ac,
					compact_priority, &compact_result);
	if (page)
		goto got_pg;

	/* Do not loop if specifically requested */
	if (gfp_mask & __GFP_NORETRY)
		goto nopage;

	/*
	 * Do not retry costly high order allocations unless they are
	 * __GFP_RETRY_MAYFAIL
	 */
	// 后续会触发 OOM 来释放更多的内存，这里需要判断本次内存分配是否需要分配大量的内存页（大于 8 ） 
	// costly_order = true
    // 如果是的话则内核认为即使执行 OOM 也未必会满足这么多的内存页分配需求.
    // 所以还是直接失败比较好，不再执行 OOM，除非设置 __GFP_RETRY_MAYFAIL
	if (costly_order && !(gfp_mask & __GFP_RETRY_MAYFAIL))
		goto nopage;

    // 流程走到这里说明我们已经尝试了所有措施内存依然分配失败了，此时内存已经非常危急了。
    // 走到这里说明进程允许内核进行重试流程，但在开始重试之前，内核需要判断是否应该进行重试,重试标准：
    // 1 如果内核已经重试了 MAX_RECLAIM_RETRIES (16) 次仍然失败，则放弃重试执行后续 OOM。
    // 2 如果内核将所有可选内存区域中的所有可回收页面全部回收之后，仍然无法满足内存的分配，那么放弃重试执行后续 OOM
	if (should_reclaim_retry(gfp_mask, order, ac, alloc_flags,
				 did_some_progress > 0, &no_progress_loops))
		goto retry;

	/*
	 * It doesn't make any sense to retry for the compaction if the order-0
	 * reclaim is not able to make any progress because the current
	 * implementation of the compaction depends on the sufficient amount
	 * of free memory (see __compaction_suitable)
	 */
	if (did_some_progress > 0 &&
			should_compact_retry(ac, order, alloc_flags,
				compact_result, &compact_priority,
				&compaction_retries))
		goto retry;


	/* Deal with possible cpuset update races before we start OOM killing */
	if (check_retry_cpuset(cpuset_mems_cookie, ac))
		goto retry_cpuset;

	/* Reclaim has failed us, start killing things */
	// 最后的杀手锏，进行 OOM，选择一个得分最高的进程，释放其占用的内存 
	page = __alloc_pages_may_oom(gfp_mask, order, ac, &did_some_progress);
	if (page)
		goto got_pg;

	/* Avoid allocations with no watermarks from looping endlessly */
	if (tsk_is_oom_victim(current) &&
	    (alloc_flags & ALLOC_OOM ||
	     (gfp_mask & __GFP_NOMEMALLOC)))
		goto nopage;

	/* Retry as long as the OOM killer is making progress */
	if (did_some_progress) {
		no_progress_loops = 0;
		goto retry;
	}

nopage:
	/* Deal with possible cpuset update races before we fail */
	if (check_retry_cpuset(cpuset_mems_cookie, ac))
		goto retry_cpuset;

	/*
	 * Make sure that __GFP_NOFAIL request doesn't leak out and make sure
	 * we always retry
	 */
	if (gfp_mask & __GFP_NOFAIL) {
		/*
		 * All existing users of the __GFP_NOFAIL are blockable, so warn
		 * of any new users that actually require GFP_NOWAIT
		 */
		if (WARN_ON_ONCE(!can_direct_reclaim))
			goto fail;

		/*
		 * PF_MEMALLOC request from this context is rather bizarre
		 * because we cannot reclaim anything and only can loop waiting
		 * for somebody to do a work for us
		 */
		WARN_ON_ONCE(current->flags & PF_MEMALLOC);

		/*
		 * non failing costly orders are a hard requirement which we
		 * are not prepared for much so let's warn about these users
		 * so that we can identify them and convert them to something
		 * else.
		 */
		WARN_ON_ONCE(order > PAGE_ALLOC_COSTLY_ORDER);

		/*
		 * Help non-failing allocations by giving them access to memory
		 * reserves but do not use ALLOC_NO_WATERMARKS because this
		 * could deplete whole memory reserves which would just make
		 * the situation worse
		 */
		page = __alloc_pages_cpuset_fallback(gfp_mask, order, ALLOC_HARDER, ac);
		if (page)
			goto got_pg;

		cond_resched();
		goto retry;
	}
fail:
	warn_alloc(gfp_mask, ac->nodemask,
			"page allocation failure: order:%u", order);
got_pg:

	if (ac->migratetype == MIGRATE_MOVABLE)
		memcg_check_wmark_min_adj(current, ac);

	return page;
}
```