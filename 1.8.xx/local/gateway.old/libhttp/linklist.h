#ifndef LINKLIST_HEAD 
#define LINKLIST_HEAD 
typedef struct  node
{
	void *node_data;  /*save data pointer*/
	struct node *next;/*��һ���ڵ� */
	struct node *prior; /*��һ���ڵ� */
}NODE;


typedef  struct linklist
{
	NODE *head; /*ͷ�ڵ� */
	NODE *rearptr;/*β�ڵ� */
	NODE *curptr;/*��ǰ�ڵ�*/ 
	int  count;/*�ڵ����� */
}LINKLIST;
#endif


void link_init(LINKLIST *linklist);/*������������󣬶�Ҫ�øú������������һ�³�ʼ������Ȼʹ���������ճɶδ���*/
void insert_link_index(LINKLIST *linklist,int index,void *data);
void insert_front_link(LINKLIST *linklist,void *data);
void insert_rear_link(LINKLIST *linklist,void *data);

void del_link_index(LINKLIST *linklist,int index);
void del_rear_link(LINKLIST *linklist);
void del_head_link(LINKLIST *linklist);
NODE *get_node_by_index(LINKLIST *linklist,int index);  
void free_all_node(LINKLIST *linklist);
void free_list(LINKLIST *linklist);
NODE *create_node(void *data);
void free_node(NODE *node);

