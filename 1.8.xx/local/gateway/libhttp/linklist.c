    #include <string.h>  
    #include <stdlib.h>  
    #include "linklist.h"  
      
    //ע���ҵ����������±�1��ʼ��  
      
    //��ʼ������ֻ��Ϊ����ṹ�����ռ䣬���ѳ�Աָ��Ϊ��  
    void  link_init(LINKLIST *linklist)  
    {  
        linklist->head=NULL;  
        linklist->rearptr=NULL;  
        linklist->curptr=NULL; // ��ǰ���  
        linklist->count=0;  
    }  
      
    //��������β��ʱֻ���ж��Ƿ��ǿ�����  
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
            linklist->head=node;  //��ͷ���ָ��node  
            linklist->rearptr=node;//��β���ָ��node  
        }  
        linklist->curptr=node;//��ǰ���ָ���´����Ľ��  
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
            linklist->head=node;  //��ͷ���ָ��node  
            linklist->rearptr=node;//��β���ָ��node  
        }  
        linklist->curptr=node;//��ǰ���ָ���´����Ľ��  
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
        node->prior=NULL; //ǰ��  
        node->next=NULL; //����  
        return node;  
    }  
    /*�����±���������У�����ȡ���±���Ȼ�󱣴�������Ȼ��Ըý��next�ֶθ����¼����� 
        �¼�����prior�ֶθ�Ϊ���������Ľ�㡣 
    */  
    void insert_link_index(LINKLIST *linklist,int index,void *data)  
    {  
        if(index==1) //����ͷ���λ�ò���һ���½��  
        {  
            insert_front_link(linklist,data);  
        }  
        else if(index==linklist->count)  //����β���λ�ò���һ���½��ʱ   
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
                linklist->head=node;  //��ͷ���ָ��node  
                linklist->rearptr=node;//��β���ָ��node  
            }  
            linklist->curptr=node;//��ǰ���ָ���´����Ľ��  
            linklist->count++;  
        }  
        else  
        {  
              
            NODE * node = create_node(data); // ����һ��Ҫ������½ڵ�  
            /* 
            int i=1; 
            NODE * pNode=linklist->head;//��ͷ�ڵ�ָ�뱣���һ����ʱָ���� 
            while(i<index) 
            { 
                pNode=pNode->next; 
                i++; 
            } 
            */  
            NODE * pNode=get_node_by_index(linklist,index);//��ȡҪ����λ�õĽڵ�ָ��  
            if(pNode!=NULL)  
            {  
                pNode->prior->next=node;  
                node->prior=pNode->prior;  
                node->next=pNode;  
                pNode->prior=node;  
                linklist->curptr=node;//��ǰ���ָ���´����Ľ��  
                linklist->count++;//��������һ  
            }  
        }  
      
    }  
      
    //��ȡָ������ֵ�Ľڵ�  
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
      
    //ɾ��ָ���ڵ�   
    void del_link_index(LINKLIST *linklist,int index)  
    {  
        //ɾ��ͷ�ڵ�   
        if(index==1)  
        {  
            del_head_link(linklist);  
        }  
        else if(index==linklist->count)//ɾ��β�ڵ�   
        {  
            del_rear_link(linklist);  
        }  
        else  
        {  
            NODE * pNode=get_node_by_index(linklist,index);//��ȡҪɾ��λ�õĽڵ�ָ��  
            if(pNode!=NULL)  
            {  
                pNode->prior->next=pNode->next;  
                pNode->next->prior=pNode->prior;  
                free_node(pNode); //ɾ��ָ���ڵ�   
                linklist->count--; //�ڵ�������һ  
            }  
        }  
        //���ɾ����ڵ���û�нڵ㣬��ôҪ�ٰ������б����   
        if(linklist->count==0)  
        {  
            link_init(linklist);  
        }  
    }   
      
    //ɾ�����нڵ�  
    void free_all_node(LINKLIST *linklist)  
    {  
        NODE *pNODE; //��ʱָ�����������һ���ڵ�  
        while(linklist->head!=NULL)  
        {  
            pNODE=linklist->head->next;//����һ�½ڵ㱣������  
            //free(linklist->head);//ɾ��ͷ�ڵ�  
            free_node(linklist->head);//��ͷ�ڵ�  
      
            linklist->head=pNODE;//�ٰ�ͷ�ڵ�ָ����һ���ڵ�  
        }  
        link_init(linklist);  
    }  
      
    //ɾ������  
    void  free_list(LINKLIST *linklist)  
    {  
        free_all_node(linklist);  
        link_init(linklist);  
    }  
      
    //ɾ��β�ڵ�  
    void  del_rear_link(LINKLIST *linklist)  
    {  
        if(linklist->rearptr!=NULL)  
        {  
            NODE * pNode=linklist->rearptr->prior; //����β�ڵ��ǰ���ڵ�   
            pNode->next=NULL;//��β�ڵ��ǰ���ڵ��е�next��ΪNULL   
            //free(linklist->rearptr);  
            free_node(linklist->rearptr); //ɾ��β�ڵ�   
            linklist->rearptr=pNode; //��βָ��ָ��β�ڵ��ǰ���ڵ�   
            linklist->count--;//�ڵ�ĸ�����һ   
        }  
        //���ɾβ�ڵ���û�нڵ㣬��ôҪ�ٰ������б����   
        if(linklist->count==0)  
        {  
            link_init(linklist);  
        }  
    }  
      
    //ɾ��ͷ�ڵ�   
    void del_head_link(LINKLIST *linklist)  
    {  
        if(linklist->head!=NULL)  
        {   
            //���ͷ�ڵ����β�ڵ㼴ֻ��һ���ڵ��ʱ���ֱ��ɾ��   
            if(linklist->head==linklist->rearptr)   
            {   
                free_node(linklist->head);   
            }   
            else  
            {   
                NODE * pNode=linklist->head->next; //����ͷ�ڵ�ĺ����ڵ�   
                pNode->prior=NULL;//��ͷ�ڵ�ĺ����ڵ��е�prior��ΪNULL   
                //free(linklist->rearptr);  
                free_node(linklist->head); //ɾ��ͷ�ڵ�   
                linklist->head=pNode; //��βָ��ָ��β�ڵ��ǰ���ڵ�   
            }   
            linklist->count--;//�ڵ�ĸ�����һ  
        }  
        //���ɾβ�ڵ���û�нڵ㣬��ôҪ�ٰ������б����   
        if(linklist->count==0)  
        {  
            link_init(linklist);  
        }  
    }  
      
    //�ͷ�ָ���Ľڵ�  
    void free_node(NODE *node)  
    {  
        //node->prior->next=node->next;  
        //node->next->prior=node->prior;  
        if(node!=NULL)  
        {  
            if(node->node_data!=NULL)  
           {  
           	/* ���ﲻ��free */
            //free(node->node_data);  
           }  
           free(node);//�ͷŽڵ�  
        }  
    }  
