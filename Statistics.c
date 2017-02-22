#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"
#include "GraphLib.h"
#include "graph.h"
#include "hashing.h"
#include "Statistics.h"
#include "CHECKS.h"


void degreeDistribution(Graph *g){



	int i=0;
	Node *node;
	DegreePair *DegreeArray;
	DegreeArray = malloc(sizeof(DegreePair)*(g->max_edges+1));
	for(i=0;i<g->max_edges +1;i++){
		DegreeArray[i].node_num =0;
		DegreeArray[i].Degree =i;
		DegreeArray[i].Deg_value=0;
	}
	CGLNode *c = g->CGL->head;
	while(c!=NULL){

		if(c->node_num == 0){//ean dn exei komvous exei Degree 0		
		
			DegreeArray[0].node_num++;
			DegreeArray[0].Deg_value=(double)DegreeArray[0].node_num/(double)g->total_nodes;
		}

		for(i=0;i<c->node_num;i++){	
		
			node = c->CGmembers[i].Member;
			
			if(node->E_List == NULL){//ean dn exei akmes exei Degree 0
				
				DegreeArray[0].node_num++;
				DegreeArray[0].Deg_value=(double)DegreeArray[0].node_num/(double)g->total_nodes;
				break;
			}
			DegreeArray[node->E_List->size].node_num++;
			DegreeArray[node->E_List->size].Deg_value=(double)DegreeArray[node->E_List->size].node_num/(double)g->total_nodes;
			
		}

		c = c->next;
	}
	FILE *fp;
	fp = fopen ("degree_dist.txt","w");
	for(i=0;i<g->max_edges+1;i++)
		fprintf(fp,"%d %lf\n",DegreeArray[i].Degree,DegreeArray[i].Deg_value);
	
	fclose(fp);
	Plot(g->max_edges+1);
	
	free(DegreeArray);

}

int numberOfCCs(Graph *g){

	return (g->CGL->size);

} 

int maxCC(Graph *g){


	return (g->CGL->max_nodes_of_CG);


}

int diameter(Graph *g){

	int i=0,j=0;
	int d;
	int diam = -1;
	//int st,end;
	int m =2,c1=3;
	TNode t_node;
	Cell *matched;
	matched = Create_T_Hash_table(m,c1);

	CGLNode *c = g->CGL->head;
	Node *nodeStart,*nodeEnd;
	while(c!=NULL){
		
		for(i=0;i<c->node_num;i++){
			nodeStart = c->CGmembers[i].Member;
			t_node.id = nodeStart->id;
			T_InsertNode(&t_node,matched);

			for(j=0;j<c->node_num;j++){
					nodeEnd = c->CGmembers[j].Member;	
					if(nodeEnd->id != nodeStart->id && T_Search(matched,nodeEnd->id)==NULL){		
					 d = ReachNode1(nodeStart->id, nodeEnd->id,g);
						if(d>diam){
							diam = d;
							//st = nodeStart->id;
							//end = nodeEnd->id;
						}


					}
			}
			
			
		}
		c = c->next;
	}

	for(j=0;j<matched->info.hash_table_size;j++)
			free(matched->bucket[j].t_node); 
	
	free(matched->bucket);
	free(matched);



	//printf("from %d to %d\n",st,end);
	return diam;
}

double density(Graph* g){

	double dens;

	dens = (double)(2*g->total_edges)/(double)((g->total_nodes)*(g->total_nodes-1));

	return dens;

}

double averagePathLength(Graph * g){

	int i=0,j=0;
	int total = 0;
	int m=2,c1=3;
	double av_path_len;
	TNode t_node;
	Cell *matched;
	matched = Create_T_Hash_table(m,c1);
	CGLNode *c = g->CGL->head;
	Node *nodeStart,*nodeEnd;
	while(c!=NULL){
		
		for(i=0;i<c->node_num;i++){
			nodeStart = c->CGmembers[i].Member;
			t_node.id = nodeStart->id;
			T_InsertNode(&t_node,matched);
			for(j=0;j<c->node_num;j++){
					nodeEnd = c->CGmembers[j].Member;		
					if(nodeEnd->id != nodeStart->id && T_Search(matched,nodeEnd->id)==NULL)		
					 total = total + ReachNode1(nodeStart->id, nodeEnd->id,g);					
					 
			}
			
			
		}
		c = c->next;
	}

	//printf("total nodes %d\n",g->total_nodes);

	for(j=0;j<matched->info.hash_table_size;j++)
			free(matched->bucket[j].t_node); 
	
	free(matched->bucket);
	free(matched);





	av_path_len = (double)(total * 2) /(double) ( (g->total_nodes) * (g->total_nodes - 1) );
	return av_path_len;


}

double closenessCentrality(Node* n, Graph* g){

	int i=0;
	double total_d=0;
	int found =0;

	double clCent =0;


	CGLNode *c = g->CGL->head;
	Node *nodeEnd;

	while(c!=NULL){
		
		if(n->Graph_ID == c->Repr_node_id){
			found =1;
			break;
		}
			c = c->next;

	}		
	if(found ==1){
		for(i=0;i<c->node_num;i++){
				nodeEnd = c->CGmembers[i].Member;	
				if(nodeEnd->id != n->id)	
					total_d = total_d +1/(double)ReachNode1(n->id, nodeEnd->id,g);
									
		}
	}		
					
		
	clCent = (double)total_d /(double)(g->total_nodes -1);
	return clCent;


}

double betweennessCentrality(Node* n, Graph* g){


	int m =2,c1=3;	
	int i=0,j=0,k=0;	 
	int s_path;
  	int total_b_num=0;		 
  	double bet_cent=0,cb=0;
	int bridge_id = n->id;

	TNode tn,*t;

	Node *nodeStart,*nodeEnd;
	CGLNode *c;
	Stack *stack;	
  	Initialize_Stack(&stack);
	struct Cell *matched;
	matched = Create_T_Hash_table(m,c1);

	

  	c = g->CGL->head;	
  	while(c!=NULL){		
		if(n->Graph_ID == c->Repr_node_id)
			break;		 
		 c = c->next;
	}	



	for(i=0;i<c->node_num;i++){	
		nodeStart = c->CGmembers[i].Member;

		tn.id = nodeStart->id;
		T_InsertNode(&tn,matched);
		nodeStart->level =0;
		Insert_Levels(nodeStart,g);
		for(j=0;j<c->node_num;j++){
			nodeEnd = c->CGmembers[j].Member;

			if(T_Search(matched,nodeEnd->id)==NULL && nodeEnd->id!=bridge_id && nodeStart->id !=bridge_id){
				
				s_path = ReachNode1(nodeStart->id,nodeEnd->id,g);
								
				find_All_possible_paths(stack,nodeStart,nodeEnd,s_path,bridge_id);
		
				total_b_num = total_b_num+stack->between_num;
									
				cb = cb + (double)total_b_num/(double)stack->total_s_path;
				total_b_num=0;
				stack->total_s_path=0;
				stack->between_num = 0;
				
			}


		}

	}	
	

	for(i=0;i<matched->info.hash_table_size;i++)
		free(matched->bucket[i].t_node); 
	
	free(matched->bucket);
	free(matched);

	//Normalize
	double x = (double)((g->total_nodes -1)*(g->total_nodes -2)) / (double)2;	
	bet_cent = cb/x;
	//printf("CB %lf\n",cb);
	free(stack);
	return bet_cent;

} 

void find_All_possible_paths(Stack *s,Node *startNode,Node *endNode,int s_path,int brID){


	int i=0;
	int found =0;
	StackNode *c;
	Node *current = startNode;
	Edge *cur_e;

	if(s_path==1){
		s->total_s_path++;		
		return;	
	}




	if(s->size > s_path)
		return;
	

	if(startNode->id == endNode->id && s->size == s_path){
		
		s->total_s_path++;
		c = s->head;

		while(c!=NULL){
						
			if(c->node->id == brID)
				s->between_num++;									
			c = c->down;
		}
		

	}else  {
				current->visited = 1;

				Insert_to_Stack(s,current);

				cur_e = current->E_List->head;

				for(i=0;i<current->E_List->size;i++){
					if(cur_e->n_node->visited == 0 && cur_e->n_node->level > current->level)						
						find_All_possible_paths(s,cur_e->n_node,endNode,s_path,brID);																

					cur_e = cur_e->next;						
				}

			 current->visited =0;
			 Remove_from_Stack(s);
	}


}

void Initialize_Stack(Stack **s){

	Stack *S;
	S = malloc(sizeof(struct Stack));
	S->size = 0;
	S->head = NULL;
	S->between_num = 0;
	S->total_s_path = 0;
	*s = S;


}

void Remove_from_Stack(Stack *s){

	s->size--;

	StackNode *tmp;
	tmp = s->head;
	if(s->head->down!=NULL)
		s->head = s->head->down;

	free(tmp);

	if(s->size == 0){
		s->head =NULL;
		return;
	}
		

} 

void Insert_to_Stack(Stack *s,Node *node){

	//printf("inserting %d\n",node->id);
	StackNode *newSNode,*tmp; 
	newSNode = malloc(sizeof(struct StackNode));
	newSNode->node = node;

	newSNode->up = NULL;
	newSNode->down = NULL;
		
	if(s->head == NULL)
		s->head =  newSNode; 
	else{
		
		tmp = s->head;
		s->head = newSNode;
		s->head->down = tmp;
		tmp->up = s->head;				
  }
	s->size++;

}

void Plot(int size){

	int NUM_COMMANDS = 2;
	int i=0,x;
	int *xvals;
   double *yvals,y;	
	xvals = malloc(sizeof(int)*size);
	yvals = malloc(sizeof(double)*size);
	FILE *fp;
	fp = fopen("degree_dist.txt","r");
	while(fscanf(fp,"%d %lf",&x,&y)!= EOF){
		xvals[i] = x;
		yvals[i] = y;
		i++;
	}

	char *commandsForGnuplot[] = {"set title \"DEGREE DISTRIBUTION\"", "plot 'data.temp'"};
	FILE *gnuplotPipe = popen ("gnuplot -persistent", "w");
	FILE *temp = fopen("data.temp", "w");
	for (i=0; i < size; i++)    
 	   fprintf(temp, "%d %lf \n", xvals[i], yvals[i]); //Write the data to a temporary file

	for (i=0; i < NUM_COMMANDS; i++)    
    	fprintf(gnuplotPipe, "%s \n", commandsForGnuplot[i]); //Send commands to gnuplot one by one.


	fclose(fp);
	remove("degree_dist.txt");
	free(xvals);
	free(yvals);

}

void testBetweennessCentrality(int bucketsNumber, int bucketSize){
    //create small graph for testing betweenness Centrality
    Graph* gBetw = createGraph(bucketsNumber, bucketSize);

    Node* n1Betw = createNode(1, NULL);
    Node* n2Betw = createNode(2, NULL);
    Node* n3Betw = createNode(3, NULL);
    Node* n4Betw = createNode(4, NULL);
    Node* n5Betw = createNode(5, NULL);

    insertNode(n1Betw, gBetw);
    insertNode(n2Betw, gBetw);
    insertNode(n3Betw, gBetw);
    insertNode(n4Betw, gBetw);
    insertNode(n5Betw, gBetw);

    /* Create edges and set properties */
    Edge* e1Betw = createEdge(1, 2, NULL);
    Edge* e2Betw = createEdge(2, 1, NULL);
    Edge* e3Betw = createEdge(2, 3, NULL);
    Edge* e4Betw = createEdge(2, 4, NULL);
    Edge* e5Betw = createEdge(3, 2, NULL);
    Edge* e6Betw = createEdge(3, 5, NULL);
    Edge* e7Betw = createEdge(4, 2, NULL);
    Edge* e8Betw = createEdge(4, 5, NULL);
    Edge* e9Betw = createEdge(5, 3, NULL);
    Edge* e10Betw = createEdge(5, 4, NULL);

    /* Insert edges in graph */
    insertEdge(1, e1Betw, gBetw);
    insertEdge(2, e2Betw, gBetw);
    insertEdge(2, e3Betw, gBetw);
    insertEdge(2, e4Betw, gBetw);
    insertEdge(3, e5Betw, gBetw);
    insertEdge(3, e6Betw, gBetw);
    insertEdge(4, e7Betw, gBetw);
    insertEdge(4, e8Betw, gBetw);
    insertEdge(5, e9Betw, gBetw);
    insertEdge(5, e10Betw, gBetw);

	 CGLNode *cur;
	 cur = gBetw->CGL->head;
	 Node *repr;	
	 while(cur!=NULL){
		repr = LookupNode(gBetw,cur->Repr_node_id);
		repr->Graph_ID = repr->id;	
		IncreaseNodesofCG(gBetw->CGL,repr->id,repr);
		Fix_graph_Members(gBetw,cur->Repr_node_id,repr);

		cur = cur->next;
	}




    double betwCentrty1 = betweennessCentrality(n1Betw, gBetw);
    CHECKDOUBLE("Small Graph betweenness centrality node:1 ", betwCentrty1, 0.0 / 6.0);

    double betwCentrty2 = betweennessCentrality(n2Betw, gBetw);
    CHECKDOUBLE("Small Graph betweenness centrality node:2 ", betwCentrty2, 3.5 / 6.0);

    double betwCentrty3 = betweennessCentrality(n3Betw, gBetw);
    CHECKDOUBLE("Small Graph betweenness centrality node:3 ", betwCentrty3, 1.0 / 6.0);

    double betwCentrty4 = betweennessCentrality(n4Betw, gBetw);
    CHECKDOUBLE("Small Graph betweenness centrality node:4 ", betwCentrty4, 1.0 / 6.0);

    double betwCentrty5 = betweennessCentrality(n5Betw, gBetw);
    CHECKDOUBLE("Small Graph betweenness centrality node:5 ", betwCentrty5, 0.5 / 6.0);

	 DestroyGraph(gBetw);	
}

void testClosenessCentrality(int bucketsNumber, int bucketSize){
    //create small graph for testing betweenness Centrality
    Graph* gClos = createGraph(bucketsNumber, bucketSize);

    Node* n1Clos = createNode(1, NULL);
    Node* n2Clos = createNode(2, NULL);
    Node* n3Clos = createNode(3, NULL);
    Node* n4Clos = createNode(4, NULL);
    Node* n5Clos = createNode(5, NULL);
    Node* n6Clos = createNode(6, NULL);
    Node* n7Clos = createNode(7, NULL);

    insertNode(n1Clos, gClos);
    insertNode(n2Clos, gClos);
    insertNode(n3Clos, gClos);
    insertNode(n4Clos, gClos);
    insertNode(n5Clos, gClos);
    insertNode(n6Clos, gClos);
    insertNode(n7Clos, gClos);

    /* Create edges and set properties */
    Edge* e1Clos = createEdge(1, 2, NULL);
    Edge* e2Clos = createEdge(1, 3, NULL);
    Edge* e3Clos = createEdge(2, 1, NULL);
    Edge* e4Clos = createEdge(2, 3, NULL);
    Edge* e5Clos = createEdge(3, 1, NULL);
    Edge* e6Clos = createEdge(3, 2, NULL);
    Edge* e7Clos = createEdge(3, 4, NULL);
    Edge* e8Clos = createEdge(4, 3, NULL);
    Edge* e9Clos = createEdge(4, 5, NULL);
    Edge* e10Clos = createEdge(5, 4, NULL);
    Edge* e11Clos = createEdge(5, 6, NULL);
    Edge* e12Clos = createEdge(5, 7, NULL);
    Edge* e13Clos = createEdge(6, 5, NULL);
    Edge* e14Clos = createEdge(6, 7, NULL);
    Edge* e15Clos = createEdge(7, 5, NULL);
    Edge* e16Clos = createEdge(7, 6, NULL);


     /* Insert edges in graph */
    insertEdge(1, e1Clos, gClos);
    insertEdge(1, e2Clos, gClos);
    insertEdge(2, e3Clos, gClos);
    insertEdge(2, e4Clos, gClos);
    insertEdge(3, e5Clos, gClos);
    insertEdge(3, e6Clos, gClos);
    insertEdge(3, e7Clos, gClos);
    insertEdge(4, e8Clos, gClos);
    insertEdge(4, e9Clos, gClos);
    insertEdge(5, e10Clos, gClos);
    insertEdge(5, e11Clos, gClos);
    insertEdge(5, e12Clos, gClos);
    insertEdge(6, e13Clos, gClos);
    insertEdge(6, e14Clos, gClos);
    insertEdge(7, e15Clos, gClos);
    insertEdge(7, e16Clos, gClos);
    

	 CGLNode *cur;
	 cur = gClos->CGL->head;
	 Node *repr;	
	 while(cur!=NULL){
		repr = LookupNode(gClos,cur->Repr_node_id);
		repr->Graph_ID = repr->id;	
		IncreaseNodesofCG(gClos->CGL,repr->id,repr);
		Fix_graph_Members(gClos,cur->Repr_node_id,repr);

		cur = cur->next;
	 }

    double closCentrty1 = closenessCentrality(n1Clos, gClos);
    CHECKDOUBLE("Small Graph closeness centrality node:1 ", closCentrty1, 3.33 / 6.0);

    double closCentrty2 = closenessCentrality(n2Clos, gClos);
    CHECKDOUBLE("Small Graph closeness centrality node:2 ", closCentrty2, 3.33 / 6.0);

    double closCentrty3 = closenessCentrality(n3Clos, gClos);
    CHECKDOUBLE("Small Graph closeness centrality node:3 ", closCentrty3, 4.16 / 6.0);

    double closCentrty4 = closenessCentrality(n4Clos, gClos);
    CHECKDOUBLE("Small Graph closeness centrality node:4 ", closCentrty4, 4.0 / 6.0);

    double closCentrty5 = closenessCentrality(n5Clos, gClos);
    CHECKDOUBLE("Small Graph closeness centrality node:5 ", closCentrty5, 4.16 / 6.0);

    double closCentrty6 = closenessCentrality(n6Clos, gClos);
    CHECKDOUBLE("Small Graph closeness centrality node:6 ", closCentrty6, 3.33 / 6.0);

    double closCentrty7 = closenessCentrality(n7Clos, gClos);
    CHECKDOUBLE("Small Graph closeness centrality node:7 ", closCentrty7, 3.33 / 6.0);

	DestroyGraph(gClos);
}

void stalkersGraphRunMetrics(Graph *gStalkers){

    printf("Stalkers graph executing metrics:\n");
    degreeDistribution(gStalkers);

    int diam = diameter(gStalkers);
	 printf("Stalkers graph diameter is %d\n",diam);
 
	 double avgPthLgth = averagePathLength(gStalkers);
	 printf("Stalkers graph Average path length is %.3lf\n",avgPthLgth);	

    int ccNumber = numberOfCCs(gStalkers);
	 printf("Stalkers graph coherent graphs %d\n",ccNumber);

    int maximumCC = maxCC(gStalkers);
	 printf("Stalkers graph Max coherent graph has %d nodes\n",maximumCC);

    double dense = density(gStalkers);
	 printf("Stalkers density is %.3lf\n",dense);


}


