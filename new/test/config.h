#include <stdbool.h>
#include <inttypes.h>

struct coninfo{
    bool is_server;
    int msg_size;
    int num_concurr_msgs;
    int sock_port_num;
    char *sock_port;
    char *server_name;
}__attribute__((aligned(64)));

extern struct coninfo scinfo;

void printk_coninfo(struct coninfo scinfo);

