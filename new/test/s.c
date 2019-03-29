#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include<malloc.h>

#define S_C 1
#define SOCK_PORT_NUM 7657
#define MSG_SIZE 64
#define NUM_CONCURR_MSGS 1

struct coninfo *global_info;
EXPORT_SYMBOL(global_info);

struct coninfo{
    bool is_server;
    int msg_size;
    int num_concurr_msgs;
    int sock_port_num;
    char *sock_port;
    char server_name_char;
    char *server_name;
}__attribute__((aligned(64)));

int main(void)
{
    global_info =(struct coninfo *) malloc(sizeof(struct coninfo));
    if(S_C == 1){
        global_info->is_server = false;
        global_info->num_concurr_msgs = (int) NUM_CONCURR_MSGS;
        global_info->sock_port_num = (int) SOCK_PORT_NUM;
        global_info->sock_port = &global_info->sock_port_num;
        global_info->server_name = "192.168.99.11";
        global_info->msg_size = (int)MSG_SIZE;
    }
    else{
        global_info->sock_port_num = (int)SOCK_PORT_NUM;
        global_info->is_server = true;
        global_info->num_concurr_msgs = (int)NUM_CONCURR_MSGS;
        global_info->sock_port =  &(global_info->sock_port_num);
        global_info->msg_size = (int)MSG_SIZE;
    }
    add();
    printk_coninfo(*global_info);
}
int add(){
	printf("global:%d",global_info->sock_port_num);
	return 0;
}

void printk_coninfo(struct coninfo scinfo)
{
    if(scinfo.is_server){
        printf("IS_Server");
    }
    else{
        printf("Is_Client");
    }
    printf("msg_size=%d", scinfo.msg_size);
    printf("num_concurr_msgs=%d", scinfo.num_concurr_msgs);
    printf("sock_port=%s", scinfo.sock_port);
    if(scinfo.is_server==false)
    printf("server_name=%s", scinfo.server_name);
}

