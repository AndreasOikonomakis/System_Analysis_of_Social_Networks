#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Community_Graph_Utilities.h"

Super_Graph *Super_Graph_Creation(List_of_CliqueList *List_of_CL,int k){

	int m=2,c=3;
	int i=0,j=0,l=0;	
	Clique *cur_clique,*cand_clique;
	Clique_List *CL;
	struct Cell *matched = Create_T_Hash_table(m,c);
	Super_Graph *sg;
	sg = malloc(sizeof(Super_Graph));
	sg->total_nodes=0;
	sg->total_edges=0;
	//sg->CL = CL;
	sg->com = Communities_Creation();

	
	for(l=0;l<List_of_CL->cur_size;l++){
		
		CL = List_of_CL->CL[l];
		//printf("Ston grafo 8a mpoun oi klikes\n");
		for(i=0;i<CL->cur_size;i++){
			cur_clique = CL->clique_Array[i];
			//Print_Clique(cur_clique);
			for(j=0;j<CL->cur_size;j++){
				cand_clique = CL->clique_Array[j];
				if(cand_clique->c_id != cur_clique->c_id){
					
						if(Cliques_Are_Neighbors(cur_clique,cand_clique,k)){
							//printf("H clique %d einai geitonas ths %d\n",cand_clique->c_id,cur_clique->c_id);
							if(cur_clique->Neighbor_cliques == NULL)
								cur_clique->Neighbor_cliques = Clique_List_Creation();

							InsertCliqueList(cur_clique->Neighbor_cliques,cand_clique);
		
						}


				}

			}

		}

	}

	Clique_List *cl;
	for(l=0;l<List_of_CL->cur_size;l++){
		
		CL = List_of_CL->CL[l];
		for(i=0;i<CL->cur_size;i++){
		
			cur_clique = CL->clique_Array[i];

		
			if(T_Search(matched,cur_clique->c_id)==NULL){
				//Print_Clique(cur_clique);
				cl = Clique_List_Creation();
				InsertCliqueList(cl,cur_clique);
				//printf("episkeptomai thn %d\n",cur_clique->c_id);
				Find_Communities(matched,cur_clique,cl);	
				Communities_Insert(sg->com,cl);

			}

		}
	}

	for(i=0;i<matched->info.hash_table_size;i++)
		free(matched->bucket[i].t_node); 
	
	free(matched->bucket);
	free(matched);

	//Print_Communities(sg->com);
	//Destroy_Communities(sg->com);
	//free(sg->CL->clique_Array);
	//free(sg->CL);	
	
	
	
	return sg;
}

int Cliques_Are_Neighbors(Clique *cur_clique,Clique *cand_clique,int k){

	int i,j,found=0;

	for(i=0;i<cur_clique->cur_size;i++){

		for(j=0;j<cand_clique->cur_size;j++){

			if(cur_clique->clique_members[i]->id == cand_clique->clique_members[j]->id)
				found++;
		}

	}

	if(found<k-1)
		return 0;
	else
		return 1;
}

void Find_Communities(struct Cell *matched,Clique *clq,Clique_List *cl){

	int i;
	TNode tn;
	Clique *cur_clique;
	tn.id = clq->c_id;
	//printf("mpainei h %d sto hash\n",tn.id);
	T_InsertNode(&tn,matched);
	if(clq->Neighbor_cliques==NULL)
		return;
	for(i=0;i<clq->Neighbor_cliques->cur_size;i++){

			cur_clique = clq->Neighbor_cliques->clique_Array[i];
			if(T_Search(matched,cur_clique->c_id)==NULL){
				InsertCliqueList(cl,cur_clique);
			Find_Communities(matched,cur_clique,cl);
	
			}
	}

}

Communities *Communities_Creation(){

	Communities *list =malloc(sizeof(Communities));
	list->cur_size = 0;
	list->max_size = 5;
	list->cur_gnl_size = 0;
	list->max_gnl_size = 5;
	list->com = malloc(sizeof(struct Clique_List*)*list->max_size);
	list->GNLS = malloc(sizeof(struct GNodeList*)*list->max_gnl_size);
	return list;


}

void Destroy_Communities(struct Communities *list){

	int i,j;
	Clique *cur_clique;

	for(i=0;i<list->cur_size;i++){		


		for(j=0;j<list->com[i]->cur_size;j++){
			cur_clique = list->com[i]->clique_Array[j];
			if(cur_clique->Neighbor_cliques!=NULL)
				Destroy_Neighbor_cliques(cur_clique->Neighbor_cliques);
		}

		
		Destroy_Clique_List(list->com[i]);
	}
	
	free(list->com);
	free(list);

}


void Communities_Insert(struct  Communities *list,struct Clique_List *cl){

	Clique_List **c;
	(list->cur_size)++;
	if(list->cur_size > list->max_size){
		
		list->max_size = 2*(list->max_size);
		c = realloc(list->com,sizeof(struct Clique_List*)*list->max_size);
		list->com = c;
		list->com[list->cur_size -1] = cl;
	}
	else{		
		list->com[list->cur_size -1] = cl;
	}

}


void Insert_Communities_GNLS(struct Communities *list,struct GNodeList *GNL){

	GNodeList **gnl;
	(list->cur_gnl_size)++;
	
	if(list->cur_gnl_size > list->max_gnl_size){
			
		list->max_gnl_size = 2*(list->max_gnl_size);
		gnl = realloc(list->GNLS,sizeof(struct GNodeList*)*list->max_gnl_size);
		list->GNLS = gnl;
		list->GNLS[list->cur_gnl_size -1] = GNL;
	}
	else{		
		list->GNLS[list->cur_gnl_size -1] = GNL;		
	}


}


void FIX_Communities(struct  Communities *communities){

	GNodeList *GNL;
	int m=2,c=3;
	struct Cell *printed;
	TNode tn;
	Node *com_member;
	//GNL = Create_GNodeList();
	 //InsertSGNodeList(GNodeList *,struct Node *);
	int i=0,j=0,k=0;	
	Clique_List *cur_community;
	for(i=0;i<communities->cur_size;i++){
		//printf("Community in forum %s:",g->Forum_Name);
		GNL = Create_GNodeList();
		printed = Create_T_Hash_table(m,c);
		cur_community = communities->com[i];
		for(j=0;j<cur_community->cur_size;j++){


			for(k=0;k<cur_community->clique_Array[j]->cur_size;k++){	
				com_member = cur_community->clique_Array[j]->clique_members[k]; 
				if(T_Search(printed,com_member->id)==NULL){
					InsertSGNodeList(GNL,com_member);
					tn.id = com_member->id;
					T_InsertNode(&tn,printed);
				}

			}

		}


		Sort_GNodeList(GNL->GNodeArray,0,GNL->size-1);
		Insert_Communities_GNLS(communities,GNL);

		for(j=0;j<printed->info.hash_table_size;j++)
			free(printed->bucket[j].t_node); 
	
		free(printed->bucket);
		free(printed);


	}


}

void Print_Communities(struct Communities *communities,Graph *g){

	int i=0,j=0;
	GNodeList *cur_GNL;
	for(i=0;i<communities->cur_gnl_size;i++){
		printf("Community in forum %s:",g->Forum_Name);
		cur_GNL = communities->GNLS[i];
		for(j=0;j<cur_GNL->size;j++)
			printf("%d ",cur_GNL->GNodeArray[j]->id);
		
		putchar('\n');
	}


}


Communities* cliquePercolationMethod(int k, Graph* g){

	Communities *com;
	List_of_CliqueList *List_of_CL;
	List_of_CL = Find_Cliques(g,k);
	Super_Graph *sg = Super_Graph_Creation(List_of_CL,k);

	com = sg->com;

	free(sg);
	FIX_Communities(com);
	return com;
} 


void Sort_GNodeList(Node **node,int first,int last){


   int pivot,j,i;
	Node **Array,*tmp;		
	Array = node;
	
   if(first<last){
         pivot=first;
         i=first;
         j=last;
         while(i<j){
             while(Array[i]->id <= Array[pivot]->id && i<last)
                 i++;
             while(Array[j]->id > Array[pivot]->id)
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

         Sort_GNodeList(Array,first,j-1);
         Sort_GNodeList(Array,j+1,last);

    }

}



