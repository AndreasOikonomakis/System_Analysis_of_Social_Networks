#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"
#include "GraphLib.h"
#include "graph.h"

typedef struct GraphList{

	struct Graph **Graph_Array;	
	int size;
	int max_size;
}GraphList;


typedef struct NF_Node{

	struct FNode *FNptr;
	int member_num;
	struct NF_Node *next;
	struct NF_Node *prev;
}NF_Node;



typedef struct NForums{

	int forum_size;
	int *cur_forum_num;
	NF_Node *head;
	NF_Node *tail;
	int max_size;
	int cur_size;
	int max_members;
	int min_members;
	
}NForums;


Graph *Create_Forum_Graph(int ,int ,FNode *);
GraphList *Create_Graph_List();


void InsertGraphList(struct GraphList *,struct Graph *);
NForums *Create_NForums(int,int *,int);
void Fix_NF(NForums *,struct FNode *);
void Insert_NForums(NForums *,struct FNode *);



