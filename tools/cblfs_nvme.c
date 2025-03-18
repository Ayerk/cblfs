#include <linux/module.h>

#include <linux/fs.h>
#include <linux/bio.h>
#include <linux/blkdev.h>

//dev/nvme0n1

#define DISK_NAME "/dev/nvme0n1"

// 添加函数前向声明
int cblfs_nvme_init(void);
void cblfs_nvme_exit(void);


static void cblfs_nvme_write(struct block_device *bdev, char *buf,int length){

    struct page *page;

    struct bio *bio=bio_alloc(bdev, BIO_MAX_VECS, REQ_OP_WRITE, GFP_NOIO);
    bio->bi_iter.bi_sector = 0;

    page = alloc_page(GFP_KERNEL);
    if(!page){
        printk(KERN_ERR "alloc_page failed\n");
        return;
    }

    memcpy(page_address(page), buf, length);
    bio_add_page(bio, page, 512, 0);
    submit_bio_wait(bio);

    __free_page(page);
    bio_put(bio);

}

static void cblfs_nvme_read(struct block_device *bdev, char *buf,int length){

    struct page *page;

    struct bio *bio=bio_alloc(bdev, BIO_MAX_VECS, REQ_OP_READ, GFP_NOIO);
    bio->bi_iter.bi_sector = 0;

    page = alloc_page(GFP_KERNEL);
    if(!page){
        printk(KERN_ERR "alloc_page failed\n");
        return;
    }
    // bio->bi_private = page;
    bio_add_page(bio, page, 512, 0);

    submit_bio_wait(bio);

    memcpy(buf, page_address(page), length);

    __free_page(page);
    bio_put(bio);
}

// insmod cblfs_nvme.ko
int cblfs_nvme_init(void){

    char *str = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char buffer[512]={0};

    struct block_device *bdev = blkdev_get_by_path(DISK_NAME, FMODE_READ | FMODE_WRITE, THIS_MODULE);
    if (!bdev) {
        printk(KERN_ERR "blkdev_get_by_path failed\n");
        return -EINVAL;
    }


    cblfs_nvme_write(bdev, str, strlen(str));

    cblfs_nvme_read(bdev, buffer, 512);

    printk("buffer : %s\n",buffer);

    return 0;
}

//rmmod cblfs_nvme.ko
void cblfs_nvme_exit(void){
    printk(KERN_INFO "cblfs_nvme_exit\n");

}

module_init(cblfs_nvme_init);
module_exit(cblfs_nvme_exit);
MODULE_LICENSE("GPL");