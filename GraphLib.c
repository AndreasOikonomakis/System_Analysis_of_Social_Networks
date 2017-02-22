/*
 * GraphLib.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "graph.h"
#include "GraphLib.h"
/************************************/
/* Implement for test main purposes */
/************************************/


/**
 * Creates a properties object
 * number: the number of properties
 * return value: a properties object
 */
Properties* createProperties(int number) {
    
	int i=0;
	Properties *p;    
	p = malloc(sizeof(struct properties));
	p->prop_num = number;
	p->pArray = malloc(sizeof(char*)*number);
	for(i;i<number;i++)
		p->pArray[i] = malloc(sizeof(char)*MAX_STRING_LENGTH);
	return p;
}
/**
 * Sets a string property "property" in position "index"
 * property: the type of the property
 * index: the position of the property
 * p: the properties object
 */
void setStringProperty(char* property, int index, Properties* p) {
 
	strcpy(p->pArray[index],property);

}
/**
 * Sets an integer property "property" in position "index"
 * property: the value of the property
 * index: the position of the property
 * p: the properties object
 */
void setIntegerProperty(int property, int index, Properties* p) {
    sprintf(p->pArray[index], "%d", property);
}

void setfloatProperty(double property, int index, Properties* p) {
    sprintf(p->pArray[index], "%lf", property);
}
/**
 * Gets a string property "property" from position "index"
 * index: the position of the requested property
 * p: the properties object
 * return value: the type of the property
 */
char* getStringProperty(int index, Properties* p) {
	 		   
	return p->pArray[index];
}

/**
 * Gets an integer property "property" from position "index"
 * index: the position of the requested property
 * p: the properties object
 * return value: the value of the property
 */
int getIntegerProperty(int index, Properties* p) {
	int x;
	x= atoi(p->pArray[index]);		
   return x;
}

/**
 * Creates a node with specific properties
 * id: the id of the node
 * p: the properties of the node
 * return value: the requested node
 */
Node* createNode(int id, Properties* p) {
    
	Node *node;	
	node = GNode_Creation(id,p);
  return node;
}

/**
 * Creates an edge with specific properties
 * startID: the id of the start node
 * endID: the id of the end node
 * p: the properties of the edge
 * return value: the requested edge
 */
Edge* createEdge(int startID, int endID, Properties* p) {
   
	Edge *edge;
	edge =  Create_Edge(startID ,endID ,p);
   return edge;
}


/************************************/
/* Implement for part 1 */
/************************************/

/* Creates an empty graph
 * m: size of hashtable
 * c: size of cells in a bucket
 * return value: an empty graph*/
Graph* createGraph(int m, int c) {
    
	Graph * g;	
	g =  Graph_Create(m,c);
   return g;
}

/* Destroys a graph
 * g: the graph
 * return value: success/fail status */
int destroyGraph(Graph* g) {
 
   DestroyGraph(g);
   return -1;
}

/* Inserts a node in the graph
 * n: the node to insert
 * g: the graph
 * return value: success/fail status */
int insertNode(const Node* const n, Graph* g) {
    
	Node *node = (Node*)n;
	InsertNode(node,g);	
	
   return 0;
}

/* Inserts an edge in the graph
 * startID: the id of the start node
 * e: the edge we want to add to the start node
 * g: the graph
 * return value: success/fail status */
int insertEdge(int startID, const Edge* const e, Graph* g) {
    
	Edge* edge = (Edge*)e;
	InsertEdge(startID,edge,g);
   return -1;
}

/* Retrieves a node from the graph
 * nodeID: the if of node to retrieve
 * g: the graph
 * return value: the requested node */
Node* lookupNode(int nodeID, const Graph* const g) {
    
	Graph* graph = (Graph*)g;
	Node *node;
	node = LookupNode(graph,nodeID);	 
  return node;
}

/* Finds shortest path distance between two nodes in an undirected graph with no edge weights
 * startID: the id of the start node
 * endID: the id of the end node
 * g: the graph
 * return value: the shortest path */
int reachNode1(int startID, int endID, const Graph* const g) {
    
	Graph* graph = (Graph*)g; 
   return ReachNode1(startID , endID ,graph);
}

/* Finds shortest path distance between all reachable nodes from a given node
 * in an undirected graph with no edge weights
 * startID: the id of the start node
 * endID: the id of the end node
 * g: the graph
 * return value: a resultSet */
ResultSet* reachNodeN(int startID, const Graph* const g) {
    
 	Graph* graph = (Graph*)g; 	
	ResultSet *res;
	res = ReachNodeN(startID,graph);
   return res;
}




