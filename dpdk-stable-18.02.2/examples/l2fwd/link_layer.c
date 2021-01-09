#include <stdio.h>
#include "global.h"

#define SZ_LINK_FIXED_LEN 14

static sz_framework_tree_node link_layer_cb;


static int link_layer_init(void *ext_args)
{
    sz_global_config *cfg = (sz_global_config *)ext_args;
    printf("link_layer_init %d\n", cfg->ports_num);
    return 0;
}
static int link_layer_handle(sz_pkt_data *pkt_data, void *ext_args)
{
    sz_global_config *cfg = (sz_global_config *)ext_args;
    if(pkt_data->len <= sizeof(sz_link_header_info))
    {
        printf("link_layer_handle drop len:%d\n", pkt_data->len);
        return 1;
    }
    sz_link_header_info *link_header = (sz_link_header_info *)pkt_data->addr;
    unsigned short type = link_header->type_high;
    type <<= 8;
    type += link_header->type_low;
    switch (type)
    {
    case 0x86dd:
        cfg->result.next_proto = SZ_PROTO_IPV6;
        break;
    case 0x0800:
        cfg->result.next_proto = SZ_PROTO_IPV4;
        break;
    case 0x0806:
        cfg->result.next_proto = SZ_PROTO_ARP;
        break;
    default:
        cfg->result.next_proto = SZ_PROTO_UKNOW;
        return 1;
    }
    pkt_data->addr += SZ_LINK_FIXED_LEN;
    pkt_data->len -= SZ_LINK_FIXED_LEN;
    sz_framework_tree_son_handle(&link_layer_cb, pkt_data, ext_args);
    return 0;

}
static int link_layer_destory(void *ext_args)
{
    sz_global_config *cfg = (sz_global_config *)ext_args;
    printf("link_layer_destory %d\n", cfg->ports_num);
    return 0;
}


static sz_framework_tree_node link_layer_cb = {
    .son_node = NULL,
    .next_node = NULL,
    .list_next = NULL,
    .name = "l2handle",
    .reload_config = 0,
    .time_usec = 0,
    .pkt_data_cb = {
        .pkt_data_init = link_layer_init,
        .pkt_data_handle = link_layer_handle,
        .pkt_data_destory = link_layer_destory,
        },
};
static void link_layer_constructor(void)
{
    sz_framework_list_add_tail(&g_fl_list, &link_layer_cb);
}

SZ_REGISTER_FUNC_PRIO(link_layer_constructor, 113);

