#ifndef _MY_GLOBAL_H_
#define _MY_GLOBAL_H_

#include <rte_config.h>
#include <rte_ether.h>
#include "protocol.h"
#include "cJSON.h"


enum sz_protocol_type{
    SZ_PROTO_UKNOW = 0,
    SZ_PROTO_ARP,
    SZ_PROTO_IPV4,
    SZ_PROTO_IPV6
};

typedef struct sz_result_t{
    enum sz_protocol_type next_proto;
    unsigned short tx_ports_num;
    unsigned short portids[RTE_MAX_ETHPORTS];
}sz_result;

typedef struct sz_port_ip_info_t
{
    enum sz_protocol_type proto;
    union{
        unsigned char ipv4[4];
        unsigned char ipv6[16];
        }ip;
}sz_port_ip_info;
typedef struct sz_port_info_t
{
    struct ether_addr mac_addr;
    sz_port_ip_info ip_info;
}sz_port_info;

typedef struct sz_global_config_t
{
    sz_result result;
    char is_little_endian;
    unsigned short cur_portid;
    unsigned short ports_num;
    unsigned short port_index[RTE_MAX_ETHPORTS];
    sz_port_info port_info[RTE_MAX_ETHPORTS];
}sz_global_config;

typedef struct sz_pkt_data_t{
	char *addr;
	unsigned int len;
}sz_pkt_data;

typedef int (*pkt_data_init_callback)(void *ext_args);
typedef int (*pkt_data_handle_callback)(sz_pkt_data *pkt_data, void *ext_args);
typedef int (*pkt_data_destory_callback)(void *ext_args);

typedef struct sz_pkt_data_callback_t{
    pkt_data_init_callback pkt_data_init;
    pkt_data_handle_callback pkt_data_handle;
    pkt_data_destory_callback pkt_data_destory;
}sz_pkt_data_callback;

typedef struct sz_framework_tree_node_t{
	struct sz_framework_tree_node_t *son_node;
	struct sz_framework_tree_node_t *next_node;
	struct sz_framework_tree_node_t *list_next;
	char name[64];
	char reload_config;
	unsigned long long time_usec;
	sz_pkt_data_callback pkt_data_cb;
}sz_framework_tree_node;

extern sz_framework_tree_node *g_ft_root;
extern sz_framework_tree_node *g_fl_list;

extern sz_global_config g_config;

int sz_framework_list_add_tail(sz_framework_tree_node **fl_head, sz_framework_tree_node *list_node);
sz_framework_tree_node *sz_framework_list_find_by_name(sz_framework_tree_node *fl_head, const char *name);
int sz_framework_tree_add_node(sz_framework_tree_node **ft_root, sz_framework_tree_node *ft_node);

int sz_framework_tree_init(sz_framework_tree_node *ft_root, void *ext_args);
int sz_framework_tree_son_handle(sz_framework_tree_node *ft_root, sz_pkt_data *pkt_data, void *ext_args);
int sz_framework_tree_destory(sz_framework_tree_node *ft_root, void *ext_args);

char sz_is_little_endian(void);

void sz_json_config_loader(const char *filename, int (*prase_json_cb)(cJSON *node));

#define SZ_REGISTER_FUNC_PRIO(func, prio) \
static void __attribute__((constructor(prio), used)) func(void)



#endif