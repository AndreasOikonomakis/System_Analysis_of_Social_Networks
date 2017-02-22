#include "defines.h"
#include "GraphLib.h"
#include "graph.h"
#include "Statistics.h"

#define PROPERTIES_NUM 2


RNode *RNode_Creation(int,Properties *);
RNode *R_setProperties(int,char *);



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////*******************QUERY1**********************/////////////////////////////////////////////////
typedef struct Match{

	Node *m_node;//upopshfio match
	int com_inter_num;//ari8mos koinwn endiaferontwn
	double score;//apostash jackard

}Match;

typedef struct Matches{

	Match *mArray;
	int size;
	int max_size;

}Matches;
int getMatch(int,Matches*);
Matches *matchSuggestion(Node *,int,int,int,int,Graph*);
Node *setProperties(int,char *); 
int  age_dif(int ,int);
void MArray_Insert(Node *,int,double ,Matches*);
void quicksortMA(Match **,int ,int ,int);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////*********************QUERY2******************//////////////////////////////////////////////////////////
typedef struct Stalkers{

	int id;
	double centrality_score;
	int Stalker_likes;
}Stalkers;
Graph* getTopStalkers(int , int , int, Graph *,Stalkers *);
Graph *Build_Stalker_Graph(Graph *,GNodeList *);
void Squicksort(Stalkers **,int,int,int);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////*********************QUERY3******************//////////////////////////////////////////////////////////
Graph *M_W_Graph_Create(int , int);
void M_W_InsertNode(Node *,Graph *);
void findTrends(int k, Graph *, char **, char **);
void Traverse_M_W_Graph(Graph *,char**,int);
void quicksort_Trends(RNode **,int,int,int );
void Traverse_M_W_Graph_by_Trend(Graph *,char **,int );
void Traverse_by_interest(RNode *,Node *,struct Cell *matched_hash_table);
void Fix_Mgraph_Members(Graph *,int ,Node *);
void Fix_Wgraph_Members(Graph *,int ,Node *);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////*********************QUERY4***********************/////////////////////////////////////////////////////
struct Graph *buildTrustGraph(int , Graph *);
Edge* setEdgeTrustProperties(int, int,double); 
double estimateTrust(Node *, Node *, Graph *);
void testTidalTrust(int , int);


