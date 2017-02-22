#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"
#include "GraphLib.h"
#include "graph.h"
#include "Statistics.h"
#include "GN_Algorithm.h"
#include "hashing.h"
#include "Community_Graph_Utilities.h"
#include "Clique_Utilities.h"

Engaged_Edges_List *EEL_Creation(){

	int m=2,c=3;
	Engaged_Edges_List *list;
	list = malloc(sizeof(Engaged_Edges_List));
	list->inserted = Create_T_Hash_table(m,c);
	list->cur_size = 0;
	list->max_size = 5;
	list->EEA = malloc(sizeof(struct edge_bet_counter*)*list->max_size);
	return list;

}

void EEL_Insert(Engaged_Edges_List *list,edge_bet_counter *ebc){

	edge_bet_counter **e;
	(list->cur_size)++;
	if(list->cur_size > list->max_size){
		
		list->max_size = 2*(list->max_size);
		e = realloc(list->EEA,sizeof(edge_bet_counter*)*list->max_size);
		list->EEA = e;
		list->EEA[list->cur_size -1] = ebc;
	}
	else{		
		list->EEA[list->cur_size -1] = ebc;
	}


}

void EEL_Destroy(Engaged_Edges_List *list){

	int i=0;
	free(list->EEA);
	
	for(i=0;i<list->inserted->info.hash_table_size;i++)
		free(list->inserted->bucket[i].t_node); 
	
	free(list->inserted->bucket);
	free(list->inserted);

	free(list);

}

void RemoveEdge(struct e_list *list,int key,int free_flag){

	int i=0;
	Edge *cur_e,*tmp;
	cur_e = list->head;
	for(i=0;i<list->size;i++){

		if(i==0 && cur_e->EdgeID == key){
			//printf("head\n");
			tmp = list->head;
			list->head = tmp->next;
			//list->head->prev = tmp->prev;
			if(free_flag == 1)
				free(tmp);
			list->size--;
			return;
		}			
		else if(i==list->size-1 && cur_e->EdgeID == key){

			//printf("end\n");
			tmp = list->tail;
			list->tail = tmp->prev;
			//list->tail->next = tmp->next;
			if(free_flag == 1)
				free(tmp);
			list->size--;
			return;
		}
		else if(cur_e->EdgeID == key){

			//printf("middle\n");
			tmp = cur_e;
			cur_e = tmp->prev;
			cur_e->next = tmp->next;
			tmp->next->prev = cur_e;
			if(free_flag == 1)
				free(tmp);
			list->size--;
			return;

		}

		cur_e = cur_e->next;
	}

	
}



void Compute_Edge_betweenness(Engaged_Edges_List *EEL,int total_s_path,int cur_state,Max_edge_cent_container *max_e_c_cont){

	int i=0;
	edge_bet_counter *cur_ebc;
	
		//printf("Edges engaged\n");
	
	for(i=0;i<EEL->cur_size;i++){
		cur_ebc = EEL->EEA[i];
		if(cur_ebc->state != cur_state){
			cur_ebc->edge_betweenness = 0;
			cur_ebc->state = cur_state;

		}

		
			//printf("[%d->%d]",cur_ebc->e_ptr->id,cur_ebc->e_ptr->end);
		cur_ebc->edge_betweenness = cur_ebc->edge_betweenness + (double)cur_ebc->bet_num/(double)total_s_path;
		if(max_e_c_cont->max_edge_betweenness < cur_ebc->edge_betweenness){
			max_e_c_cont->max_edge_betweenness = cur_ebc->edge_betweenness;
			max_e_c_cont->ebc = cur_ebc;

		}
		cur_ebc->bet_num = 0;
	}

}

void Edge_betweenness(Graph *g,int state){

	Max_edge_cent_container max_e_c_cont;
	max_e_c_cont.max_edge_betweenness = -1;
	int m =2,c1=3;	
	int i=0,j=0,k=0;	 
	int s_path;
	edge_bet_counter *ebc;
	TNode tn,*t;
	Engaged_Edges_List *EEL;
	Node *nodeStart,*nodeEnd;
	CGLNode *c;
	Stack *stack;	
  	Initialize_Stack(&stack);
	struct Cell *matched;
	matched = Create_T_Hash_table(m,c1);

	c = g->CGL->head;	
  	while(c!=NULL){

		for(i=0;i<c->node_num;i++){	
			nodeStart = c->CGmembers[i].Member;
			
			tn.id = nodeStart->id;
			T_InsertNode(&tn,matched);
			nodeStart->level =0;
			Insert_Levels(nodeStart,g);
			

			for(j=0;j<c->node_num;j++){
				nodeEnd = c->CGmembers[j].Member;
				
				if(T_Search(matched,nodeEnd->id)==NULL){
					s_path = ReachNode1(nodeStart->id,nodeEnd->id,g);
					nodeStart->level =0;
					
					EEL = EEL_Creation();
					e_find_All_possible_paths(EEL,stack,nodeStart,nodeEnd,s_path,g,g->edge_hash);
					Compute_Edge_betweenness(EEL,stack->total_s_path,state,&max_e_c_cont);

					stack->total_s_path=0;
					EEL_Destroy(EEL);

				}


			}


		}	
	
		c = c->next;
	}

	//printf("Edge %d-%d removed -->%d\n",max_e_c_cont.ebc->e_ptr->id,max_e_c_cont.ebc->e_ptr->end,max_e_c_cont.ebc->e_ptr->EdgeID);


	nodeStart = LookupNode(g,max_e_c_cont.ebc->e_ptr->id);
		
	nodeEnd = LookupNode(g,max_e_c_cont.ebc->e_ptr->end);
	RemoveEdge(nodeStart->E_List,max_e_c_cont.ebc->e_ptr->EdgeID,0);
	RemoveEdge(nodeEnd->E_List,max_e_c_cont.ebc->e_ptr->EdgeID,1);
	g->total_edges--;
	g->total_edges--;

	for(i=0;i<matched->info.hash_table_size;i++)
		free(matched->bucket[i].t_node); 
	
	free(matched->bucket);
	free(matched);


	free(stack);

}

void e_find_All_possible_paths(Engaged_Edges_List *EEL,Stack *s,Node *startNode,Node *endNode,int s_path,Graph *g,Cell *edge_hash){


	int i=0;
	int found =0;
	StackNode *c;
	Node *current = startNode;
	Edge *cur_e;
	TNode tn,*t,ins_tn;	
	edge_bet_counter *ebc;
	int edgeID;
	if(s_path==1){
		edgeID = Fix_EdgeID(startNode->ins_num,endNode->ins_num);	
		t = T_Search(g->edge_hash,edgeID);
		ebc = (edge_bet_counter*)t->data;
		ebc->bet_num++;
		//printf("[%d->%d]\n",ebc->e_ptr->id,ebc->e_ptr->end);
		if(T_Search(EEL->inserted,edgeID)==NULL){
			ins_tn.id = edgeID;
			T_InsertNode(&ins_tn,EEL->inserted);
			EEL_Insert(EEL,ebc);
		}

		s->total_s_path++;		
		return;	
	}




	if(s->size > s_path){
		//printf("kanw return gia to %d\n",startNode->id);
		return;
	}

	if(startNode->id == endNode->id && s->size == s_path){
		
		s->total_s_path++;
		c = s->head;
		
		edgeID = Fix_EdgeID(c->node->ins_num,endNode->ins_num);	
		

		t = T_Search(g->edge_hash,edgeID);
		ebc = (edge_bet_counter*)t->data;
		ebc->bet_num++;
		//printf("[%d->%d]",ebc->e_ptr->id,ebc->e_ptr->end);

		if(T_Search(EEL->inserted,edgeID)==NULL){
			ins_tn.id = edgeID;
			T_InsertNode(&ins_tn,EEL->inserted);
			EEL_Insert(EEL,ebc);
		}




		while(c!=NULL){
						
			if(c->down!=NULL){			
				edgeID = Fix_EdgeID(c->node->ins_num,c->down->node->ins_num);
				

				t = T_Search(g->edge_hash,edgeID);
				ebc = (edge_bet_counter*)t->data;
				ebc->bet_num++;
				//printf("[%d->%d]",ebc->e_ptr->id,ebc->e_ptr->end);
				if(T_Search(EEL->inserted,edgeID)==NULL){
					ins_tn.id = edgeID;
					T_InsertNode(&ins_tn,EEL->inserted);
					EEL_Insert(EEL,ebc);
				}		



			}	

			c = c->down;
		}


	}else  {
				current->visited = 1;
				//printf("vazw to %d\n",current->id);
				Insert_to_Stack(s,current);

				cur_e = current->E_List->head;

				for(i=0;i<current->E_List->size;i++){
					if(cur_e->n_node->visited == 0 && cur_e->n_node->level > current->level){						
						//printf("episkeptomai to %d me level %d\n",cur_e->n_node->id,cur_e->n_node->level);
						e_find_All_possible_paths(EEL,s,cur_e->n_node,endNode,s_path,g,edge_hash);
					}											
					cur_e = cur_e->next;
						

				}

			 current->visited =0;
			 //printf("->vgazw to %d\n",current->id);	
			 Remove_from_Stack(s);
	}


}

struct Communities* GirvanNewman(double mod, Graph *g){

	
	Communities *com = Communities_Creation();
	int i=0;
	GNodeList *GNL = Create_GNodeList();
	
	Node *node;
	CGLNode *c = g->CGL->head;	
	while(c!=NULL){

		for(i=0;i<c->node_num;i++){
			node = c->CGmembers[i].Member;
			InsertSGNodeList(GNL,node);
		}

		c = c->next;
	}

	int state=0;
	double prev_Modularity=-1,cur_Modularity=0;
	Coherent_Graph_List *tmp;
	while(1){
		Edge_betweenness(g,state);
		Apply_Changes_to_Graph(g,GNL);
		cur_Modularity = Modularity(g,GNL);
		//printf("Modularity %lf\n",cur_Modularity);
		if(cur_Modularity < prev_Modularity || cur_Modularity==0){
			tmp = g->CGL;
			g->CGL = g->prev_CGL;
			break;

		}
		else{
			DestroyCGL(g->prev_CGL);
			prev_Modularity = cur_Modularity;
			state++;

		}
		
	}


	GNodeList *nodeList;
	
	c = g->CGL->head;
	
	while(c!=NULL){
		nodeList = Create_GNodeList();
		for(i=0;i<c->node_num;i++){
			node = c->CGmembers[i].Member;
			InsertSGNodeList(nodeList,node);
			
		}
		Sort_GNodeList(nodeList->GNodeArray,0,nodeList->size-1);
		Insert_Communities_GNLS(com,nodeList);

		c = c->next;
	}

	for(i=0;i<g->edge_hash->info.hash_table_size;i++){
		free(g->edge_hash->bucket[i].t_node->data);
		free(g->edge_hash->bucket[i].t_node); 
	}
	free(g->edge_hash->bucket);
	free(g->edge_hash);
	
	free(GNL->GNodeArray);
	free(GNL);



	return com;
	
} 

void Apply_Changes_to_Graph(Graph *g,GNodeList *GNL){

	int i=0;
	
	Node *node,*repr;
	Coherent_Graph_List *next_CGL,*tmp;
	Initialize_CGL(&next_CGL);
	CGLNode *newCGLnode,*cur;
	
	tmp = g->CGL;
	g->CGL = next_CGL;
	g->prev_CGL = tmp;
	//DestroyCGL(tmp);	

	for(i=0;i<GNL->size;i++){
		node = GNL->GNodeArray[i];
		newCGLnode = malloc(sizeof(CGLNode));
		node->Graph_ID = -1;
		newCGLnode->Repr_node_id = node->id;
		newCGLnode->node_num =0;
		newCGLnode->CGmembers = malloc(sizeof(Node)*4);	
		newCGLnode->max_size = 4;
		CGL_Link(newCGLnode,g->CGL);

	}

		
	cur = g->CGL->head;
	
	while(cur!=NULL){
		repr = LookupNode(g,cur->Repr_node_id);
		
		repr->Graph_ID = repr->id;	
		IncreaseNodesofCG(g->CGL,repr->id,repr);
		Fix_graph_Members(g,cur->Repr_node_id,repr);

		cur = cur->next;
	}


}

double Modularity(Graph *g,GNodeList *GNL){

	int m =2,c=3;
	int degr_i,degr_j;
	int edge_num = g->total_edges/2;
	double Q=0;
	double cur_sum=0;
	int i=0,j=0;
	struct Cell *matched;
	Node *node_i,*node_j;
	TNode tn;
	matched = Create_T_Hash_table(m,c);

	for(i=0;i<GNL->size;i++){
		node_i = GNL->GNodeArray[i];
		if(node_i->E_List!=NULL)
			degr_i = node_i->E_List->size;
		else
			degr_i = 0;

		tn.id = node_i->id;
		T_InsertNode(&tn,matched);
		for(j=0;j<GNL->size;j++){
			node_j = GNL->GNodeArray[j];
			if(node_j->E_List!=NULL)
				degr_j = node_j->E_List->size;
			else
				degr_j = 0;

			if(T_Search(matched,node_j->id)==NULL){
				//printf("%d-%d A %d degr_i %d degr_j %d d %d\n",node_i->id,node_j->id,A(node_i,node_j),degr_i,degr_j,d(node_i,node_j));
				//getchar();
				if(node_i->id != node_j->id)
					cur_sum = cur_sum + (A(node_i,node_j) - (double)(degr_i*degr_j)/(double)(2*edge_num))*d(node_i,node_j);

			}


		}


	}

	for(i=0;i<matched->info.hash_table_size;i++)
		free(matched->bucket[i].t_node); 
	
	free(matched->bucket);
	free(matched);


	Q = ((double)1/(double)(2*edge_num))*cur_sum;


	return Q;
}

int A(Node *node_i,Node *node_j){

	if(T_Search(node_i->knows_hash_table,node_j->id)!=NULL)
		return 1;
	else
		return 0;
}

int d(Node *node_i,Node *node_j){

	if(node_i->Graph_ID == node_j->Graph_ID)
		return 1;
	else
		return 0;


}



