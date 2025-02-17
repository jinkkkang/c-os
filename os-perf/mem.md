# pmap

pmap - report memory map of a process

/proc/$pid/smaps

```
pmap -x  92002
92002:   /usr/local/share/aliyun-assist/2.2.3.616/aliyun-service
Address           Kbytes     RSS   Dirty Mode  Mapping
0000000008048000   10532    7588       0 r-x-- aliyun-service
0000000008a91000   10840    7384       0 r---- aliyun-service
0000000009527000     524     396     100 rw--- aliyun-service
00000000095aa000     148      76      76 rw---   [ anon ]
0000000009c00000    8192    3980    3980 rw---   [ anon ]
000000000a400000  520192       0       0 -----   [ anon ]
00000000ef796000    1940    1292    1292 rw---   [ anon ]
00000000ef97b000  137144       0       0 -----   [ anon ]
00000000f7f69000     268      44      44 rw---   [ anon ]
00000000f7fac000      16       0       0 r----   [ anon ]
00000000f7fb0000       8       8       0 r-x--   [ anon ]
00000000ffe44000     132      16      16 rw---   [ stack ]
---------------- ------- ------- ------- 
total kB          689936   20784    5508
```


# sar 命令


# vmstat 命令