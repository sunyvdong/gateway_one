#include <stdio.h>
#include "global.h"


static int tcp_init(void *ext_args)
{
    sz_global_config *cfg = (sz_global_config *)ext_args;
    printf("tcp_init %d\n", cfg->ports_num);
    return 0;
}
static int tcp_handle(sz_pkt_data *pkt_data, void *ext_args)
{
    sz_global_config *cfg = (sz_global_config *)ext_args;

    cfg->result.tx_ports_num = 1;
    cfg->result.portids[0] = 0 == cfg->cur_portid ? 1 : 0;
    printf("tcp_handle cur_portid:%d tx_ports_num:%d portid:%d\n", cfg->cur_portid, cfg->result.tx_ports_num, cfg->result.portids[0]);
    return 0;

}
static int tcp_destory(void *ext_args)
{
    sz_global_config *cfg = (sz_global_config *)ext_args;
    printf("tcp_destory %d\n", cfg->ports_num);
    return 0;
}


static sz_framework_tree_node tcp_cb = {
    .son_node = NULL,
    .next_node = NULL,
    .list_next = NULL,
    .name = "tcp",
    .reload_config = 0,
    .time_usec = 0,
    .pkt_data_cb = {
        .pkt_data_init = tcp_init,
        .pkt_data_handle = tcp_handle,
        .pkt_data_destory = tcp_destory,
        },
};
static void tcp_constructor(void)
{
    sz_framework_list_add_tail(&g_fl_list, &tcp_cb);
}

SZ_REGISTER_FUNC_PRIO(tcp_constructor, 113);

