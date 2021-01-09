#include <stdio.h>
#include <stdlib.h>
#include "global.h"
#include <netinet/in.h>


static sz_framework_tree_node ipv4_cb;
sz_router_tables g_route_tables;

static int ipv4_init(void *ext_args)
{
    sz_global_config *cfg = (sz_global_config *)ext_args;
    printf("ipv4_init %d\n", cfg->ports_num);
    memset(&g_route_tables, 0, sizeof(g_route_tables));
    return 0;
}

static int ip_stoul(const char *ip, unsigned char *buf)
{
	int s;
	s = inet_pton(AF_INET, ip, buf);
	if(0 == s)
	{
		printf("error:ip_stoul ip:%s, Not in presentation format\n", ip);
		return 1;
	}
	else if(s < 0)
	{
		printf("error:ip_stoul ip:%s, inet_pton\n", ip);
		return 1;
	}

	return 0;
}


static int ipv4_config_json_prase(cJSON * node)
{
    cJSON * item = NULL;
    unsigned short index = 0, rindex = 0;
    unsigned short move_bits = 0;
    unsigned int mask_len = 0;
    node = node->child->child;
    while(NULL != node)
    {
        if(g_route_tables.total_num < SZ_ROUTER_MAX_ITEM_NUM)
        {
            index = g_route_tables.total_num;
            ++g_route_tables.total_num;

            item = cJSON_GetObjectItem(node, "match_ip");
            ip_stoul(item->valuestring, g_route_tables.tables[index].match_ip);
            item = cJSON_GetObjectItem(node, "match_mask");
            ip_stoul(item->valuestring, g_route_tables.tables[index].match_mask);
            item = cJSON_GetObjectItem(node, "is_match_dst");
            g_route_tables.tables[index].is_match_dst = atoi(item->valuestring);
            item = cJSON_GetObjectItem(node, "port");
            g_route_tables.tables[index].port = atoi(item->valuestring);
            item = cJSON_GetObjectItem(node, "dst_ip");
            ip_stoul(item->valuestring, g_route_tables.tables[index].dst_ip);

            for(move_bits = 0, mask_len = 0; move_bits < 32; ++move_bits)
            {
                if(g_route_tables.tables[index].match_mask[move_bits / 8] & (1 << (move_bits % 8)))
                {
                    ++mask_len;
                }
            }
            mask_len <<= 24;
            mask_len |= index;

            for(rindex = index; 0 != rindex; --rindex)
            {
                if((mask_len & SZ_ROUTER_VAILD_MASKLEN_MASK) >
                    (g_route_tables.mask_len_index[rindex - 1] & SZ_ROUTER_VAILD_MASKLEN_MASK))
                {
                    g_route_tables.mask_len_index[rindex] = g_route_tables.mask_len_index[rindex - 1];
                }
                else
                {
                    break;
                }
            }
            g_route_tables.mask_len_index[rindex] = mask_len;
        }
        else
        {
            break;
        }

        node = node->next;
    }
    return 0;
}

static int ipv4_handle(sz_pkt_data *pkt_data, void *ext_args)
{
    sz_global_config *cfg = (sz_global_config *)ext_args;
    if(ipv4_cb.reload_config)
    {
        ipv4_cb.reload_config = 0;
        sz_json_config_loader("./ipv4.json", ipv4_config_json_prase);
    }
    if(SZ_PROTO_IPV4 != cfg->result.next_proto)
    {
        return 1;
    }

    unsigned short index = 0;
    unsigned int table_index = 0;
    sz_ipv4_header_info *ipv4_header = (sz_ipv4_header_info *)pkt_data->addr;
    unsigned char *mac_addr = NULL;
    for(index = 0; index < g_route_tables.total_num; ++index)
    {
        table_index = g_route_tables.mask_len_index[index] & SZ_ROUTER_VAILD_INDEX_MASK;
        if(g_route_tables.tables[table_index].is_match_dst)
        {
            mac_addr = ipv4_header->dst_ip;
        }
        else
        {
            mac_addr = ipv4_header->src_ip;
        }
        if((mac_addr[0] &  g_route_tables.tables[table_index].match_mask[0]) == (g_route_tables.tables[table_index].match_ip[0] &  g_route_tables.tables[table_index].match_mask[0]) &&
            (mac_addr[1] &  g_route_tables.tables[table_index].match_mask[1]) == (g_route_tables.tables[table_index].match_ip[1] &  g_route_tables.tables[table_index].match_mask[1]) &&
            (mac_addr[2] &  g_route_tables.tables[table_index].match_mask[2]) == (g_route_tables.tables[table_index].match_ip[2] &  g_route_tables.tables[table_index].match_mask[2]) &&
            (mac_addr[3] &  g_route_tables.tables[table_index].match_mask[3]) == (g_route_tables.tables[table_index].match_ip[3] &  g_route_tables.tables[table_index].match_mask[3]))
        {
            if(cfg->cur_portid != g_route_tables.tables[table_index].port)
            {
                cfg->result.portids[cfg->result.tx_ports_num] = g_route_tables.tables[table_index].port;
                ++cfg->result.tx_ports_num;
                break;
            }
        }
    }

    for(index = 0; index < cfg->result.tx_ports_num; ++index)
    {
        printf("ipv4_handle is_dst:%d ip:%d.%d.%d.%d cur_portid:%d tx_ports_num:%d portid:%d\n",
            g_route_tables.tables[table_index].is_match_dst, mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], cfg->cur_portid, cfg->result.tx_ports_num, cfg->result.portids[index]);
    }
    return 0;
}
static int ipv4_destory(void *ext_args)
{
    sz_global_config *cfg = (sz_global_config *)ext_args;
    printf("ipv4_destory %d\n", cfg->ports_num);
    return 0;
}


static sz_framework_tree_node ipv4_cb = {
    .son_node = NULL,
    .next_node = NULL,
    .list_next = NULL,
    .name = "ipv4",
    .reload_config = 1,
    .time_usec = 0,
    .pkt_data_cb = {
        .pkt_data_init = ipv4_init,
        .pkt_data_handle = ipv4_handle,
        .pkt_data_destory = ipv4_destory,
        },
};
static void ipv4_constructor(void)
{
    sz_framework_list_add_tail(&g_fl_list, &ipv4_cb);
}

SZ_REGISTER_FUNC_PRIO(ipv4_constructor, 113);

