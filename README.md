Linux内核版本6.2 文件读写

```
time dd if=/dev/zero of=/dev/nvme0n1 bs=5M count=4096
```
![bs=5M](photo/blocksize_test/A.png)

```
time dd if=/dev/zero of=/dev/nvme0n1 bs=2K count=10485760
```
![bs=2K](photo/blocksize_test/B.png)
