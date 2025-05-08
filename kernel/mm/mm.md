

https://docs.kernel.org/mm/physical_memory.html


内存的布局




```
/*
 * On NUMA machines, each NUMA node would have a pg_data_t to describe
 * it's memory layout. On UMA machines there is a single pglist_data which
 * describes the whole memory.
 *
 * Memory statistics and page replacement data structures are maintained on a
 * per-zone basis.
 */
typedef struct pglist_data {
	/*
	 * node_zones contains just the zones for THIS node. Not all of the
	 * zones may be populated, but it is the full list. It is referenced by
	 * this node's node_zonelists as well as other node's node_zonelists.
	 */
	struct zone node_zones[MAX_NR_ZONES];  /*  这个node存在的区域zone,  DMA, DMA32, Normal ... */

	/*
	 * node_zonelists contains references to all zones in all nodes.
	 * Generally the first zones will be references to this node's
	 * node_zones.
	 */
	struct zonelist node_zonelists[MAX_ZONELISTS]; /* 分配zone时的顺序 */

	int nr_zones; /* number of populated zones in this node; zone 的数量*/

#ifdef CONFIG_FLATMEM	/* means !SPARSEMEM */
	struct page *node_mem_map; /* 该节点的第一个页面在全局变量 mem_map 数组中的地址 */
#ifdef CONFIG_PAGE_EXTENSION
	struct page_ext *node_page_ext;
#endif
#endif
#if defined(CONFIG_MEMORY_HOTPLUG) || defined(CONFIG_DEFERRED_STRUCT_PAGE_INIT)
	/*
	 * Must be held any time you expect node_start_pfn,
	 * node_present_pages, node_spanned_pages or nr_zones to stay constant.
	 * Also synchronizes pgdat->first_deferred_pfn during deferred page
	 * init.
	 *
	 * pgdat_resize_lock() and pgdat_resize_unlock() are provided to
	 * manipulate node_size_lock without checking for CONFIG_MEMORY_HOTPLUG
	 * or CONFIG_DEFERRED_STRUCT_PAGE_INIT.
	 *
	 * Nests above zone->lock and zone->span_seqlock
	 */
	spinlock_t node_size_lock;
#endif
	unsigned long node_start_pfn; /* 该节点起始的物理页号 */
	unsigned long node_present_pages; /* total number of physical pages 该节点总的物理页面数, 不包含内存空洞 */
	unsigned long node_spanned_pages; /* total size of physical page
					     range, including holes  该节点总的物理页面数 包括内存空洞 */
	int node_id; /* 节点id, 从 0 开始 */
	wait_queue_head_t kswapd_wait;
	wait_queue_head_t pfmemalloc_wait;

	/* workqueues for throttling reclaim for different reasons. */
	wait_queue_head_t reclaim_wait[NR_VMSCAN_THROTTLE];

	atomic_t nr_writeback_throttled;/* nr of writeback-throttled tasks */
	unsigned long nr_reclaim_start;	/* nr pages written while throttled
					 * when throttling started. */
#ifdef CONFIG_MEMORY_HOTPLUG
	struct mutex kswapd_lock;
#endif
    # 页面回收进程，内核会为每个 NUMA 节点分配一个 kswapd 进程用于回收不经常使用的页面，
	# 还会为每个 NUMA 节点分配一个 kcompactd 进程用于内存的规整避免内存碎片
	struct task_struct *kswapd;	/* Protected by kswapd_lock */
	int kswapd_order;
	enum zone_type kswapd_highest_zoneidx;

	int kswapd_failures;		/* Number of 'reclaimed == 0' runs */

#ifdef CONFIG_COMPACTION
	int kcompactd_max_order;
	enum zone_type kcompactd_highest_zoneidx;
	wait_queue_head_t kcompactd_wait;
	struct task_struct *kcompactd;
	bool proactive_compact_trigger;
#endif
	/*
	 * This is a per-node reserve of pages that are not available
	 * to userspace allocations.
	 */
	unsigned long		totalreserve_pages;

#ifdef CONFIG_NUMA
	/*
	 * node reclaim becomes active if more unmapped pages exist.
	 */
	unsigned long		min_unmapped_pages;
	unsigned long		min_slab_pages;
#endif /* CONFIG_NUMA */

	/* Write-intensive fields used by page reclaim */
	CACHELINE_PADDING(_pad1_);

#ifdef CONFIG_DEFERRED_STRUCT_PAGE_INIT
	/*
	 * If memory initialisation on large machines is deferred then this
	 * is the first PFN that needs to be initialised.
	 */
	unsigned long first_deferred_pfn;
#endif /* CONFIG_DEFERRED_STRUCT_PAGE_INIT */

#ifdef CONFIG_TRANSPARENT_HUGEPAGE
	struct deferred_split deferred_split_queue;
#endif

#ifdef CONFIG_NUMA_BALANCING
	/* start time in ms of current promote rate limit period */
	unsigned int nbp_rl_start;
	/* number of promote candidate pages at start time of current rate limit period */
	unsigned long nbp_rl_nr_cand;
	/* promote threshold in ms */
	unsigned int nbp_threshold;
	/* start time in ms of current promote threshold adjustment period */
	unsigned int nbp_th_start;
	/*
	 * number of promote candidate pages at start time of current promote
	 * threshold adjustment period
	 */
	unsigned long nbp_th_nr_cand;
#endif
	/* Fields commonly accessed by the page reclaim scanner */

	/*
	 * NOTE: THIS IS UNUSED IF MEMCG IS ENABLED.
	 *
	 * Use mem_cgroup_lruvec() to look up lruvecs.
	 */
	struct lruvec		__lruvec;

	unsigned long		flags;

#ifdef CONFIG_LRU_GEN
	/* kswap mm walk data */
	struct lru_gen_mm_walk mm_walk;
	/* lru_gen_folio list */
	struct lru_gen_memcg memcg_lru;
#endif

	CACHELINE_PADDING(_pad2_);

	/* Per-node vmstats */
	struct per_cpu_nodestat __percpu *per_cpu_nodestats;
	atomic_long_t		vm_stat[NR_VM_NODE_STAT_ITEMS];
#ifdef CONFIG_NUMA
	struct memory_tier __rcu *memtier;
#endif
#ifdef CONFIG_MEMORY_FAILURE
	struct memory_failure_stats mf_stats;
#endif
} pg_data_t;

```


# struct zone_type

硬件通常会限制访问不同物理内存范围的方式。在某些情况下，设备无法对所有可寻址内存执行 DMA。在其他情况下，物理内存的大小超出了虚拟内存的最大可寻址大小，需要采取特殊操作才能访问部分内存。Linux 根据内存页面的可能用途将其分组到区域中。例如，ZONE_DMA 将包含设备可用于 DMA 的内存，ZONE_HIGHMEM 将包含未永久映射到内核地址空间的内存，ZONE_NORMAL 将包含正常寻址的页面。

内存区域的实际布局取决于硬件，因为并非所有架构都定义所有区域，并且不同平台对 DMA 的要求也不同。


```

enum zone_type {
	/*
	 * ZONE_DMA and ZONE_DMA32 are used when there are peripherals not able
	 * to DMA to all of the addressable memory (ZONE_NORMAL).
	 * On architectures where this area covers the whole 32 bit address
	 * space ZONE_DMA32 is used. ZONE_DMA is left for the ones with smaller
	 * DMA addressing constraints. This distinction is important as a 32bit
	 * DMA mask is assumed when ZONE_DMA32 is defined. Some 64-bit
	 * platforms may need both zones as they support peripherals with
	 * different DMA addressing limitations.
	 */
#ifdef CONFIG_ZONE_DMA
	ZONE_DMA,
#endif
#ifdef CONFIG_ZONE_DMA32
	ZONE_DMA32,
#endif
	/*
	 * Normal addressable memory is in ZONE_NORMAL. DMA operations can be
	 * performed on pages in ZONE_NORMAL if the DMA devices support
	 * transfers to all addressable memory.
	 */
	ZONE_NORMAL,
#ifdef CONFIG_HIGHMEM
	/*
	 * A memory area that is only addressable by the kernel through
	 * mapping portions into its own address space. This is for example
	 * used by i386 to allow the kernel to address the memory beyond
	 * 900MB. The kernel will set up special mappings (page
	 * table entries on i386) for each page that the kernel needs to
	 * access.
	 */
	ZONE_HIGHMEM,
#endif
	/*
	 * ZONE_MOVABLE is similar to ZONE_NORMAL, except that it contains
	 * movable pages with few exceptional cases described below. Main use
	 * cases for ZONE_MOVABLE are to make memory offlining/unplug more
	 * likely to succeed, and to locally limit unmovable allocations - e.g.,
	 * to increase the number of THP/huge pages. Notable special cases are:
	 *
	 * 1. Pinned pages: (long-term) pinning of movable pages might
	 *    essentially turn such pages unmovable. Therefore, we do not allow
	 *    pinning long-term pages in ZONE_MOVABLE. When pages are pinned and
	 *    faulted, they come from the right zone right away. However, it is
	 *    still possible that address space already has pages in
	 *    ZONE_MOVABLE at the time when pages are pinned (i.e. user has
	 *    touches that memory before pinning). In such case we migrate them
	 *    to a different zone. When migration fails - pinning fails.
	 * 2. memblock allocations: kernelcore/movablecore setups might create
	 *    situations where ZONE_MOVABLE contains unmovable allocations
	 *    after boot. Memory offlining and allocations fail early.
	 * 3. Memory holes: kernelcore/movablecore setups might create very rare
	 *    situations where ZONE_MOVABLE contains memory holes after boot,
	 *    for example, if we have sections that are only partially
	 *    populated. Memory offlining and allocations fail early.
	 * 4. PG_hwpoison pages: while poisoned pages can be skipped during
	 *    memory offlining, such pages cannot be allocated.
	 * 5. Unmovable PG_offline pages: in paravirtualized environments,
	 *    hotplugged memory blocks might only partially be managed by the
	 *    buddy (e.g., via XEN-balloon, Hyper-V balloon, virtio-mem). The
	 *    parts not manged by the buddy are unmovable PG_offline pages. In
	 *    some cases (virtio-mem), such pages can be skipped during
	 *    memory offlining, however, cannot be moved/allocated. These
	 *    techniques might use alloc_contig_range() to hide previously
	 *    exposed pages from the buddy again (e.g., to implement some sort
	 *    of memory unplug in virtio-mem).
	 * 6. ZERO_PAGE(0), kernelcore/movablecore setups might create
	 *    situations where ZERO_PAGE(0) which is allocated differently
	 *    on different platforms may end up in a movable zone. ZERO_PAGE(0)
	 *    cannot be migrated.
	 * 7. Memory-hotplug: when using memmap_on_memory and onlining the
	 *    memory to the MOVABLE zone, the vmemmap pages are also placed in
	 *    such zone. Such pages cannot be really moved around as they are
	 *    self-stored in the range, but they are treated as movable when
	 *    the range they describe is about to be offlined.
	 *
	 * In general, no unmovable allocations that degrade memory offlining
	 * should end up in ZONE_MOVABLE. Allocators (like alloc_contig_range())
	 * have to expect that migrating pages in ZONE_MOVABLE can fail (even
	 * if has_unmovable_pages() states that there are no unmovable pages,
	 * there can be false negatives).
	 */
	ZONE_MOVABLE,
#ifdef CONFIG_ZONE_DEVICE
	ZONE_DEVICE,
#endif
	__MAX_NR_ZONES

};
```


# struct zone

```
struct zone {
	/* Read-mostly fields */

	/* zone watermarks, access with *_wmark_pages(zone) macros */
	//物理内存区域中的水位线
	unsigned long _watermark[NR_WMARK];
	unsigned long watermark_boost;

	unsigned long nr_reserved_highatomic;

	/*
	 * We don't know if the memory that we're going to allocate will be
	 * freeable or/and it will be released eventually, so to avoid totally
	 * wasting several GB of ram we must reserve some of the lower zone
	 * memory (otherwise we risk to run OOM on the lower zones despite
	 * there being tons of freeable ram on the higher zones).  This array is
	 * recalculated at runtime if the sysctl_lowmem_reserve_ratio sysctl
	 * changes.
	 */
	long lowmem_reserve[MAX_NR_ZONES];

#ifdef CONFIG_NUMA
	int node;
#endif
	// 指向该内存区域所属的 NUMA 节点
	struct pglist_data	*zone_pgdat;
	 /* 每个cpu的冷热页管理 */
	struct per_cpu_pages	__percpu *per_cpu_pageset; 
	struct per_cpu_zonestat	__percpu *per_cpu_zonestats;
	/*
	 * the high and batch values are copied to individual pagesets for
	 * faster access
	 */
	int pageset_high_min;
	int pageset_high_max;
	int pageset_batch;

#ifndef CONFIG_SPARSEMEM
	/*
	 * Flags for a pageblock_nr_pages block. See pageblock-flags.h.
	 * In SPARSEMEM, this map is stored in struct mem_section
	 */
	unsigned long		*pageblock_flags;
#endif /* CONFIG_SPARSEMEM */

	// 属于该内存区域中的第一个物理页 PFN
	/* zone_start_pfn == zone_start_paddr >> PAGE_SHIFT */
	unsigned long		zone_start_pfn;

	/*
	 * spanned_pages is the total pages spanned by the zone, including
	 * holes, which is calculated as:
	 // 该内存区域中所有的物理页个数（包含内存空洞）
	 * 	spanned_pages = zone_end_pfn - zone_start_pfn;
	 *
	 * present_pages is physical pages existing within the zone, which
	 * is calculated as:
	 * // 该内存区域所有可用的物理页个数（不包含内存空洞）
	 *	present_pages = spanned_pages - absent_pages(pages in holes);
	 *
	 * present_early_pages is present pages existing within the zone
	 * located on memory available since early boot, excluding hotplugged
	 * memory.
	 *
	 * managed_pages is present pages managed by the buddy system, which
	 * is calculated as (reserved_pages includes pages allocated by the
	 * bootmem allocator):
	*  被伙伴系统所管理的物理页数
	 *	managed_pages = present_pages - reserved_pages;
	 *
	 * cma pages is present pages that are assigned for CMA use
	 * (MIGRATE_CMA).
	 *
	 * So present_pages may be used by memory hotplug or memory power
	 * management logic to figure out unmanaged pages by checking
	 * (present_pages - managed_pages). And managed_pages should be used
	 * by page allocator and vm scanner to calculate all kinds of watermarks
	 * and thresholds.
	 *
	 * Locking rules:
	 *
	 * zone_start_pfn and spanned_pages are protected by span_seqlock.
	 * It is a seqlock because it has to be read outside of zone->lock,
	 * and it is done in the main allocator path.  But, it is written
	 * quite infrequently.
	 *
	 * The span_seq lock is declared along with zone->lock because it is
	 * frequently read in proximity to zone->lock.  It's good to
	 * give them a chance of being in the same cacheline.
	 *
	 * Write access to present_pages at runtime should be protected by
	 * mem_hotplug_begin/done(). Any reader who can't tolerant drift of
	 * present_pages should use get_online_mems() to get a stable value.
	 */
	atomic_long_t		managed_pages;
	unsigned long		spanned_pages;
	unsigned long		present_pages;
#if defined(CONFIG_MEMORY_HOTPLUG)
	unsigned long		present_early_pages;
#endif
#ifdef CONFIG_CMA
	unsigned long		cma_pages;
#endif

	// 内存区域名称：Normal ，DMA，HighMem
	const char		*name;

#ifdef CONFIG_MEMORY_ISOLATION
	/*
	 * Number of isolated pageblock. It is used to solve incorrect
	 * freepage counting problem due to racy retrieving migratetype
	 * of pageblock. Protected by zone->lock.
	 */
	unsigned long		nr_isolate_pageblock;
#endif

#ifdef CONFIG_MEMORY_HOTPLUG
	/* see spanned/present_pages for more description */
	seqlock_t		span_seqlock;
#endif

	int initialized;

	/* Write-intensive fields used from the page allocator */
	CACHELINE_PADDING(_pad1_);

	/* free areas of different sizes  重要*/
	//// 伙伴系统的核心数据结构 
	struct free_area	free_area[NR_PAGE_ORDERS];

#ifdef CONFIG_UNACCEPTED_MEMORY
	/* Pages to be accepted. All pages on the list are MAX_PAGE_ORDER */
	struct list_head	unaccepted_pages;
#endif

	/* zone flags, see below */
	unsigned long		flags;

	/* Primarily protects free_area */
	spinlock_t		lock;

	/* Write-intensive fields used by compaction and vmstats. */
	CACHELINE_PADDING(_pad2_);

	/*
	 * When free pages are below this point, additional steps are taken
	 * when reading the number of free pages to avoid per-cpu counter
	 * drift allowing watermarks to be breached
	 */
	unsigned long percpu_drift_mark;

#if defined CONFIG_COMPACTION || defined CONFIG_CMA
	/* pfn where compaction free scanner should start */
	unsigned long		compact_cached_free_pfn;
	/* pfn where compaction migration scanner should start */
	unsigned long		compact_cached_migrate_pfn[ASYNC_AND_SYNC];
	unsigned long		compact_init_migrate_pfn;
	unsigned long		compact_init_free_pfn;
#endif

#ifdef CONFIG_COMPACTION
	/*
	 * On compaction failure, 1<<compact_defer_shift compactions
	 * are skipped before trying again. The number attempted since
	 * last failure is tracked with compact_considered.
	 * compact_order_failed is the minimum compaction failed order.
	 */
	unsigned int		compact_considered;
	unsigned int		compact_defer_shift;
	int			compact_order_failed;
#endif

#if defined CONFIG_COMPACTION || defined CONFIG_CMA
	/* Set to true when the PG_migrate_skip bits should be cleared */
	bool			compact_blockskip_flush;
#endif

	bool			contiguous;

	CACHELINE_PADDING(_pad3_);
	/* Zone statistics */
	//  该内存区域内存使用的统计信息
	atomic_long_t		vm_stat[NR_VM_ZONE_STAT_ITEMS];
	atomic_long_t		vm_numa_event[NR_VM_NUMA_EVENT_ITEMS];
} ____cacheline_internodealigned_in_smp;

enum pgdat_flags {
	PGDAT_DIRTY,			/* reclaim scanning has recently found
					 * many dirty file pages at the tail
					 * of the LRU.
					 */
	PGDAT_WRITEBACK,		/* reclaim scanning has recently found
					 * many pages under writeback
					 */
	PGDAT_RECLAIM_LOCKED,		/* prevents concurrent reclaim */
};

enum zone_flags {
	ZONE_BOOSTED_WATERMARK,		/* zone recently boosted watermarks.
					 * Cleared when kswapd is woken.
					 */
	ZONE_RECLAIM_ACTIVE,		/* kswapd may be scanning the zone. */
	ZONE_BELOW_HIGH,		/* zone is below high watermark. */
};
```


```
struct free_area {
	struct list_head	free_list[MIGRATE_TYPES];  /*空闲页面块的双链表*/
	unsigned long		nr_free;  /*该区域的空闲页面块数量*/
};
```