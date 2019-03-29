#include <linux/init.h>
#include <linux/module.h>
#include <rdma/ib_verbs.h>
#include <linux/kfifo.h>
#include <linux/kthread.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/pci.h>
#include <linux/proc_fs.h>
#include <rdma/rdma_cm.h>
#include <linux/semaphore.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/mutex.h>
#include <linux/netdevice.h>
#include <linux/security.h>
#include <linux/notifier.h>
#include <rdma/rdma_netlink.h>
#include <rdma/ib_addr.h>
#include <rdma/ib_cache.h>
static DEFINE_MUTEX(device_mutex);
static LIST_HEAD(device_list);
static LIST_HEAD(client_list);

static void dnova_ib_add_one(struct ib_device *device) {};

static void dnova_ib_remove_one(struct ib_device *device, void *client_data) {};

static void dnova_ib_release_dev(struct device *dev) {};

struct ib_client dn_ib_client = {.name = "dnova_ib_client",
				 .add = dnova_ib_add_one,
				 .remove = dnova_ib_remove_one};

static int __init hello_init(void)
{
	
	int ret;
	struct ib_client *n;

	struct ib_device *device;
	n->name = "a";
	n->add = dnova_ib_add_one;
	n->remove = dnova_ib_remove_one;

	ret = ib_register_client(&dn_ib_client);
	if (ret)
		printk("fail register_client");	
	
	mutex_lock(&device_mutex);

	list_for_each_entry(device, &device_list, core_list)
		if(n->add)
		{
			n->add(device);
			printk("device is no 0;");
			printk("device:%c",device);
		}
	 mutex_unlock(&device_mutex);
			
	return ret;
}


static void __exit hello_exit(void)
{
        printk (KERN_ALERT "Goodbye, cruel world\n");
}
module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");

