#ifndef GN_A_H
#define GN_A_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"
#include "GraphLib.h"
#include "graph.h"
#include "Community_Graph_Utilities.h"
#include "Clique_Utilities.h"

struct Communities* GirvanNewman(double , Graph *); 


typedef struct Max_edge_cent_container{


	struct edge_bet_counter *ebc;
	double max_edge_betweenness;

}Max_edge_cent_container;

typedef struct Engaged_Edges_List{

	struct Cell *inserted;
	struct edge_bet_counter **EEA;
	int cur_size;
	int max_size;

}Engaged_Edges_List;

Engaged_Edges_List *EEL_Creation();
void EEL_Insert(Engaged_Edges_List *,edge_bet_counter *);
void EEL_Destroy(Engaged_Edges_List *);
void Compute_Edge_betweenness(Engaged_Edges_List *,int,int,Max_edge_cent_container *);


void RemoveEdge(struct e_list *,int,int);

void Edge_betweenness(Graph *,int);
void e_find_All_possible_paths(Engaged_Edges_List *,Stack *,Node *,Node *,int,Graph *,struct Cell *);

void Apply_Changes_to_Graph(Graph *,GNodeList *);

double Modularity(Graph *,GNodeList *);

int A(Node *,Node *);

int d(Node *,Node *);







#endif
