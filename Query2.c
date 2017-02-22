#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"graph.h"
#include"hashing.h"
#include"defines.h"
#include"Queries.h"

Graph* getTopStalkers(int k, int x, int centralityMode, Graph* g, Stalkers* st){

	int m=2,c=3;
	int i=0,j=0;
	GNodeList *labeled_stalkers;
	labeled_stalkers = Create_GNodeList();
	Node *input_node;
	Graph *Stalker_graph;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////poioi xarakthrizontai ws stalkers/////////////////////////////////////////////////
	CGLNode *newCGLnode;
	CGLNode *cur = g->CGL->head;
	Node *cur_Member;
	Node *owner;
	while(cur!=NULL){
				
		for(i=0;i<cur->node_num;i++){
				cur_Member = cur->CGmembers[i].Member;	
				
				if(cur_Member->likes!=NULL){

					for(j=0;j<cur_Member->likes->size;j++){						
						owner = cur_Member->likes->RArray[j]->owner;

						if(owner!=NULL){
							if(T_Search(cur_Member->knows_hash_table,owner->id)==NULL&&cur_Member->id!=owner->id)							
								(cur_Member->Stalker_likes)++;														
						}	
											
					}
				}
				if(cur_Member->Stalker_likes>x)							
					InsertSGNodeList(labeled_stalkers,cur_Member);

		}									
		cur = cur->next;	

	}
	
	
	Stalker_graph = Build_Stalker_Graph(g,labeled_stalkers);

	free(labeled_stalkers->GNodeArray);
	free(labeled_stalkers);

	Node *n;
	Stalkers *st1;
	st1 = malloc(sizeof(Stalkers)*Stalker_graph->total_nodes);
	j=0;
	double clCent,bet_cent;
	cur = Stalker_graph->CGL->head;
	while(cur!=NULL){
		for(i=0;i<cur->node_num;i++){
			n = LookupNode(Stalker_graph,cur->CGmembers[i].Member->id);
			if(centralityMode == 1){
					//printf("---->%d stalker likes %d\n",n->id,n->Stalker_likes);
					clCent = closenessCentrality(n,Stalker_graph);
					st1[j].id = n->id;
					st1[j].centrality_score = clCent;
					st1[j].Stalker_likes = n->Stalker_likes;
					j++;
					//printf("clCent of %d is %lf\n",n->id,clCent);			
			}
			else{
					bet_cent = betweennessCentrality(n,Stalker_graph);
					st1[j].id = n->id;
					st1[j].centrality_score = bet_cent;
					st1[j].Stalker_likes = n->Stalker_likes;
					j++;
					//printf("clCent of %d is %lf\n",n->id,bet_cent);
				

			}
		}
		cur = cur->next;
	}
	
	Squicksort(&st1,0,Stalker_graph->total_nodes-1,Stalker_graph->total_nodes);
	
	int k1=0;
	
	for(j=Stalker_graph->total_nodes-1;j>=0;j--){
		st[k1].id = st1[j].id;
		st[k1].centrality_score = st1[j].centrality_score;
		k1++;
		if(k1==k)
			break;
	}
	free(st1);



	return Stalker_graph;
}

Graph *Build_Stalker_Graph(Graph *g,GNodeList *labeled_stalkers){

	int i=0,j=0;
	int m=2,c=3;	
	Edge *cur_e,*new_e;
	Graph *Stalker_graph = Graph_Create(m,c);
	Node *stalker_cand,*input_node,*nodeStart,*nodeEnd;

	for(i=0;i<labeled_stalkers->size;i++){
		stalker_cand =	labeled_stalkers->GNodeArray[i];	
		input_node = setPersonProperties(stalker_cand->id,"NULL","NULL",0,"NULL");
		insertNode(input_node,Stalker_graph);

	}

	for(i=0;i<labeled_stalkers->size;i++){
		stalker_cand =	labeled_stalkers->GNodeArray[i];	
		if(stalker_cand->E_List!=NULL){
			nodeStart = LookupNode(Stalker_graph,stalker_cand->id);	
			Initialize_E_List(&nodeStart->E_List,nodeStart->id);
	
			cur_e = stalker_cand->E_List->head;
			for(j=0;j<stalker_cand->E_List->size;j++){

				if(T_Search(Stalker_graph->hash_table,cur_e->n_node->id)!=NULL){

					nodeEnd = LookupNode(g,cur_e->n_node->id);
					new_e = setEdgeProperties(nodeStart->id,nodeEnd->id,"knows",0);
					insertEdge(nodeStart->id,new_e,Stalker_graph);		


				}
				cur_e = cur_e->next;


			}

		}

	}

	Node *repr;
	CGLNode *cur;
	cur = Stalker_graph->CGL->head;
	
	while(cur!=NULL){
		repr = LookupNode(Stalker_graph,cur->Repr_node_id);
		repr->Graph_ID = repr->id;	
		IncreaseNodesofCG(Stalker_graph->CGL,repr->id,repr);
		Fix_graph_Members(Stalker_graph,cur->Repr_node_id,repr);

		cur = cur->next;
	}

	return Stalker_graph;

}


void Squicksort(Stalkers **st,int first,int last,int size){
 
	
   int pivot,j,i;
	Stalkers *Array,tmp;		
	Array =*st;

  if(first<last){
         pivot=first;
         i=first;
         j=last;

         while(i<j){
				 
             while(Array[i].centrality_score <= Array[pivot].centrality_score && i<last)
                 i++;
             while(Array[j].centrality_score > Array[pivot].centrality_score)
                 j--;
             if(i<j){
										  
					  tmp = Array[i];
					  Array[i] = Array[j];	
					  Array[j] = tmp;	
             }
         }


 			tmp=Array[pivot];
			//printf("->Allazei to %lf me to %lf\n",tmp.score,Array[j].score);
         Array[pivot]=Array[j];
 
			Array[j] = tmp;	
         Squicksort(&Array,first,j-1,size);
         Squicksort(&Array,j+1,last,size);

    }
}
