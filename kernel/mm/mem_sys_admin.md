cat /proc/meminfo 

cat /proc/pagetypeinfo 

cat /proc/zoneinfo

echo m > /proc/sysrq-trigger 

[ 3260.277330] sysrq: Show Memory
[ 3260.277387] Mem-Info:
[ 3260.277390] active_anon:573 inactive_anon:29639 isolated_anon:0
                active_file:39619 inactive_file:312444 isolated_file:0
                unevictable:0 dirty:210 writeback:0
                slab_reclaimable:13992 slab_unreclaimable:9397
                mapped:37638 shmem:535 pagetables:1098 bounce:0
                free:3511417 free_pcp:1479 free_cma:0
[ 3260.277392] Node 0 active_anon:2292kB inactive_anon:118556kB active_file:158476kB inactive_file:1249776kB unevictable:0kB isolated(anon):0kB isolated(file):0kB mapped:150552kB dirty:840kB writeback:0kB shmem:2140kB shmem_thp: 0kB shmem_pmdmapped: 0kB anon_thp: 38912kB writeback_tmp:0kB kernel_stack:4064kB pagetables:4392kB all_unreclaimable? no
[ 3260.277392] Node 0 DMA free:13312kB min:64kB low:80kB high:96kB reserved_highatomic:0KB active_anon:0kB inactive_anon:0kB active_file:0kB inactive_file:0kB unevictable:0kB writepending:0kB present:15992kB managed:15360kB mlocked:0kB bounce:0kB free_pcp:0kB local_pcp:0kB free_cma:0kB
[ 3260.277395] lowmem_reserve[]: 0 2718 15455 15455 15455
[ 3260.277397] Node 0 DMA32 free:2780512kB min:11780kB low:14724kB high:17668kB reserved_highatomic:0KB active_anon:0kB inactive_anon:0kB active_file:0kB inactive_file:0kB unevictable:0kB writepending:0kB present:3111616kB managed:2783936kB mlocked:0kB bounce:0kB free_pcp:1376kB local_pcp:0kB free_cma:0kB
[ 3260.277399] lowmem_reserve[]: 0 0 12737 12737 12737
[ 3260.277401] Node 0 Normal free:11251844kB min:55732kB low:69664kB high:83596kB reserved_highatomic:0KB active_anon:2292kB inactive_anon:118556kB active_file:158476kB inactive_file:1249776kB unevictable:0kB writepending:840kB present:13369344kB managed:13042952kB mlocked:0kB bounce:0kB free_pcp:4540kB local_pcp:600kB free_cma:0kB
[ 3260.277403] lowmem_reserve[]: 0 0 0 0 0
[ 3260.277405] Node 0 DMA: 0*4kB 0*8kB 0*16kB 0*32kB 0*64kB 0*128kB 0*256kB 0*512kB 1*1024kB (U) 2*2048kB (UM) 2*4096kB (M) = 13312kB
[ 3260.277410] Node 0 DMA32: 2*4kB (M) 1*8kB (M) 1*16kB (M) 2*32kB (M) 2*64kB (M) 3*128kB (UM) 3*256kB (UM) 2*512kB (UM) 3*1024kB (M) 5*2048kB (UM) 675*4096kB (M) = 2780512kB
[ 3260.277417] Node 0 Normal: 55*4kB (UME) 23*8kB (UME) 7*16kB (UME) 2*32kB (M) 53*64kB (UME) 72*128kB (UME) 21*256kB (UME) 10*512kB (UME) 3*1024kB (M) 1*2048kB (U) 2740*4096kB (M) = 11251844kB
[ 3260.277425] Node 0 hugepages_total=0 hugepages_free=0 hugepages_surp=0 hugepages_size=1048576kB
[ 3260.277426] Node 0 hugepages_total=0 hugepages_free=0 hugepages_surp=0 hugepages_size=2048kB
[ 3260.277426] 352599 total pagecache pages
[ 3260.277427] 0 pages in swap cache
[ 3260.277428] Swap cache stats: add 0, delete 0, find 0/0
[ 3260.277429] Free swap  = 0kB
[ 3260.277429] Total swap = 0kB
[ 3260.277430] 4124238 pages RAM
[ 3260.277430] 0 pages HighMem/MovableOnly
[ 3260.277430] 163676 pages reserved
[ 3260.277431] 0 pages cma reserved
[ 3260.277431] 0 pages hwpoisoned


# 物理内存布局的探测  

[bios 内存探测](https://zhuanlan.zhihu.com/p/435020338)
bios 0x15 中断 e820 

16G 的系统
BIOS-e820: [mem 0x0000000000000000-0x000000000009fbff] usable  #654335
BIOS-e820: [mem 0x000000000009fc00-0x000000000009ffff] reserved
BIOS-e820: [mem 0x00000000000f0000-0x00000000000fffff] reserved
BIOS-e820: [mem 0x0000000000100000-0x000000009fffffff] usable #2683305983  2558M
BIOS-e820: [mem 0x00000000a0000000-0x00000000a111ffff] reserved
BIOS-e820: [mem 0x00000000a1120000-0x00000000bffcffff] usable #518717439 494M
BIOS-e820: [mem 0x00000000bffd0000-0x00000000bfffffff] reserved
BIOS-e820: [mem 0x00000000fffbc000-0x00000000ffffffff] reserved
BIOS-e820: [mem 0x0000000100000000-0x000000042fffffff] usable #13690208255 13055M
BIOS-e820: [mem 0x0000000430000000-0x000000043fffffff] reserved