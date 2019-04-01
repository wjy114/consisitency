#ifndef __SE_H__
#define __SE_H__
#include <linux/wait.h>
#include <rdma/ib_verbs.h>
#include <linux/kfifo.h>
#include <linux/kthread.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/pci.h>
#include <linux/proc_fs.h>
#include <linux/pmem.h>
#include <rdma/rdma_cm.h>
#include <linux/semaphore.h>

typedef struct ddpfs_context dctx;

#define D_LOG_RDMA_REGION PAGE_SIZE * D_REG_PAGE_LEN
#define D_REG_PAGE_LEN 4
#define CQLENTH 2
#define D_RECV_ENTRY_SIZE 512
#define	INADDR_ANY		((unsigned long int) 0x00000000)
#define RDS_PORT 18733 //18464-18768
#define p_err(string,args...)\
    do{printk(KERN_ERR "[Error]ddpfs:%s:%d:" string "\n",__FUNCTION__, __LINE__, ##args);}while(0)
#define p_pri(string,args...)\
    do{printk(KERN_INFO "ddpfs:%s:%d:" string "\n", __FUNCTION__, __LINE__, ##args);}while(0)

struct ddpfs_context{
    struct ib_context *context;
    enum ddpfs_state state;
    struct ib_cq *recv_cq;
    struct ib_cq *send_cq;
    struct ib_pd *ctx_pd; //1
    struct ib_qp *qp;
    wait_queue_head_t ctl_wq;

    struct ib_port_attr *port_attr; //1
    //struct ib_mr *mr;
    struct ddpfs_mem_info *m;
    struct ib_wc recv_wr;
    struct ib_wc send_wc;

    struct rdma_cm_id *listen_cm_id;
    struct rdma_cm_id *peer_cm_id;//保存对方
    struct list_head list;
    union ddpfs_ops ops;
};

enum ddpfs_state{
    IDLE =1,
    CONNECT_REQUEST,
	ADDR_RESOLVED,
	ROUTE_RESOLVED,
	CONNECTED,
	READY,
    ERROR_STATE,
    DISCONNECT,
};

struct ddpfs_mem_info{
    struct ib_mr *rdma_mr;
    struct ib_reg_wr reg_wr;

    //struct ib_mr *frdma_mr;
    //struct ib_reg_wr freg_wr;
    u8 *rdma_buf;
    u64 rdma_dma_addr;

    struct page *recv_pages;
    u8 *recv_buf;
    u64 recv_dma_addr;
};

union ddpfs_ops {
    struct dnova_client_ops c;
};

struct dnova_client_ops{
    int (*send)(struct ddpfs_context *,struct)

}
