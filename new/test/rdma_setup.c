#include <errno.h>
#include <pthread.h>
#include "se.h"

// struct IBR{
//     struct d_ib_context  *ctx;
//     struct d_ib_pd  *pd;
// ...

// };

//struct IBR *ibr;
// struct d_ib_device **dev_list == NULL;

// static pthread_once_t device_list_once = 0;

// static void count_devices(void)
// {
//     num_devices = d_idev_init(&device_list);
// }

// int d_idev_init(struct ibv_device ***list)
// {
// 	const char *sysfs_path;
// 	struct ibv_sysfs_dev *sysfs_dev, *next_dev;
// 	struct ibv_device *device;
// 	int num_devices = 0;
// 	int list_size = 0;
// 	int statically_linked = 0;
// 	int no_driver = 0;
// 	int ret;

//     *list = NULL
// }

// struct d_ib_device **d_ib_get_device_list(int *num)
// {
//     struct d_ib_device **l;
//     int i;
//     if(num)
//         *num=0;
    
//     pthread_once(&device_list_once, count_devices);
// }

// int d_rdma_setup () {
//     int ret = 0;
//     ibr=(struct IBR *)kzalloc(sizeof(struct IBR),GFP_KERNEL);

//     dev_list = d_ib_get_device_list(NULL);
//     if(dev_list == NULL){
// //        printk("dev_list fail(rdma_setup.c)");
//         p_err("get device list fail",dev_list);
//     }
// }