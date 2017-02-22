#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"
#include "GraphLib.h"
#include "graph.h"
#include "Statistics.h"
#include "Clique_Utilities.h"
#include "hashing.h"
#include "Community_Graph_Utilities.h"

Clique *Clique_Creation(int clique_size){//dhmiourgia klikas me mege8os clique_size

	Clique *c;
	c = malloc(sizeof(struct Clique));
	c->clique_members = malloc(sizeof(struct Node*)*clique_size);
	c->cur_size = 0;
	c->max_size = clique_size;
	c->Neighbor_cliques = NULL;
	return c;
}


void Sort_Nodes_by_ins_num(Node **node,int first,int last){//ta3inomhsh komvwn analoga me ton ari8mo eisagwghs


   int pivot,j,i;
	Node **Array,*tmp;		
	Array = node;
	
   if(first<last){
         pivot=first;
         i=first;
         j=last;
         while(i<j){
             while(Array[i]->ins_num <= Array[pivot]->ins_num && i<last)
                 i++;
             while(Array[j]->ins_num > Array[pivot]->ins_num)
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

         Sort_Nodes_by_ins_num(Array,first,j-1);
         Sort_Nodes_by_ins_num(Array,j+1,last);

    }

}



void Insert_to_Clique(Clique *clq,struct Node *n){//eisagwgh sthn klika

	clq->clique_members[clq->cur_size] = n;
	clq->cur_size++;
}


void Fix_Clique_id(Clique *clq){//ftia3e to id ths klikas analoga me tous ari8mous eisagwghs twn komvwn

	int i;
	
	char str[MAX_STRING_LENGTH];
	Sort_Nodes_by_ins_num(clq->clique_members,0,clq->cur_size-1);
	sprintf(str, "%d", clq->clique_members[0]->ins_num);
	strcpy(clq->clique_id,str);
	for(i=1;i<clq->cur_size;i++){
		
		sprintf(str, "%d", clq->clique_members[i]->ins_num);
		strcat(clq->clique_id,str);
		strcat(clq->clique_id,"\0");
	}

	if(clq->cur_size == clq->max_size)
		clq->c_id = atoi(clq->clique_id);
}

void Insert_Past_members_to_Clique(Clique *old_clq,Clique *new_clq){//eisagwgh melwn ths prohgoumenhs klikas

	int i;
	Node *cur_clq_mem;
	for(i=0;i<old_clq->cur_size;i++){
		cur_clq_mem = old_clq->clique_members[i];
		Insert_to_Clique(new_clq,cur_clq_mem);
	}


}


int Belong_To_Clique(Clique *cl,struct Node *n){//sunartish pou krinei an enas komvos anhkei se mia klika

	int found=0,i;
	Node *cur_clq_mem;
	
	for(i=0;i<cl->cur_size;i++){
		cur_clq_mem = cl->clique_members[i];
		if(T_Search(cur_clq_mem->knows_hash_table,n->id)!=NULL)//ean ton 3erei to trexon melos
			found++;//metavlhth pou krataei posoi apo ta melh ths klikas ton 3eroun
	}

	if(found == cl->cur_size)//ean ton 3eroun oloi shmainei oti anhke sthn klika
		return 1;
	else
		return 0;
}

void Print_Clique(Clique *cl){
	
	int i;
	printf("[");
	for(i=0;i<cl->cur_size-1;i++)
		printf("%d,",cl->clique_members[i]->id);

	printf("%d]",cl->clique_members[i]->id);

}

Clique_List *Clique_List_Creation(){//dhmiougia listas klikwn

	Clique_List *CL;
	CL = malloc(sizeof(struct Clique_List));
	
	CL->keep = 0;
	CL->sorted = 0;
	CL->cur_size = 0;
	CL->max_size = 5;

	CL->clique_Array = malloc(sizeof(struct Clique*)*CL->max_size);

	return CL;
}

void Destroy_Neighbor_cliques(struct Clique_List *list){//katastrofh listas geitonikwn klikwn

	
	free(list->clique_Array);

	free(list);
}


void Destroy_Clique_List(Clique_List *list){//katastrofh lista klikwn

	int i;
	
	for(i=0;i<list->cur_size;i++){		

		free(list->clique_Array[i]->clique_members);
		free(list->clique_Array[i]);
	}
	free(list->clique_Array);

	free(list);	


}



void InsertCliqueList(struct  Clique_List *list,struct Clique *cl){//eisagwgh klikas sth lista klikwn

	Clique **c;
	(list->cur_size)++;
	if(list->cur_size > list->max_size){
		
		list->max_size = 2*(list->max_size);
		c = realloc(list->clique_Array,sizeof(struct Clique*)*list->max_size);
		list->clique_Array = c;
		list->clique_Array[list->cur_size -1] = cl;
	}
	else{		
		list->clique_Array[list->cur_size -1] = cl;
	}

}

void Sort_Clique_List(Clique **c,int first,int last){//ta3inomhsh klikwn analoga me to id tous

   int pivot,j,i;
	Clique **Array,*tmp;		
	Array = c;
	
   if(first<last){
         pivot=first;
         i=first;
         j=last;
         while(i<j){
             while(Array[i]->c_id <= Array[pivot]->c_id && i<last)
                 i++;
             while(Array[j]->c_id > Array[pivot]->c_id)
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

         Sort_Clique_List(Array,first,j-1);
         Sort_Clique_List(Array,j+1,last);

    }

}

Clique *Clique_List_Search(int key,Clique_List *CL){//anazhthsh se lista apo klikes

	int first;
	int last;
	int middle;
	int place = -1;
	if(CL->sorted == 0){
		Sort_Clique_List(CL->clique_Array,0,CL->cur_size-1);
		CL->sorted = 1;
	}

	if(CL->cur_size > 0){

		first=0;
		last = CL->cur_size-1;
		while(first <= last){
			middle = (first + last)/2;
  
			if(CL->clique_Array[middle]->c_id == key){
					place = middle;
					break;			
			}			
			else if (CL->clique_Array[middle]->c_id > key)
				last = middle-1;			
			else
         	first = middle+1;      	
		}

	}

	if(place == -1)
		return NULL;
	else
		return CL->clique_Array[place];

}



List_of_CliqueList *Find_Cliques(Graph *g,int k){

	int i,j,m,clq_size=1;
	Clique *clq,*cur_clq,*new_clq;
	List_of_CliqueList *list_of_clique_list;
	list_of_clique_list = List_of_CliqueList_Creation();

	Node *n;

	Clique_List *CL,*next_CL,*tmp;
	
	struct Cell *ignore_duplicate_cliques;
	TNode tn;

	CGLNode *c = g->CGL->head;

	while(c!=NULL){//Gia ka8e sunektiko grafo
		CL = Clique_List_Creation();//dhmiourgia listas klikwn gia ka8e sunektiko grafo
		clq_size=1;//Oloi oi komvoi einai upopshfies klikes
		for(i=0;i<c->node_num;i++){

			n = c->CGmembers[i].Member;
		
			if(c->CGmembers[i].Member->E_List!=NULL){//agnohse tous komvous pou dn exoun geitones
								
				if(n->E_List->size>=k-1){//agnohse tous komvous pou ligoterous apo k-1 geitones
					clq = Clique_Creation(1);
					Insert_to_Clique(clq,n);
					InsertCliqueList(CL,clq);//eisagwgh gia mia lista me upopshfies 1-cliques
				}
	
			}

		}
	
		for(clq_size=1;clq_size<k;clq_size++){//epanalhpsh mexri na ftasei to mege8os ths klikas pou 8eloume
			

			ignore_duplicate_cliques = Create_T_Hash_table(2,3);//hash gia na agnoh8oun oi hdh uparxouses klikes
			next_CL = Clique_List_Creation();//epomenh lista gia upopshfies klikes mege8ous clq_size	
			for(j=0;j<CL->cur_size;j++){

					cur_clq = CL->clique_Array[j];					
					
					for(i=0;i<c->node_num;i++){
						new_clq = Clique_Creation(cur_clq->cur_size+1);//dhmiougia neas klikas
						n = c->CGmembers[i].Member;	
						Insert_Past_members_to_Clique(cur_clq,new_clq);//eisagwgh paliwn melos ths klikas
						if(Belong_To_Clique(cur_clq,n) && n->E_List->size>=k-1)
							Insert_to_Clique(new_clq,n);								


						if(new_clq->cur_size == new_clq->max_size){//ean h nea klika exei ftasei sto apaitoumeno trexon mege8os
							Fix_Clique_id(new_clq);//ftia3e to id ths klikas
							tn.id = atoi(new_clq->clique_id);
							if(T_Search(ignore_duplicate_cliques,tn.id)==NULL){//psa3e ean uparxei hdh
								InsertCliqueList(next_CL,new_clq);
								T_InsertNode(&tn,ignore_duplicate_cliques);
							}
							else{//ean uparxei hdh katastrepse thn
								free(new_clq->clique_members);
								free(new_clq);
							}
						}
						else{//ean den exei ftasei to apaitoumeno mege8os katastrepse thn
							free(new_clq->clique_members);
							free(new_clq);
						}



					}	
					
			

			}
			tmp = CL;
			CL = next_CL;
			if(clq_size+1 == k)//kratame thn lista me tis telikes k-cliques
				CL->keep = 1;
	
			for(m=0;m<ignore_duplicate_cliques->info.hash_table_size;m++)
				free(ignore_duplicate_cliques->bucket[m].t_node); 
	
			free(ignore_duplicate_cliques->bucket);
			free(ignore_duplicate_cliques);
			
			if(tmp->keep =! 1 || tmp->cur_size == 0)
				Destroy_Clique_List(tmp);
			else if(CL->keep == 1)				
				Insert_List_of_CliqueList(list_of_clique_list,CL);
			
		}
		
		
		c = c->next;
	}


	return list_of_clique_list;//epistrofh mia listas pou periexei listes me cliques/ka8e lista gia ka8e sunektiko grafo

}


List_of_CliqueList *List_of_CliqueList_Creation(){

	List_of_CliqueList *list =malloc(sizeof(List_of_CliqueList));
	list->cur_size = 0;
	list->max_size = 5;
	list->CL = malloc(sizeof(struct Clique_List*)*list->max_size);

	return list;

}

void Insert_List_of_CliqueList(struct  List_of_CliqueList *list,struct Clique_List *cl){

	Clique_List **c;
	(list->cur_size)++;
	if(list->cur_size > list->max_size){
		
		list->max_size = 2*(list->max_size);
		c = realloc(list->CL,sizeof(struct Clique_List*)*list->max_size);
		list->CL = c;
		list->CL[list->cur_size -1] = cl;
	}
	else{		
		list->CL[list->cur_size -1] = cl;
	}

}

void Destroy_List_of_CliqueList(List_of_CliqueList *list){

	int i;

	for(i=0;i<list->cur_size;i++)				
		Destroy_Clique_List(list->CL[i]);
	
	
	free(list->CL);
	free(list);

}

void Print_List_of_CliqueList(List_of_CliqueList *list){

	int i,j;
	Clique_List *cur_list;
	for(i=0;i<list->cur_size;i++){
		printf("Clique list %d: ",i+1);
		cur_list = list->CL[i];
		for(j=0;j<cur_list->cur_size;j++)
			Print_Clique(cur_list->clique_Array[j]);
		putchar('\n');

	}


}

/////test//////
void test(Graph *g){
    
	int m=2,c1=3;
	edge_bet_counter *ebc;
	
	TNode tn;
	g->edge_hash = Create_T_Hash_table(m,c1);
	FILE *fp;
	fp = fopen ("dataset1/persons.txt", "r");   
	int id;
	Node *n,*n2;
	while(fscanf(fp,"%d",&id)!=EOF){	 
		n = setPersonProperties(id,"","",0,"");
		insertNode(n, g);		
	}	

		
	fclose(fp);
	fp = fopen ("dataset1/edges.txt", "r");
	int start,end;
	int skata =0;
	Edge *e;
	while(fscanf(fp,"%d %d",&start,&end)!=EOF){
		e = setEdgeProperties(start,end,"null", 1.0);
		n = LookupNode(g,start);
		n2 = LookupNode(g,end);
		e->EdgeID = Fix_EdgeID(n->ins_num,n2->ins_num);
		insertEdge(start,e, g);

		if(T_Search(g->edge_hash,e->EdgeID) == NULL){
			tn.id = e->EdgeID;
			ebc = malloc(sizeof(edge_bet_counter));
			ebc->e_ptr = e;
			ebc->state = -1;
			ebc->bet_num = 0;
			ebc->edge_betweenness = 0;
			tn.data = ebc;
			T_InsertNode(&tn,g->edge_hash);
		}
	}
 
	fclose(fp);
 
	
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


}

