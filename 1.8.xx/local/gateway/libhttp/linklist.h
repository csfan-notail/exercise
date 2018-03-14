#ifndef LINKLIST_HEAD 
#define LINKLIST_HEAD 
typedef struct  node
{
	void *node_data;  /*save data pointer*/
	struct node *next;/*下一个节点 */
	struct node *prior; /*上一个节点 */
}NODE;


typedef  struct linklist
{
	NODE *head; /*头节点 */
	NODE *rearptr;/*尾节点 */
	NODE *curptr;/*当前节点*/ 
	int  count;/*节点总数 */
}LINKLIST;
#endif


void link_init(LINKLIST *linklist);/*当创建完链表后，都要用该函数对链表进行一下初始化，不然使用起来会照成段错误*/
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

