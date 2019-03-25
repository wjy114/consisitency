#include "se.h"
#include <linux/kthread.h>
#include <linux/slab.h>

#define S_C 1
#define SOCK_PORT_NUM 7657
#define MSG_SIZE 64
#define NUM_CONCURR_MSGS 1

struct coninfo *global_info;
EXPORT_SYMBOL(global_info);

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

    

}

static int __init d_rdma_cleanup_module(void)
{
    global_info = kzalloc(sizeof(struct coninfo))
}

module_init(d_rdma_init_module);
module_init(d_rdma_cleanup_module);