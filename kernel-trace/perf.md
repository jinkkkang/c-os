perf list

```
perf list -h

 Usage: perf list [<options>] [hw|sw|cache|tracepoint|pmu|sdt|metric|metricgroup|event_glob]

    -d, --desc            Print extra event descriptions. --no-desc to not print.
    -v, --long-desc       Print longer event descriptions.
        --debug           Enable debugging output
        --deprecated      Print deprecated events.
        --details         Print information on the perf event names and expressions used internally by events.


1. hw or hardware to list hardware events such as cache-misses, etc. 硬件事件

2. sw or software to list software events such as context switches, etc. 软件事件

3. cache or hwcache to list hardware cache events such as L1-dcache-loads, etc. 硬件cache相关事件

4. tracepoint to list all tracepoint events, alternatively use subsys_glob:event_glob to filter by tracepoint subsystems such as sched, block, etc. 持的所有tracepoint列表

5. pmu to print the kernel supplied PMU events. 支持的PMU事件列表

6. sdt to list all Statically Defined Tracepoint events.

7. metric to list metrics

8. metricgroup to list metricgroups with metrics.
```



perf top
第一列：符号引发的性能事件的比例，指占用的cpu周期比例。

第二列：符号所在的DSO(Dynamic Shared Object)，可以是应用程序、内核、动态链接库、模块。

第三列：DSO的类型。[.]表示此符号属于用户态的ELF文件，包括可执行文件与动态链接库；[k]表述此符号属于内核或模块。

第四列：符号名。有些符号不能解析为函数名，只能用地址表示。

```
amples: 2K of event 'cpu-clock:pppH', 4000 Hz, Event count (approx.): 261863598 lost: 0/0 drop: 0/0
Overhead  Shared Object             Symbol
  12.22%  [kernel]                  [k] intel_idle_irq
   5.83%  libc-2.32.so              [.] _int_malloc
   5.43%  libc-2.32.so              [.] _int_free
   2.98%  [kernel]                  [k] cpuidle_enter_state
   2.39%  [kernel]                  [k] kallsyms_expand_symbol.constprop.0
   2.39%  perf                      [.] evsel__parse_sample
   2.36%  perf                      [.] ordered_events__queue
   1.88%  perf                      [.] rb_next
   1.77%  perf                      [.] kallsyms__parse
   1.60%  [kernel]                  [k] finish_task_switch
   1.30%  [kernel]                  [k] _raw_spin_unlock_irqrestore
   1.29%  libc-2.32.so              [.] malloc
   1.22%  perf                      [.] __symbols__insert
   1.15%  [kernel]                  [k] vsnprintf
   0.85%  [kernel]                  [k] number
   0.77%  [kernel]                  [k] format_decode
```


# 常用命令

perf stat -e kmem:mm_page_alloc -e kmem:mm_page_free \
               -e kmem:mm_page_free_batched  ./hackbench 10