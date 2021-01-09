#include "global.h"


int sz_framework_list_add_tail(sz_framework_tree_node **fl_head, sz_framework_tree_node *list_node)
{
    sz_framework_tree_node *tmp = *fl_head;
    if(NULL == tmp)
    {
        *fl_head = list_node;
        list_node->list_next = NULL;
        return 0;
    }

    while(NULL != tmp->list_next)
    {
        tmp = tmp->list_next;
    }

    tmp->list_next = list_node;
    return 0;
}

sz_framework_tree_node *sz_framework_list_find_by_name(sz_framework_tree_node *fl_head, const char *name)
{
    sz_framework_tree_node *node = fl_head;
    while(NULL != node)
    {
        if(0 == strcmp(node->name, name))
        {
            return node;
        }
        node = node->list_next;
    }
    return NULL;
}


int sz_framework_tree_add_node(sz_framework_tree_node **ft_root, sz_framework_tree_node *ft_node)
{
    sz_framework_tree_node *tmp = *ft_root;
    
    ft_node->son_node = NULL;
    ft_node->next_node = NULL;
    if(NULL == tmp)
    {
        *ft_root = ft_node;
        return 0;
    }

    if(NULL == tmp->son_node)
    {
        tmp->son_node = ft_node;
        return 0;
    }

    tmp = tmp->son_node;

    while(NULL != tmp->next_node)
    {
        tmp = tmp->next_node;
    }
    tmp->next_node = ft_node;
    return 0;
}

int sz_framework_tree_init(sz_framework_tree_node *ft_root, void *ext_args)
{
    sz_framework_tree_node *son = ft_root->son_node;
    ft_root->pkt_data_cb.pkt_data_init(ext_args);
    if(NULL != son)
    {
        do{
            sz_framework_tree_init(son, ext_args);
            son = son->next_node;
        }while(NULL != son);
    }
    return 0;
}
int sz_framework_tree_son_handle(sz_framework_tree_node *ft_root, sz_pkt_data *pkt_data, void *ext_args)
{
    sz_framework_tree_node *son = ft_root->son_node;
    if(NULL != son)
    {
        do{
            son->pkt_data_cb.pkt_data_handle(pkt_data, ext_args);
            son = son->next_node;
        }while(NULL != son);
    }
    return 0;
}
int sz_framework_tree_destory(sz_framework_tree_node *ft_root, void *ext_args)
{
    sz_framework_tree_node *son = ft_root->son_node;
    if(NULL != son)
    {
        do{
            sz_framework_tree_destory(son, ext_args);
            son = son->next_node;
        }while(NULL != son);
    }
    ft_root->pkt_data_cb.pkt_data_destory(ext_args);
    return 0;
}

char sz_is_little_endian(void)
{
    int test_num = 1;
    char *buf = (char *)&test_num;
    return buf[0];
}

void sz_json_config_loader(const char *filename, int (*prase_json_cb)(cJSON *node))
{
    FILE *f;
    long len;
    char *content;
    f=fopen(filename,"rb");
    fseek(f,0,SEEK_END);
    len=ftell(f);
    fseek(f,0,SEEK_SET);
    content=(char*)malloc(len+1);
    fread(content,1,len,f);
    fclose(f);

    cJSON *root=cJSON_Parse(content);
    prase_json_cb(root);
    free(content);
    return ;
}


