#include "se.h"
#include <linux/kthread.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/module.h>

#define S_C 1
#define SOCK_PORT_NUM 7657
#define MSG_SIZE 64
#define NUM_CONCURR_MSGS 1

struct coninfo *global_info;
EXPORT_SYMBOL(global_info);
extern struct ib_device *d_dev;
extern int ib_port;
extern dctx *ddpfs_ctx;

static int __init d_rdma_init_module(void)
{
    int ret = 0;

    global_info = kzalloc(sizeof(struct coninfo),GFP_KERNEL);
    if(S_C == 1){
        global_info->is_server = false;
        global_info->num_concurr_msgs = (int) NUM_CONCURR_MSGS;
        global_info->sock_port_num = (int) SOCK_PORT_NUM;
        global_info->sock_port = &global_info->sock_port_num;
        global_info->server_name = "192.168.99.11";
        global_info->msg_size = (int)MSG_SIZE;
    }
    else{
        global_info->sock_port_num = (int)SOCK_PORT_NUM,
        global_info->is_server = true,
        global_info->num_concurr_msgs = (int)NUM_CONCURR_MSGS,
        global_info->sock_port = &global_info->sock_port_num,
        global_info->msg_size = (int)MSG_SIZE
    }
    
    printk_coninfo(*global_info);

//    ret = d_rdma_setup();

//设备，在add_one中产生设备pd；
    ret = ib_register_client(&ddpfs_ib_client);
    if(ret){
        p_err("ib_register fail", &ddpfs_ib_client);
        return ret;
    }

    if(rdma_port_get_link_layer(d_dev, ib_port)!=IB_LINK_LAYER_INFINIBAND){
        p_err("ib_layer is not Infiniband,is:%s",rdma_port_get_link_layer(d_dev, ib_port));
        return ret;
    }

    ret = d_ib_query_port(d_dev, ib_port, ddpfs_ctx->port_attr)；

    ret = ddpfs_setup_local_memory（ddpfs_ctx）;

}

static void __init d_rdma_cleanup_module(void)
{
    ib_unregister_client(&ddbfs_ib_client);
    kfree(global_info);
    
}

module_init(d_rdma_init_module);
module_exit(d_rdma_cleanup_module);