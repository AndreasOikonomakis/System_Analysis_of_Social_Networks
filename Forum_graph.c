#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"
#include "GraphLib.h"
#include "Forum_graph.h"
#include "hashing.h"
#include "graph.h"

GraphList *Create_Graph_List(){

	GraphList *gl;
	gl = malloc(sizeof(GraphList));
	gl->size =0;
	gl->max_size = 3;
	gl->Graph_Array = malloc(sizeof(Graph)*gl->max_size);

	return gl;
}



void InsertGraphList(struct GraphList *list,struct Graph *g){

	Graph **gl;
	(list->size)++;
	if(list->size > list->max_size){
		
		list->max_size = 2*(list->max_size);
		gl = realloc(list->Graph_Array,sizeof(struct Graph*)*list->max_size);
		list->Graph_Array = gl;
		list->Graph_Array[list->size -1] = g;
	}
	else{		
		list->Graph_Array[list->size -1] = g;
	}

}


//Lista pou krataei ta top forums
NForums *Create_NForums(int size, int *cur_forum_num,int forum_size){
	NForums *NF;
	NF = malloc(sizeof(NForums));	
	NF->max_size = size;
	NF->cur_size =0;
	NF->head = NULL;
	NF->tail = NULL;
	NF->max_members = 0;
	NF->min_members = 0;
	NF->cur_forum_num = cur_forum_num;
	NF->forum_size = forum_size;
	return NF;
}

void Insert_NForums(NForums *NF,struct FNode *fn){//eisagwgh sthn lista twn top forums

	NF_Node *n,*tmp,*cur;
	n = malloc(sizeof(NF_Node));
	n->FNptr = fn;
	n->next = NULL;
	n->prev = NULL;	
	//prwto head
   if(NF->head == NULL){
      NF->head = n;    
		NF->cur_size++;		
		NF->max_members = n->FNptr->Forum_Members->size;
		return;
   }
   else if (fn->Forum_Members->size > NF->head->FNptr->Forum_Members->size){
 		//Neo head
		if(NF->max_size==1){
			tmp = NF->head;
      	NF->head = n;
			NF->cur_size++;	
			return;

		}
      tmp = NF->head;
      NF->head = n;
      n->next = tmp;
		tmp->prev = n;
		NF->cur_size++;
		NF->max_members = n->FNptr->Forum_Members->size; 
		return; 
  }
   else{
		//prwto tail
 		if(NF->tail == NULL){
			//printf("to %d einai to prwto tail\n",n->FNptr->Forum_Members->size);
			NF->tail = n;
			NF->head->next = n;
			n->prev = NF->head;
			NF->cur_size++;
			NF->min_members = n->FNptr->Forum_Members->size; 
			return;
		}

     cur = NF->head;
        
      while (cur->next != NULL && fn->Forum_Members->size < cur->next->FNptr->Forum_Members->size)
          cur = cur->next;

		//mpainei sto telos
		if(cur->next == NULL){
			tmp = NF->tail;
			tmp->next = n;
			n->prev = tmp;
			NF->tail = n;
			NF->cur_size++;
			NF->min_members = n->FNptr->Forum_Members->size;
			//printf("Mpainei to %d ws kainourgio tail\n",n->FNptr->Forum_Members->size);
			return;
		}

		//printf("To %d 8a mpei prin to %d\n",n->FNptr->Forum_Members->size,cur->FNptr->Forum_Members->size);
		
		//endiamesa
		tmp = cur->next;
		cur->next = n;
		n->next = tmp;
		n->prev = cur;
		tmp->prev = n;
		NF->cur_size++;
	 
	
    }

}

void Fix_NF(NForums *NF,struct FNode *fn){//sunarthsh pou vazei forums sthn lista twn top forums

	NF_Node *n,*tmp,*cur;
	n = malloc(sizeof(NF_Node));
	n->FNptr = fn;
	n->next = NULL;
	n->prev = NULL;

	if(fn->Forum_Members->size > NF->head->FNptr->Forum_Members->size){
 		//Neo head
      tmp = NF->head;
      NF->head = n;
      n->next = tmp;
		tmp->prev = n;
		NF->max_members = n->FNptr->Forum_Members->size; 
 		tmp = NF->tail;
		NF->tail = tmp->prev;
		NF->tail->next = NULL;
		NF->min_members = NF->tail->FNptr->Forum_Members->size; 
		free(tmp); 
		return;
 	}

    cur = NF->head;
        
    while (cur->next != NULL && fn->Forum_Members->size < cur->next->FNptr->Forum_Members->size)
        cur = cur->next;

		//mpainei sto telos
		if(cur->next == NULL){
			tmp = NF->tail;
			NF->tail = n;
			NF->tail->next = NULL;
			NF->min_members = n->FNptr->Forum_Members->size;
			//printf("-->Mpainei to %d ws kainourgio tail\n",n->FNptr->Forum_Members->size);
			free(tmp);
			return;
		}

		//printf("-->To %d 8a mpei prin to %d\n",n->FNptr->Forum_Members->size,cur->FNptr->Forum_Members->size);
		
		//endiamesa
		tmp = cur->next;
		cur->next = n;
		n->next = tmp;
		n->prev = cur;
		tmp->prev = n;

		tmp = NF->tail;
		NF->tail = tmp->prev;
		NF->tail->next = NULL;
		NF->min_members = NF->tail->FNptr->Forum_Members->size;

		free(tmp);


}


Graph *Create_Forum_Graph(int m,int c,FNode *forum){//Dhmiourgia grafou forums

	int i,j=0;
	Edge *new_e,*cur_e;
	Node *cur_member,*input_node,*n;
	TNode *is_member;
	Graph *g;
	g = createGraph(m,c);
	edge_bet_counter *ebc;
	TNode tn;
	g->edge_hash = Create_T_Hash_table(m,c);




	strcpy(g->Forum_Name,forum->name);
	g->Forum_ID = forum->id;


	//printf("To forum %s exei %d members\n",forum->name,forum->Forum_Members->size);
	for(i=0;i<forum->Forum_Members->size;i++){					
		cur_member = forum->Forum_Members->GNodeArray[i];
				
		input_node =  createNode(cur_member->id,NULL);					

		insertNode(input_node,g);			

		//printf("vazw ton %d me ins_num %d\n",input_node->id,input_node->ins_num);
	}


	Node *endNode;
	for(i=0;i<forum->Forum_Members->size;i++){
					
		cur_member = forum->Forum_Members->GNodeArray[i];
		if(cur_member->E_List!=NULL){
			
			n = LookupNode(g,cur_member->id);
			Initialize_E_List(&n->E_List,n->id);

			//printf("E_List forum %d tou %d\n",forum->id,n->id);

			cur_e = cur_member->E_List->head;
		
			for(j=0;j<cur_member->E_List->size;j++){
			
				is_member = T_Search(forum->likes_to_from,cur_e->n_node->id);
				if(is_member!=NULL){

					endNode = LookupNode(g,is_member->id);
					new_e = setEdgeProperties(n->id,cur_e->n_node->id,"knows",0);
					new_e->EdgeID = Fix_EdgeID(n->ins_num,endNode->ins_num);					
					insertEdge(n->id,new_e, g);						

					if(T_Search(g->edge_hash,new_e->EdgeID) == NULL){
						tn.id = new_e->EdgeID;
						ebc = malloc(sizeof(edge_bet_counter));
						ebc->e_ptr = new_e;
						ebc->state = -1;
						ebc->bet_num = 0;
						ebc->edge_betweenness = 0;
						tn.data = ebc;
						T_InsertNode(&tn,g->edge_hash);
					}





					//printf("mphke h akmh me id %d\n",new_e->EdgeID);
				}

				cur_e = cur_e->next;
			}

		}

	}

	Node *repr;
	CGLNode *cur;
	cur = g->CGL->head;
	
	while(cur!=NULL){
		repr = LookupNode(g,cur->Repr_node_id);
		repr->Graph_ID = repr->id;	
		IncreaseNodesofCG(g->CGL,repr->id,repr);
		Fix_graph_Members(g,cur->Repr_node_id,repr);

		cur = cur->next;
	}




	return g;

}
