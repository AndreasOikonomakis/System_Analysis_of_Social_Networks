#ifndef COM_U_H
#define COM_U_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"
#include "GraphLib.h"
#include "graph.h"
#include "Statistics.h"
#include "hashing.h"
#include "Clique_Utilities.h"


typedef struct Communities{

	struct GNodeList **GNLS;
	struct Clique_List **com;	
	int cur_size;
	int max_size;
	int cur_gnl_size;
	int max_gnl_size;

}Communities;

Communities *Communities_Creation();
void Destroy_Communities(struct Communities *);
void Communities_Insert(struct  Communities *,struct Clique_List *);
void FIX_Communities(struct  Communities *);
void Print_Communities(struct Communities *,Graph *);
void Insert_Communities_GNLS(struct Communities*,struct GNodeList *);

typedef struct Super_Graph{

	int total_nodes;//plh8os komvwn
	int total_edges;//to sunoliko plh8os twn akmwn	
	//Clique_List *CL;
	Communities *com;


}Super_Graph;

Super_Graph *Super_Graph_Creation(List_of_CliqueList *,int);

int Cliques_Are_Neighbors(Clique *,Clique *,int);

void Find_Communities(struct Cell *,Clique *,Clique_List *);

Communities* cliquePercolationMethod(int , Graph* );

void Sort_GNodeList(Node **,int,int);
#endif



