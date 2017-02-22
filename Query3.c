#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"graph.h"
#include"hashing.h"
#include"defines.h"
#include"Queries.h"

Graph *M_W_Graph_Create(int m , int c){

	struct Graph *gr;
	gr = malloc(sizeof(struct Graph));
	gr->hash_table = Create_T_Hash_table(m,c);
	gr->total_nodes=0;
	gr->total_edges=0;
	gr->cur_bridgeID=-1;
	gr->betweeness_num=0;
	gr->max_edges = -1;
	gr->h_ORG = NULL;
	gr->h_INT = NULL;
	gr->h_PL = NULL;

	Initialize_CGL(&(gr->CGL));

	return gr;
}

void M_W_InsertNode(Node *node,Graph *graph){

	TNode t_node;

	t_node.id = node->id;
	t_node.Nptr = node;									
	
	T_InsertNode(&t_node,graph->hash_table);

	
	(graph->total_nodes)++;//au3hsh twn komvwn tou grafou gia ton komvo pou mphke omala
	//dhmiourgia kovmou gia thn lista CGL gia olous tous komvous tou grafou ws uposphfious antiproswpous gia coherent grafous
	CGLNode *newCGLnode;
	newCGLnode = malloc(sizeof(CGLNode));
	newCGLnode->Repr_node_id = node->id;
	newCGLnode->node_num =0;
	newCGLnode->CGmembers = malloc(sizeof(Node)*4);	
	newCGLnode->max_size = 4;
	CGL_Link(newCGLnode ,graph->CGL);


}

void Fix_Mgraph_Members(Graph *g,int repr_id,Node *cur_node){

	int i=0;

	if(cur_node->MSG!=NULL){

		for(i=0;i<cur_node->MSG->size;i++){

				if(cur_node->MSG->GNodeArray[i]->Graph_M_ID==-1){					
					cur_node->MSG->GNodeArray[i]->Graph_M_ID= repr_id;	
					IncreaseNodesofCG(g->CGL,repr_id,cur_node->MSG->GNodeArray[i]);
					RemoveCGL(g->CGL,cur_node->MSG->GNodeArray[i]->id);
					Fix_Mgraph_Members(g,repr_id,cur_node->MSG->GNodeArray[i]);
					
				}


		}
	}
}

void Fix_Wgraph_Members(Graph *g,int repr_id,Node *cur_node){

	int i=0;

	if(cur_node->WSG!=NULL){

		for(i=0;i<cur_node->WSG->size;i++){

				if(cur_node->WSG->GNodeArray[i]->Graph_W_ID==-1){					
					cur_node->WSG->GNodeArray[i]->Graph_W_ID= repr_id;	
					IncreaseNodesofCG(g->CGL,repr_id,cur_node->WSG->GNodeArray[i]);

					RemoveCGL(g->CGL,cur_node->WSG->GNodeArray[i]->id);
					Fix_Wgraph_Members(g,repr_id,cur_node->WSG->GNodeArray[i]);
					
				}


		}
	}
}



void findTrends(int k, Graph *g, char **womenTrends, char **menTrends){


	
	CGLNode *cur;
	cur = g->M_graph->CGL->head;
	Node *repr;
	
	while(cur!=NULL){
		repr = LookupNode(g->M_graph,cur->Repr_node_id);
		repr->Graph_M_ID = repr->id;	
		IncreaseNodesofCG(g->M_graph->CGL,repr->id,repr);
		Fix_Mgraph_Members(g->M_graph,cur->Repr_node_id,repr);	
		cur = cur->next;
	}	

	
	cur = g->W_graph->CGL->head;
	while(cur!=NULL){
		repr = LookupNode(g->W_graph,cur->Repr_node_id);
		repr->Graph_W_ID = repr->id;	
		IncreaseNodesofCG(g->W_graph->CGL,repr->id,repr);
		Fix_Wgraph_Members(g->W_graph,cur->Repr_node_id,repr);
		cur = cur->next;
	}	


	Traverse_M_W_Graph_by_Trend(g->W_graph,womenTrends,k);
	Traverse_M_W_Graph_by_Trend(g->M_graph,menTrends,k);

	

}


void quicksort_Trends(RNode **Trends,int first,int last,int size){
 

   int pivot,j,i;
	RNode **Array,*tmp;		
	Array =Trends;

  if(first<last){
         pivot=first;
         i=first;
         j=last;

         while(i<j){
				 
             while(Array[i]->max_freq <= Array[pivot]->max_freq && i<last)
                 i++;
             while(Array[j]->max_freq>Array[pivot]->max_freq)
                 j--;
             if(i<j){
										  
					  tmp = Array[i];
					  Array[i] = Array[j];	
					  Array[j] = tmp;	
             }
         }


 			tmp=Array[pivot];
         Array[pivot]=Array[j];
 
			Array[j] = tmp;	
         quicksort_Trends(Array,first,j-1,size);
         quicksort_Trends(Array,j+1,last,size);

    }
}

void Traverse_M_W_Graph_by_Trend(Graph *g,char **M_W_Trends,int k){

	int m = 2;
   int h = 3;

	int i=0,j=0;
	TNode t_node;
	RNode *cur_inter;
	CGLNode *c = g->CGL->head;
	Node *cur_node;
	RNodeList *Trends;	
	Trends = malloc(sizeof(RNodeList));
	Trends->size = 0;
	Trends->max_size = 2;
	Trends->RArray = malloc(sizeof(RNode*)*Trends->max_size);

	while(c!=NULL){		
		if(g->CGL->CGmax_ID == c->Repr_node_id)
			break;		
		c = c->next;
	}		

	for(i=0;i<c->node_num;i++){
		//printf("%d %s\n",c->CGmembers[i].Member->id,c->CGmembers[i].Member->node_prop->pArray[0]);	
		for(j=0;j<c->CGmembers[i].Member->interests->size;j++){
			//printf("%s\n",c->CGmembers[i].Member->interests->RArray[j]->node_prop->pArray[0]);
			
			if(c->CGmembers[i].Member->interests->RArray[j]->matched == 0){
				//cur_inter = c->CGmembers[i].Member->interests->RArray[j];
///////////////*******Dhmiougria hash_table gia ta matched*******/////////////////////////////////////////////
				c->CGmembers[i].Member->interests->RArray[j]->matched_hash_table = Create_T_Hash_table(m,h);
				cur_inter = c->CGmembers[i].Member->interests->RArray[j];
				InsertRNodeList(Trends,cur_inter);
				cur_inter->matched = 1;
				
				t_node.id = c->CGmembers[i].Member->id;
				t_node.Nptr = c->CGmembers[i].Member; 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
				T_InsertNode(&t_node,cur_inter->matched_hash_table);
				//printf("??inter %s from %d\n",cur_inter->node_prop->pArray[0],c->CGmembers[i].Member->id);
				cur_inter->freq = 1;
				Traverse_by_interest(cur_inter,c->CGmembers[i].Member,cur_inter->matched_hash_table);

				if(cur_inter->freq> cur_inter->max_freq)
					cur_inter->max_freq = cur_inter->freq;
			}
			cur_node = c->CGmembers[i].Member;

			if(T_Search(cur_inter->matched_hash_table,cur_node->id) == NULL){

				t_node.id = c->CGmembers[i].Member->id;
				t_node.Nptr = c->CGmembers[i].Member; 
				T_InsertNode(&t_node,cur_inter->matched_hash_table);
				//printf("->??inter %s from %d\n",cur_inter->node_prop->pArray[0],c->CGmembers[i].Member->id);
				cur_inter->freq = 1;
				Traverse_by_interest(cur_inter,c->CGmembers[i].Member,cur_inter->matched_hash_table);

				if(cur_inter->freq> cur_inter->max_freq)
					cur_inter->max_freq = cur_inter->freq;

			}

		}
										
	}


	quicksort_Trends(Trends->RArray,0,Trends->size -1,Trends->size);

	j=0;
	for(i=Trends->size-1;j<k;i--){			
			if(Trends->RArray[i]->max_freq == 1)	
				M_W_Trends[j] = NULL;
			else
				strcpy(M_W_Trends[j],Trends->RArray[i]->node_prop->pArray[0]);
			j++;
	}


	for(i=0;i<Trends->size;i++){
		//printf("%s -> %d\n",Trends->RArray[i]->node_prop->pArray[0],Trends->RArray[i]->max_freq);
		Trends->RArray[i]->matched = 0;
		for(j=0;j<Trends->RArray[i]->matched_hash_table->info.hash_table_size;j++){
			free(Trends->RArray[i]->matched_hash_table->bucket[j].t_node); 
		}
		free(Trends->RArray[i]->matched_hash_table->bucket);
		free(Trends->RArray[i]->matched_hash_table);
		Trends->RArray[i]->freq= 0;	
		Trends->RArray[i]->max_freq =0;
	}


	


	free(Trends->RArray);		
	free(Trends);


}

void Traverse_by_interest(RNode *cur_inter,Node *node,Cell *matched_hash_table){

	int i=0,j=0;

	TNode *t_node;
	Node *cur_n;
	
	if(!strcmp(node->node_prop->pArray[3],"female")){

		for(i=0;i<node->WSG->size;i++){
			cur_n = node->WSG->GNodeArray[i];
			for(j=0;j<cur_n->interests->size;j++){
				if(cur_inter->id == cur_n->interests->RArray[j]->id && T_Search(matched_hash_table,cur_n->id) == NULL){
					//printf("o %d exei inter %s\n",cur_n->id,cur_n->interests->RArray[j]->node_prop->pArray[0]);
					t_node = malloc(sizeof(TNode));
					t_node->id = cur_n->id;
					t_node->Nptr = cur_n; 
					T_InsertNode(t_node,matched_hash_table);
					(cur_inter->freq)++;
					free(t_node);
					Traverse_by_interest(cur_inter,cur_n,matched_hash_table);
				}
	
			}

		}

	}
	else{

		for(i=0;i<node->MSG->size;i++){
			cur_n = node->MSG->GNodeArray[i];
			for(j=0;j<cur_n->interests->size;j++){
				if(cur_inter->id == cur_n->interests->RArray[j]->id && T_Search(matched_hash_table,cur_n->id) == NULL){
					//printf("o %d exei inter %s\n",cur_n->id,cur_n->interests->RArray[j]->node_prop->pArray[0]);
					t_node = malloc(sizeof(TNode));
					t_node->id = cur_n->id;
					t_node->Nptr = cur_n; 
					T_InsertNode(t_node,matched_hash_table);
					(cur_inter->freq)++;
					free(t_node);
					Traverse_by_interest(cur_inter,cur_n,matched_hash_table);
				}
	
			}

		}


	}


}

