#ifndef CL_U_H
#define CL_U_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"
#include "GraphLib.h"
#include "graph.h"

#define MAX_STRING_LENGTH 256

typedef struct Clique{

	int c_id;
	char clique_id[MAX_STRING_LENGTH];
	struct Node **clique_members;
	struct Clique_List *Neighbor_cliques;
	int cur_size;
	int max_size;

}Clique;

void Destroy_Neighbor_cliques(struct Clique_List *);

typedef struct Clique_List{

	int sorted;
	int keep;
	int stored_cliques_size;
	struct Clique **clique_Array;
	int cur_size;
	int max_size;

}Clique_List;

typedef struct List_of_CliqueList{

	struct Clique_List **CL;	
	int cur_size;
	int max_size;

}List_of_CliqueList;

List_of_CliqueList * List_of_CliqueList_Creation();
void Insert_List_of_CliqueList(struct  List_of_CliqueList *,struct Clique_List *);
void Destroy_List_of_CliqueList(List_of_CliqueList *);
void Print_List_of_CliqueList(List_of_CliqueList *);






Clique *Clique_Creation(int);
void Fix_Clique_id(Clique *);
void Insert_to_Clique(Clique *,struct Node *);
int Belong_To_Clique(Clique *,struct Node *);
void Insert_Past_members_to_Clique(Clique *,Clique *);
void Sort_Nodes_by_ins_num(Node **,int ,int );
void Print_Clique(Clique *);



void Destroy_Clique_List(Clique_List *);

Clique_List *Clique_List_Creation();
Clique *Clique_List_Search(int ,Clique_List *);
void Sort_Clique_List(Clique **,int,int);

void InsertCliqueList(struct  Clique_List *,struct Clique *);
List_of_CliqueList *Find_Cliques(Graph *,int);


void test(Graph *);




#endif

