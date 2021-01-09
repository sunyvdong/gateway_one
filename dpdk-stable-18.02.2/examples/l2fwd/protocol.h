#ifndef _MY_PROTOCOL_H_
#define _MY_PROTOCOL_H_

typedef struct sz_link_header_info_t
{
    unsigned char dst_mac[6];
    unsigned char src_mac[6];
    unsigned char type_high;
    unsigned char type_low;
}sz_link_header_info;
///////////////////////arp///////////////////////////////

typedef struct sz_arp_info_t
{
    unsigned char hw_type_high;
    unsigned char hw_type_low;

    unsigned char proto_type_high;
    unsigned char proto_type_low;
    
    unsigned char hardware_size;
    unsigned char protocol_size;

    unsigned char op_code_high;
    unsigned char op_code_low;

    unsigned char sender_mac[6];
    unsigned char sender_ip[4];
    unsigned char target_mac[6];
    unsigned char target_ip[4];
}sz_arp_info;


///////////////////////ipv4/////////////////////////////
#define SZ_ROUTER_MAX_ITEM_NUM         32
#define SZ_ROUTER_VAILD_INDEX_MASK     0x00ffffff
#define SZ_ROUTER_VAILD_MASKLEN_MASK   0xff000000



typedef struct sz_ipv4_header_info_t
{
    unsigned int header_len:4;
    unsigned int version:4;
    unsigned int service_field:8;
    unsigned int total_high_len:8;
    unsigned int total_low_len:8;
    
    unsigned int identification_high:8;
    unsigned int identification_low:8;
    unsigned int flags_high:8;
    unsigned int flags_low:8;
    
    unsigned int time_to_live:8;
    unsigned int proto:8;
    unsigned int header_checknum_high:8;
    unsigned int header_checknum_low:8;

    unsigned char src_ip[4];
    unsigned char dst_ip[4];
}sz_ipv4_header_info;

typedef struct sz_router_table_one_t
{
    unsigned char match_ip[4];
    unsigned char match_mask[4];
    unsigned char is_match_dst;
    unsigned short port;
    unsigned char dst_ip[4];
}sz_router_table_one;


typedef struct sz_router_tables_t
{
    unsigned short total_num;
    unsigned int mask_len_index[SZ_ROUTER_MAX_ITEM_NUM];
    sz_router_table_one tables[SZ_ROUTER_MAX_ITEM_NUM];
}sz_router_tables;



#endif

