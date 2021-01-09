#include <stdio.h>
#include "global.h"


extern sz_router_tables g_route_tables;

static int arp_init(void *ext_args)
{
    sz_global_config *cfg = (sz_global_config *)ext_args;
    printf("arp_init %d\n", cfg->ports_num);
    return 0;
}
static int arp_handle(sz_pkt_data *pkt_data, void *ext_args)
{
    sz_global_config *cfg = (sz_global_config *)ext_args;
    if(SZ_PROTO_ARP != cfg->result.next_proto)
    {
        return 1;
    }

    unsigned short index = 0;
    unsigned int table_index = 0;
    sz_arp_info *arp_info = (sz_arp_info *)pkt_data->addr;
    for(index = 0; index < g_route_tables.total_num; ++index)
    {
        table_index = g_route_tables.mask_len_index[index] & SZ_ROUTER_VAILD_INDEX_MASK;
        if(0 == arp_info->op_code_high &&
            1 == arp_info->op_code_low)
        {
            if((arp_info->sender_ip[0] &  g_route_tables.tables[table_index].match_mask[0]) == (g_route_tables.tables[table_index].match_ip[0] &  g_route_tables.tables[table_index].match_mask[0]) &&
                (arp_info->sender_ip[1] &  g_route_tables.tables[table_index].match_mask[1]) == (g_route_tables.tables[table_index].match_ip[1] &  g_route_tables.tables[table_index].match_mask[1]) &&
                (arp_info->sender_ip[2] &  g_route_tables.tables[table_index].match_mask[2]) == (g_route_tables.tables[table_index].match_ip[2] &  g_route_tables.tables[table_index].match_mask[2]) &&
                (arp_info->sender_ip[3] &  g_route_tables.tables[table_index].match_mask[3]) == (g_route_tables.tables[table_index].match_ip[3] &  g_route_tables.tables[table_index].match_mask[3]))
            {
                if(cfg->cur_portid != g_route_tables.tables[table_index].port)
                {
                    cfg->result.portids[cfg->result.tx_ports_num] = g_route_tables.tables[table_index].port;
                    printf("arp_handle sender_ip:%d.%d.%d.%d cur_portid:%d tx_ports_num:%d portid:%d\n",
                        arp_info->sender_ip[0], arp_info->sender_ip[1], arp_info->sender_ip[2], arp_info->sender_ip[3], cfg->cur_portid, cfg->result.tx_ports_num, cfg->result.portids[cfg->result.tx_ports_num]);
                    ++cfg->result.tx_ports_num;
                    break;
                }
            }
        }
        else if(0 == arp_info->op_code_high &&
            2 == arp_info->op_code_low)
        {
            if((arp_info->target_ip[0] &  g_route_tables.tables[table_index].match_mask[0]) == (g_route_tables.tables[table_index].match_ip[0] &  g_route_tables.tables[table_index].match_mask[0]) &&
                (arp_info->target_ip[1] &  g_route_tables.tables[table_index].match_mask[1]) == (g_route_tables.tables[table_index].match_ip[1] &  g_route_tables.tables[table_index].match_mask[1]) &&
                (arp_info->target_ip[2] &  g_route_tables.tables[table_index].match_mask[2]) == (g_route_tables.tables[table_index].match_ip[2] &  g_route_tables.tables[table_index].match_mask[2]) &&
                (arp_info->target_ip[3] &  g_route_tables.tables[table_index].match_mask[3]) == (g_route_tables.tables[table_index].match_ip[3] &  g_route_tables.tables[table_index].match_mask[3]))
            {
                if(cfg->cur_portid != g_route_tables.tables[table_index].port)
                {
                    cfg->result.portids[cfg->result.tx_ports_num] = g_route_tables.tables[table_index].port;
                    printf("arp_handle target_ip:%d.%d.%d.%d cur_portid:%d tx_ports_num:%d portid:%d\n",
                        arp_info->target_ip[0], arp_info->target_ip[1], arp_info->target_ip[2], arp_info->target_ip[3], cfg->cur_portid, cfg->result.tx_ports_num, cfg->result.portids[cfg->result.tx_ports_num]);
                    ++cfg->result.tx_ports_num;
                    break;
                }
            }
        }
    }

    return 0;

}
static int arp_destory(void *ext_args)
{
    sz_global_config *cfg = (sz_global_config *)ext_args;
    printf("arp_destory %d\n", cfg->ports_num);
    return 0;
}


static sz_framework_tree_node arp_cb = {
    .son_node = NULL,
    .next_node = NULL,
    .list_next = NULL,
    .name = "arp",
    .reload_config = 0,
    .time_usec = 0,
    .pkt_data_cb = {
        .pkt_data_init = arp_init,
        .pkt_data_handle = arp_handle,
        .pkt_data_destory = arp_destory,
        },
};
static void arp_constructor(void)
{
    sz_framework_list_add_tail(&g_fl_list, &arp_cb);
}

SZ_REGISTER_FUNC_PRIO(arp_constructor, 113);

