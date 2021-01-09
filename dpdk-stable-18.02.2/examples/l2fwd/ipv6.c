#include <stdio.h>
#include "global.h"


static int ipv6_init(void *ext_args)
{
    sz_global_config *cfg = (sz_global_config *)ext_args;
    printf("ipv6_init %d\n", cfg->ports_num);
    return 0;
}
static int ipv6_handle(sz_pkt_data *pkt_data, void *ext_args)
{
    sz_global_config *cfg = (sz_global_config *)ext_args;
    if(SZ_PROTO_IPV6 != cfg->result.next_proto)
    {
        return 1;
    }

    cfg->result.tx_ports_num = 1;
    cfg->result.portids[0] = 0 == cfg->cur_portid ? 1 : 0;
    printf("ipv6_handle cur_portid:%d tx_ports_num:%d portid:%d\n", cfg->cur_portid, cfg->result.tx_ports_num, cfg->result.portids[0]);
    return 0;

}
static int ipv6_destory(void *ext_args)
{
    sz_global_config *cfg = (sz_global_config *)ext_args;
    printf("ipv6_destory %d\n", cfg->ports_num);
    return 0;
}


static sz_framework_tree_node ipv6_cb = {
    .son_node = NULL,
    .next_node = NULL,
    .list_next = NULL,
    .name = "ipv6",
    .reload_config = 0,
    .time_usec = 0,
    .pkt_data_cb = {
        .pkt_data_init = ipv6_init,
        .pkt_data_handle = ipv6_handle,
        .pkt_data_destory = ipv6_destory,
        },
};
static void ipv6_constructor(void)
{
    sz_framework_list_add_tail(&g_fl_list, &ipv6_cb);
}

SZ_REGISTER_FUNC_PRIO(ipv6_constructor, 113);

