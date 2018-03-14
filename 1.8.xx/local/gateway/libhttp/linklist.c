    #include <string.h>  
    #include <stdlib.h>  
    #include "linklist.h"  
      
    //注意我的链表是以下标1开始的  
      
    //初始化链表只是为链表结构体分配空间，并把成员指针为空  
    void  link_init(LINKLIST *linklist)  
    {  
        linklist->head=NULL;  
        linklist->rearptr=NULL;  
        linklist->curptr=NULL; // 当前结点  
        linklist->count=0;  
    }  
      
    //插入链表尾部时只是判断是否是空链表  
    void insert_rear_link(LINKLIST *linklist,void *data)  
    {  
        NODE * node=create_node(data);  
        if(linklist->rearptr!=NULL)  
        {  
            linklist->rearptr->next=node;  
            node->prior=linklist->rearptr;  
            linklist->rearptr=node;  
        }  
        else  
        {  
            linklist->head=node;  //将头结点指向node  
            linklist->rearptr=node;//将尾结点指向node  
        }  
        linklist->curptr=node;//当前结点指向新创建的结点  
        linklist->count++;  
    }  
      
    void  insert_front_link(LINKLIST *linklist,void *data)  
    {  
        NODE * node=create_node(data);  
        if(linklist->head!=NULL)  
        {  
            linklist->head->prior=node;  
            node->next=linklist->head;  
            linklist->head=node;  
        }  
        else  
        {  
            linklist->head=node;  //将头结点指向node  
            linklist->rearptr=node;//将尾结点指向node  
        }  
        linklist->curptr=node;//当前结点指向新创建的结点  
        linklist->count++;  
    }  
      
      
    NODE *create_node(void *data)  
    {  
        NODE * node=(NODE*)malloc(sizeof(NODE));   
        if(data!=NULL)  
        {   
            node->node_data=data;   
        }else   
        {   
            node->node_data=NULL;   
        }  
        node->prior=NULL; //前驱  
        node->next=NULL; //后驱  
        return node;  
    }  
    /*根据下标插入链表中：首先取得下标结点然后保存起来，然后对该结点next字段改我新加入结点 
        新加入结点prior字段改为保存起来的结点。 
    */  
    void insert_link_index(LINKLIST *linklist,int index,void *data)  
    {  
        if(index==1) //即在头结点位置插入一个新结点  
        {  
            insert_front_link(linklist,data);  
        }  
        else if(index==linklist->count)  //当在尾结点位置插入一个新结点时   
        {  
            NODE * node=create_node(data);  
            if(linklist->rearptr!=NULL)  
            {  
                linklist->rearptr->prior->next=node;  
                node->prior=linklist->rearptr->prior;  
                node->next=linklist->rearptr;  
                linklist->rearptr->prior=node;  
            }  
            else  
            {  
                linklist->head=node;  //将头结点指向node  
                linklist->rearptr=node;//将尾结点指向node  
            }  
            linklist->curptr=node;//当前结点指向新创建的结点  
            linklist->count++;  
        }  
        else  
        {  
              
            NODE * node = create_node(data); // 创建一个要插入的新节点  
            /* 
            int i=1; 
            NODE * pNode=linklist->head;//把头节点指针保存进一个临时指针中 
            while(i<index) 
            { 
                pNode=pNode->next; 
                i++; 
            } 
            */  
            NODE * pNode=get_node_by_index(linklist,index);//获取要插入位置的节点指针  
            if(pNode!=NULL)  
            {  
                pNode->prior->next=node;  
                node->prior=pNode->prior;  
                node->next=pNode;  
                pNode->prior=node;  
                linklist->curptr=node;//当前结点指向新创建的结点  
                linklist->count++;//结点个数加一  
            }  
        }  
      
    }  
      
    //获取指定索引值的节点  
    NODE *get_node_by_index(LINKLIST *linklist,int index)  
    {  
        int i=1;  
        NODE * node=linklist->head;  
        if(index>0 && index <=linklist->count)  
        {  
            while(i<index)  
            {  
                node=node->next;  
                i++;  
            }  
            return node;  
        }  
        return NULL;  
      
    }  
      
    //删除指定节点   
    void del_link_index(LINKLIST *linklist,int index)  
    {  
        //删除头节点   
        if(index==1)  
        {  
            del_head_link(linklist);  
        }  
        else if(index==linklist->count)//删除尾节点   
        {  
            del_rear_link(linklist);  
        }  
        else  
        {  
            NODE * pNode=get_node_by_index(linklist,index);//获取要删除位置的节点指针  
            if(pNode!=NULL)  
            {  
                pNode->prior->next=pNode->next;  
                pNode->next->prior=pNode->prior;  
                free_node(pNode); //删除指定节点   
                linklist->count--; //节点总数减一  
            }  
        }  
        //如查删任意节点完没有节点，那么要再把链表列表清空   
        if(linklist->count==0)  
        {  
            link_init(linklist);  
        }  
    }   
      
    //删除所有节点  
    void free_all_node(LINKLIST *linklist)  
    {  
        NODE *pNODE; //临时指针用来存放下一个节点  
        while(linklist->head!=NULL)  
        {  
            pNODE=linklist->head->next;//将下一下节点保存起来  
            //free(linklist->head);//删掉头节点  
            free_node(linklist->head);//掉头节点  
      
            linklist->head=pNODE;//再把头节点指向下一个节点  
        }  
        link_init(linklist);  
    }  
      
    //删除链表  
    void  free_list(LINKLIST *linklist)  
    {  
        free_all_node(linklist);  
        link_init(linklist);  
    }  
      
    //删除尾节点  
    void  del_rear_link(LINKLIST *linklist)  
    {  
        if(linklist->rearptr!=NULL)  
        {  
            NODE * pNode=linklist->rearptr->prior; //保存尾节点的前驱节点   
            pNode->next=NULL;//把尾节点的前驱节点中的next设为NULL   
            //free(linklist->rearptr);  
            free_node(linklist->rearptr); //删除尾节点   
            linklist->rearptr=pNode; //将尾指针指向尾节点的前驱节点   
            linklist->count--;//节点的个数减一   
        }  
        //如查删尾节点完没有节点，那么要再把链表列表清空   
        if(linklist->count==0)  
        {  
            link_init(linklist);  
        }  
    }  
      
    //删除头节点   
    void del_head_link(LINKLIST *linklist)  
    {  
        if(linklist->head!=NULL)  
        {   
            //如果头节点等于尾节点即只有一个节点的时候就直接删掉   
            if(linklist->head==linklist->rearptr)   
            {   
                free_node(linklist->head);   
            }   
            else  
            {   
                NODE * pNode=linklist->head->next; //保存头节点的后驱节点   
                pNode->prior=NULL;//把头节点的后驱节点中的prior设为NULL   
                //free(linklist->rearptr);  
                free_node(linklist->head); //删除头节点   
                linklist->head=pNode; //将尾指针指向尾节点的前驱节点   
            }   
            linklist->count--;//节点的个数减一  
        }  
        //如查删尾节点完没有节点，那么要再把链表列表清空   
        if(linklist->count==0)  
        {  
            link_init(linklist);  
        }  
    }  
      
    //释放指定的节点  
    void free_node(NODE *node)  
    {  
        //node->prior->next=node->next;  
        //node->next->prior=node->prior;  
        if(node!=NULL)  
        {  
            if(node->node_data!=NULL)  
           {  
           	/* 这里不能free */
            //free(node->node_data);  
           }  
           free(node);//释放节点  
        }  
    }  
