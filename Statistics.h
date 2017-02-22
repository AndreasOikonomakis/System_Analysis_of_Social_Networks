#ifndef STATISTICS_H
#define STATISTICS_H
#include "defines.h"
#include "GraphLib.h"
#include "graph.h"


typedef struct DegreePair{

	int node_num;
	int Degree;
	double Deg_value;
}DegreePair;

typedef struct StackNode {

	struct Node *node;
	struct StackNode *up;
	struct StackNode *down;


}StackNode;


typedef struct Stack {
	
	StackNode *head;
	int size;
	int between_num;
	int total_s_path;
}Stack;


void find_All_possible_paths(Stack *,Node *,Node *,int,int);

//Stack functions
void Initialize_Stack(Stack **);
void Remove_from_Stack(Stack *);
void Insert_to_Stack(Stack *,Node *);
void Destroy_Stack(Stack *);

//Statistics functions
void degreeDistribution(Graph*);
int numberOfCCs(Graph*);
int maxCC(Graph *);
int diameter(Graph *);
double averagePathLength(Graph *);
double density(Graph* g);
double closenessCentrality(Node* , Graph* ); 
double betweennessCentrality(Node* , Graph* ); 
void Plot(int);

void testClosenessCentrality(int,int);
void testBetweennessCentrality(int,int);
void stalkersGraphRunMetrics(Graph *);

#endif
