

kernel.softlockup_panic

```
  541.071115] watchdog: BUG: soft lockup - CPU#0 stuck for 111s! [insmod:2279]
[  541.071228] Modules linked in: AliSecNetFlt64(OE+) hello(OE+) AliSecGuard(OE) binfmt_misc cfg80211 rfkill nft_fib_inet nft_fib_ipv4 nft_fib_ipv6 nft_fib nft_reject_inet nf_reject_ipv4 nf_reject_ipv6 nft_reject nft_ct nft_chain_nat nf_nat nf_conntrack nf_defrag_ipv6 nf_defrag_ipv4 mousedev ip_set nf_tables libcrc32c nfnetlink intel_rapl_msr intel_rapl_common nfit intel_powerclamp crct10dif_pclmul crc32_pclmul ghash_clmulni_intel rapl psmouse sunrpc serio_raw pcspkr i2c_piix4 virtio_balloon vfat fat virtio_net net_failover crc32c_intel failover cirrus drm_kms_helper syscopyarea sysfillrect sysimgblt fb_sys_fops drm i2c_core virtio_console
[  541.071255] CPU: 0 PID: 2279 Comm: insmod Kdump: loaded Tainted: G           OE     5.10.134-16.1.al8.x86_64 #1
[  541.071256] Hardware name: Alibaba Cloud Alibaba Cloud ECS, BIOS 2221b89 04/01/2014
[  541.071258] RIP: 0010:queued_spin_lock_slowpath.part.0+0x41/0x1a0
[  541.071259] Code: 2f 08 0f 92 c0 0f b6 c0 c1 e0 08 89 c2 8b 07 30 e4 09 d0 a9 00 01 ff ff 0f 85 f6 00 00 00 85 c0 74 0e 8b 07 84 c0 74 08 f3 90 <8b> 07 84 c0 75 f8 b8 01 00 00 00 66 89 07 c3 cc cc cc cc 8b 37 b8
[  541.071260] RSP: 0018:ffffb23c0093fde8 EFLAGS: 00000202
[  541.071261] RAX: 0000000000000101 RBX: 0000000000000000 RCX: 0000000000000000
[  541.071262] RDX: 0000000000000000 RSI: 0000000000000000 RDI: ffffb23c0093fdf8
[  541.071263] RBP: ffffffffc0627000 R08: 0000000000000010 R09: ffffda21842aa6c8
[  541.071263] R10: 000000000010aa7d R11: 0000000000000000 R12: ffff9064864d22e0
[  541.071264] R13: ffffb23c0093fe98 R14: 0000000000000003 R15: 0000000000000000
[  541.071265] FS:  00007f09c0b0e740(0000) GS:ffff9067a0000000(0000) knlGS:0000000000000000
[  541.071266] CS:  0010 DS: 0000 ES: 0000 CR0: 0000000080050033
[  541.071267] CR2: 000055d78e434e98 CR3: 000000010a620006 CR4: 00000000007706f0
[  541.071269] DR0: 0000000000000000 DR1: 0000000000000000 DR2: 0000000000000000
[  541.071269] DR3: 0000000000000000 DR6: 00000000fffe0ff0 DR7: 0000000000000400
[  541.071270] PKRU: 55555554
[  541.071270] Call Trace:
[  541.071285]  init_hello+0x30/0x1000 [hello]
[  541.071289]  ? do_one_initcall+0x41/0x1d0
[  541.071292]  ? do_init_module+0x23/0x250
[  541.071294]  ? kmem_cache_alloc_trace+0x11f/0x250
[  541.071295]  ? do_init_module+0x4c/0x250
[  541.071296]  ? __do_sys_finit_module+0x98/0xe0
[  541.071299]  ? do_syscall_64+0x30/0x40
[  541.071301]  ? entry_SYSCALL_64_after_hwframe+0x61/0xc6
[  541.071303] Kernel panic - not syncing: softlockup: hung tasks
[  541.071386] CPU: 0 PID: 2279 Comm: insmod Kdump: loaded Tainted: G           OEL    5.10.134-16.1.al8.x86_64 #1
[  541.071533] Hardware name: Alibaba Cloud Alibaba Cloud ECS, BIOS 2221b89 04/01/2014
[  541.071641] Call Trace:
[  541.071679]  <IRQ>
[  541.071711]  ? report_softlockup+0x1d0/0x1d0
[  541.071774]  dump_stack+0x57/0x6e
[  541.071824]  panic+0x10d/0x2e9
[  541.071871]  watchdog_timer_fn.cold+0xc/0x16
[  541.071934]  __run_hrtimer+0x5c/0x190
[  541.071988]  __hrtimer_run_queues+0x71/0xd0
[  541.072049]  hrtimer_interrupt+0x110/0x2c0
[  541.072111]  __sysvec_apic_timer_interrupt+0x5c/0xe0
[  541.072183]  asm_call_irq_on_stack+0xf/0x20
[  541.072243]  </IRQ>
[  541.072276]  sysvec_apic_timer_interrupt+0x72/0x80
[  541.072345]  asm_sysvec_apic_timer_interrupt+0x12/0x20
[  541.072419] RIP: 0010:queued_spin_lock_slowpath.part.0+0x41/0x1a0
[  541.072512] Code: 2f 08 0f 92 c0 0f b6 c0 c1 e0 08 89 c2 8b 07 30 e4 09 d0 a9 00 01 ff ff 0f 85 f6 00 00 00 85 c0 74 0e 8b 07 84 c0 74 08 f3 90 <8b> 07 84 c0 75 f8 b8 01 00 00 00 66 89 07 c3 cc cc cc cc 8b 37 b8
[  541.072767] RSP: 0018:ffffb23c0093fde8 EFLAGS: 00000202
[  541.072842] RAX: 0000000000000101 RBX: 0000000000000000 RCX: 0000000000000000
[  541.072941] RDX: 0000000000000000 RSI: 0000000000000000 RDI: ffffb23c0093fdf8
[  541.073039] RBP: ffffffffc0627000 R08: 0000000000000010 R09: ffffda21842aa6c8
[  541.073142] R10: 000000000010aa7d R11: 0000000000000000 R12: ffff9064864d22e0
[  541.073241] R13: ffffb23c0093fe98 R14: 0000000000000003 R15: 0000000000000000
[  541.073342]  ? 0xffffffffc0627000
[  541.073392]  ? __vunmap+0x112/0x190
[  541.073452]  init_hello+0x30/0x1000 [hello]
[  541.073513]  ? do_one_initcall+0x41/0x1d0
[  541.073571]  ? do_init_module+0x23/0x250
[  541.073628]  ? kmem_cache_alloc_trace+0x11f/0x250
[  541.073696]  ? do_init_module+0x4c/0x250
[  541.073753]  ? __do_sys_finit_module+0x98/0xe0
[  541.073818]  ? do_syscall_64+0x30/0x40
[  541.073873]  ? entry_SYSCALL_64_after_hwframe+0x61/0xc6
[  541.075616] Kernel Offset: 0x31000000 from 0xffffffff81000000 (relocation range: 0xffffffff80000000-0xffffffffbfffffff)
```