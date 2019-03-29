#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <linux/kernel.h>
#include "config.h"


void printk_coninfo(struct coninfo scinfo)
{
    if(scinfo.is_server){
        printk("IS_Server");
    }
    else{
        printk("Is_Client");
    }
    printk("msg_size=%d", scinfo.msg_size);
    printk("num_concurr_msgs=%d", scinfo.num_concurr_msgs);
    printk("sock_port=%d", scinfo.sock_port_num);
    if(scinfo.is_server==false)
    printk("server_name=%s", scinfo.server_name);
}
