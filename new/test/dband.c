
#include "se.h"

dctx *ddpfs_ctx;
struct ib_device *d_dev; //创设备
int ib_port = 1;

static void ddpfs_ib_add_one(struct ib_device *device)
{
    ddpfs_ctx= (dctx *)kmalloc(sizeof(dctx), GFP_KERNEL);
    d_dev = device;

    ddpfs_ctx->ctx_pd = ib_alloc_pd(device);
    if(!ddpfs_ctx->ctx_pd){
        p_err("could not allocate pd\n", 0);
    }
    return 0;
}

static void ddpfs_ib_remove_one(struct ib_device *device, void *client_data){
    return;
}

struct ib_client ddpfs_ib_client = {
    .name = "ddpfs_ib_client",
    .add = ddpfs_ib_add_one,
    .remove = ddpfs_ib_remove_one
};

int d_ib_query_port(struct ib_device *device, u8 port_num, struct ib_port_attr *port_attr)
{
    int c;
    c = ib_query_port(device, port_num, port_attr);
    if(c<0)
    {
        p_err("Fail to do query_port");
    }
    return c;
}




//创建mr
int ddpfs_setup_mr(dctx *ctx)
{
    int ret;
    struct scatterlist sg = {0};
    struct ib_send_wr *bad_wr;
    u8 key = 0;

    ib_update_fast_reg_key(ctx->m->rdma_mr, ++key);
    ctx->m->reg_wr.key = ctx->m->rdma_mr->rkey;

    ctx->m->rdma_buf = kzalloc(D_LOG_RDMA_REGION, GFP_KERNEL);
    ctx->m->rdma_dma_addr = dma_map_single(ctx->pd->device->dma_device, ctx->m->rdma_buf,D_LOG_RDMA_REGION, DMA_BIDIRECTIONAL);

    sg_dma_address(&sg) = ctx->m->rdma_dma_addr;
    sg_dma_len(&sg) = D_LOG_RDMA_REGION;

    p_pri("MAP %llx .. %llx", ctx->m-rdma_dma_addr, ctx->m->rdma_dma_addr + D_LOG_RDMA_REGION);
    ret = ib_map_mr_sg(ctx->m->rdma_mr, &sg, 1, NULL, PAGE_SIZE);
    if(ret != 1){
        p_err("ERROR map sg ret %d, sg.address %llx sg.len %u",ret,sg.dma_address, sg.dma_length);
        return -EINVAL;
    }
    p_pri("map sg ret %d, sg.address %llx sg.len %u\n", ret, sg.dma_address,sg.dma_length);
    p_pri("reg MR sg%d rkey 0x%x
     va %llx len %u page_size %u", ctx->m->reg_wr.wr.num_sge, ctx->m->reg_wr.key,
    ctx->m->rdma_mr->iova, ctx->m->rdma_mr->length, ctx->mi->rdma_mr->page_size);

    return ret;
}

int ddpfs_setup_local_memory(dctx *ctx){
    int ret;
    int i;
    struct ddpfs_mem_info *m;

    ctx->m = kzalloc(sizeof(struct ddpfs_mem_info), GFP_KERNEL);
    if(!ctx->m){
        return -ENOMEM;
    }
    return 0;
}

int setup_mem(dctx *ctx){
    int ret,i;
    struct ddpfa_mem_info *m;

    ctx->m = kzalloc(sizeof(struct ddpfs_mem_info), GFP_KERNEL)
    if(!ctx->m){
        return -ENOMEM;
    }
    m->recv_pages = alloc_pages(GFP_KERNEL,9);
    if(!m->recv_pages){
        p_err("Fail to allocate pages for recv buf",0);
        return -ENOMEM;
    }
    m->recv_buf = page_address(m->recv_pages);

    m->recv_dma_addr = dma_map_single(ctx->pd->device->dma_device,m->recv_buf, 
    D_RECV_ENTRY_SIZE * CQLENTH, DMA_BIDIRECTIONAL);
    p_pri("recv_buf vaddr %p ,dma %llx---%llx",ctx->recv_buf ,ctx->m->recv_dma_addr, ctx->m->recv_dma_addr+D_RECV_ENTRY_SIZE * CQLENTH);

  // 	dnova_post_recv_req(ctx, 0, D_RECV_ENTRY_SIZE, CQLENTH);
    m->rdma_mr = ib_alloc_mr(ctx->pd, IB_MR_TYPE_MEM_REG, 4);
    m->reg_wr.wr_opcode = IB_WR_REG_MR;
    m->reg_wr.mr = m->rdma_mr;
    m->reg_wr.key = m->rdma_mr->rkey;
    m->reg_wr.access = IB_ACCRESS_REMOTE_WRITE | IB_ACCESS_LOCAL_WRITE;
    if(!m->rdma_mr)
}

//建立监听相关
int est_listen (dctx *ctx)
{
    int  ret;
    ctx->listen_cm_id = rdma_create_id(&init_net,)
}

//创建pq
int ddpfs_create_id(dctx *ctx)
{
    int ret;
    ctx->listen_cm_id = rdma_create_id(&init_net, ddpfs_cma_handler, ctx, RDMA_PS_TCP, IB_QPT_RC);
    if(IS_ERR(ctx->listen_cm_id)){
        p_err("error to rdma_create_id");
        return -EINVAL;
    }
return 0;
}

int ddpfs_cma_handler(struct rdma_cm_id *id, struct rdma_cm_event *event)
{
    int ret;
    dctx *ctx = id->context;

    p_pri("cma_event type %d id %p (%s)", event->event, id, (id==ctx->listen_cm_id) ? "self" : "connect");

    switch (event->event){
        case RDMA_CM_EVENT_ADDR_RESOLVED:
            ctx->state = ADDR_RESOLVED;
            ret = rdma_resolve_route(id, 2000);
            if(ret){
                p_err("rdma_resolve_route error");
        	    wake_up_interruptible(&ctx->ctl_wq);
		    }
		    break;

	    case RDMA_CM_EVENT_ROUTE_RESOLVED:
		    ctx->state = ROUTE_RESOLVED;
		    wake_up_interruptible(&ctx->ctl_wq);
		    break; 
        case RDMA_CM_EVENT_CONNECT_REQUEST:
		    ctx->state = CONNECT_REQUEST;
		    ctx->peer_cm_id = id;
		    p_pri("CONNECT_REQUEST %p", ctx->peer_cm_id);
            wake_up_interruptible(&ctx->ctl_wq);
	    	break;

	    case RDMA_CM_EVENT_ESTABLISHED:
		    p_pri("ESTABLISHED");
		    ctx->state = CONNECTED;
		    wake_up_interruptible(&ctx->ctl_wq);
		    break;

	    case RDMA_CM_EVENT_ADDR_ERROR:
	    case RDMA_CM_EVENT_ROUTE_ERROR:
	    case RDMA_CM_EVENT_CONNECT_ERROR:
	    case RDMA_CM_EVENT_UNREACHABLE:
	    case RDMA_CM_EVENT_REJECTED:
		    p_pri("RDMA_CM_EVENT_REJECTED, event %d, previous status %d",
						 event->event, event->status);
		    ctx->state = ERROR_STATE;
		    wake_up_interruptible(&ctx->ctl_wq);
		    break;
	    case RDMA_CM_EVENT_TIMEWAIT_EXIT:
	    case RDMA_CM_EVENT_DISCONNECTED:

		    ctx->state = DISCONNECT;
		    wake_up_interruptible(&ctx->ctl_wq);
		    break;

	    case RDMA_CM_EVENT_DEVICE_REMOVAL:
		    p_pri("DEVICE_REMOVAL");
		    break;

	    default:
		    p_pri("Unexpected RDMA CM event %d!", event->event);
		    wake_up_interruptible(&ctx->ctl_wq);
		    break;
	}
	return 0;
}

int ddpfs_rdma_listen_init(dctx * ctx)
{
    struct sockaddr_in sin;
    int ret;
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = (u32)hton1(INADDR_ANY);
    sin.sin_port = (u16)htons(RDS_PORT);

    ret = rdma_bind_addr(ctx->listen_cm_id, (struct sockaddr *)&sin);
    if(ret){
        p_err("failed to setup listener");
        goto out;
    }
    ret = rdma_listen(ctx->listen_cm_id, 3);
    if(ret){
        p_err("failed to setup listener");
        goto out;
    }
    wait_event_interruptible(ctx->ctl_wq, ctx->state >= CONNECT_REQUEST);
	if (ctx->state != CONNECT_REQUEST) {
		p_err("expect CONNECT_REQUEST, got %d", ctx->state);
		return -1;
	}
    p_pri("cm %p listening on port %u\n",ctx->listen_cm_id,RDS_PORT);
out:
    if(ctx->listen_cm_id)
        rdma_destory_id(ctx->listen_cm_id);
    return ret;
}
static void ddpfs_ib_cq_event_handler(struct ib_event *event, void *data)
{

}


int ddpfs_create_qp(dctx *ctx)
{

}

//创建cq
static void d_ib_cq_event_handler(struct ib_evernt *event, void *data)
{
    p_pri("event %d(%s) data %p", event->event,ib_event_msg(event->event), data);
}