#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"
#include "GraphLib.h"
#include "graph.h"
#include "hashing.h"
#include "Statistics.h"
#include "Parts.h"
#include "CHECKS.h"
#include "Thread_Utilities.h"

///////////////////////////////////////////////PART1//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Part1(){

	int m = 2;
	int c = 3;

	Graph* g = createGraph(m, c);

	Insert_Persons(g,"dataset1/persons.txt");
 	Insert_Edges(g,"dataset1/edges.txt");

	Node* nl1 = lookupNode(12, g);
	printPersonProperties(nl1);
	Node* nl2 = lookupNode(16, g);
	printPersonProperties(nl2);

	int spt1 = reachNode1(1, 12, g);
	CHECK("Shortest path between nodes (1,12)", spt1, 5);

	int spt2 = reachNode1(14, 14, g);
	CHECK("Shortest path between nodes (14,14)", spt2, 0);

	int spt3 = reachNode1(3, 16, g);
	CHECK("Shortest path between nodes (3,16)", spt3, 3);

	int spt4 = reachNode1(5, 3, g);
	CHECK("Shortest path between nodes (5,3)", spt4, INFINITY_REACH_NODE);

	ResultSet *res = ReachNodeN(1,g);

    Pair results[10] = {
        {2, 1},
        {6, 1},
        {4, 1},
        {3, 2},
        {7, 2},
        {8, 2},
        {10, 3},
        {14, 4},
        {16, 5},
        {12, 5}
    };

    Pair pair;
    int k;
    int counter = 0;
    while (next(res, &pair)) {
        ++counter;
        for (k = 0; k < 10; ++k) {
            if (results[k].ID == pair.ID) {
                if (results[k].distance == pair.distance) {
                    printf("Shortest path between nodes (%d,%d): Success\n", 1, results[k].ID);
                } else {
                    printf("Shortest path between nodes (%d,%d): Failed | actual = %3d, expected = %3d\n", 1, results[k].ID, pair.distance, results[k].distance);
                }
                break;
            }
        }
        if (k == 10) {
            printf("ReachNodeN : Failed | Your returned an extra Pair(%d,%d) ", pair.ID, pair.distance);
        }
    }
    CHECK("Number of pairs in the ResultSet", counter, 10);
   

	Destroy_ResultSet(res);

	DestroyGraph(g);
}
///////////////////////////////////////////////PART2//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Part2(){

	int i=0,j=0;
	int m=2,c=3;
	Node *node;
	Node *repr,*n; 
	Graph* g = createGraph(m,c);	
	Insert_Persons(g,"dataset2/person.csv");

	Insert_Tables(g,2);//eisagwgh twn tables

	Insert_Edges(g,"dataset2/person_knows_person.csv");

	CGLNode *cur;
	cur = g->CGL->head;
	
	while(cur!=NULL){
		repr = LookupNode(g,cur->Repr_node_id);
		repr->Graph_ID = repr->id;	
		IncreaseNodesofCG(g->CGL,repr->id,repr);
		Fix_graph_Members(g,cur->Repr_node_id,repr);

		cur = cur->next;
	}	


//////////////////STATISTICS//////////////////////////////////
	printf("*********************STATISTICS TEST*******************************************\n");
	int diam;
	diam = diameter(g); 
	printf("Diameter of graph %d\n",diam);
	CHECKINT("Graph diameter", diam, 14);
	
	//degreeDistribution(g);

	double avgPthLgth = averagePathLength(g);
	printf("Average path length %.4lf\n",avgPthLgth);
	CHECKDOUBLE("Graph average path length", avgPthLgth,5.0322);

	int ccNumber = numberOfCCs(g);
	printf("Number of coherent graphs: %d\n",ccNumber);
	CHECKINT("Graph number of components ", ccNumber, 1);
	
	int maximumCC = maxCC(g);
	printf("Number of nodes of greater coherent graph: %d\n",maximumCC);
	CHECKINT("Graph maximum connected component ", maximumCC, 111);
	
   double dense = density(g);
	printf("Density %.3lf\n",density(g));
	CHECKDOUBLE("Graph density ", dense, 0.073);
	printf("******************************************************************************\n");
/////////////////////////////////closenessCentrality////////
	printf("***********************CLOSENESS CENTRALITY TEST******************************\n");
	
	printf("------------small graph test--------\n");
	testClosenessCentrality(m,c);
	printf("------------------------------------\n");

	printf("------------main graph test--------\n");
   int closenessIds[5] = {1734, 38, 8899, 3501, 75};
   float closenessIdsRes[5] = {0.3430,0.3439,0.3147,0.2667, 0.1594};
   for (i = 0; i < 5; ++i) {
        int nodeID = closenessIds[i];
        node = lookupNode(nodeID, g);
        double closCentrty = closenessCentrality(node, g);
		  printf("closeness centrality of node %d: %.4lf\n",node->id,closCentrty);		 
        CHECKDOUBLE("Graph closeness centrality ", closCentrty, closenessIdsRes[i]);
   }
	printf("------------------------------------\n");
	printf("******************************************************************************\n");
////////////////////////////BetweenessCentrality///////////////
	printf("***********************BETWEENNESS CENTRALITY TEST****************************\n");

 	printf("------------small graph test--------\n");
	testBetweennessCentrality(m,c);
	printf("------------------------------------\n");


	printf("------------main graph test--------\n");
	int betweennessIds[5] = { 1734,   38,     8899,   9900,   75};
   float betweennessIdsRes[5] = {0.306,  0.053,  0.018,  0.005,  0.000};

   for (i = 0; i < 5; ++i) {
        int nodeID = betweennessIds[i];
        node = lookupNode(nodeID, g);
        double betwCentrty = betweennessCentrality(node, g);
		  printf("betweenness centrality of node %d: %.3lf\n",node->id,betwCentrty);
        CHECKDOUBLE("Graph betweenness centrality ", betwCentrty, betweennessIdsRes[i]);
   }
	printf("------------------------------------\n");
	printf("**********************************************************************************\n");			


///////////////////////////QUERY1/////////////////////////////////
	printf("****************************QUERY1 TEST*******************************************\n");
	Matches *match;	
		
	//num_of_common_interest , h num of steps, x age diffrence, limit max matches
   Node *dateNode = lookupNode(3755, g);


  int commonInterests = 1, ageDiff = 30, acquaintanceHops = 3, matchesNum = 1;
  match = matchSuggestion(dateNode, commonInterests, acquaintanceHops, ageDiff, matchesNum, g);

	int id1 = getMatch(0, match);
	CHECKINT("Query 1: Date result 1st id", id1, 7107);
	 
	free(match->mArray);
	free(match);
			
	printf("**********************************************************************************\n");				


///////////////////////////////////////////////////////////////////
////////////////////////QUERY2////////////////////////////////////
	printf("****************************QUERY2 TEST*******************************************\n");
	Graph *Stalker_graph;
	struct Stalkers *s;	
	int stalkersNum = 7, likesNumber = 1, centralityMode = 1;
	s = malloc(sizeof(Stalkers)*stalkersNum);
	for(i=0;i<stalkersNum;i++)
		s[i].id = -1;
	Stalker_graph = getTopStalkers(stalkersNum,likesNumber,centralityMode,g,s);
	//Results 347, 495, 7768, 8354???, 8403, 8899, 9633
	for(j=0;j<stalkersNum;j++){
		if(s[j].id!=-1){					
			node = LookupNode(g,s[j].id);
			if(centralityMode == 1)
				printf("Stalker %d centrality_score %.3lf/%.3lf\n",s[j].id,s[j].centrality_score,closenessCentrality(node,g));			
			else
				printf("Stalker %d centrality_score %.3lf/%.3lf\n",s[j].id,s[j].centrality_score,betweennessCentrality(node,g));
			
		}

	}	
	//stalkersGraphRunMetrics(Stalker_graph);
	free(s);
	DestroyGraph(Stalker_graph);
	printf("**********************************************************************************\n");
///////////////////////////////////////////////////////////////////
////////////////////////QUERY3////////////////////////////////////
	printf("****************************QUERY3 TEST*******************************************\n");

	char **womenTrends,**menTrends;
	int trendsNum = 4;

	womenTrends = malloc(sizeof(char*)*trendsNum);
	menTrends = malloc(sizeof(char*)*trendsNum);
	for(j=0;j<trendsNum;j++){		
		womenTrends[j] = malloc(sizeof(char)*MAX_STRING_LENGTH);
		menTrends[j] = malloc(sizeof(char)*MAX_STRING_LENGTH);
	}


	findTrends(trendsNum,g,womenTrends,menTrends);
	CHECK_TRENDS(menTrends,womenTrends);
	printf("------->Men Trends\n");
	for(i=0;i<trendsNum;i++){
		if(menTrends[i]!=NULL)
			printf("%s\n",menTrends[i]);
	}

	printf("--------------------------\n");
	printf("------->Women Trends\n");
	for(i=0;i<trendsNum;i++){
		if(womenTrends[i]!=NULL)
			printf("%s\n",womenTrends[i]);
	}
	
	for(j=0;j<trendsNum;j++){		
		free(womenTrends[j]); 
		free(menTrends[j]);
	}
	free(womenTrends);
	free(menTrends);
	printf("**********************************************************************************\n");
///////////////////////////////////////////////////////////////////
////////////////////////QUERY4////////////////////////////////////
	printf("****************************QUERY4 TEST*******************************************\n");
	
	printf("------------small graph test--------\n");
	testTidalTrust(m,c); 
	printf("------------------------------------\n");
	int forumID = 34680;
	Graph *trustGraph;
	trustGraph = buildTrustGraph(forumID,g);
	int trustANodeId = 30;
   int trustBNodeId = 9805;
   int trustCNodeId = 9700;
   Node *ta = lookupNode(trustANodeId, trustGraph);
   Node *tb = lookupNode(trustBNodeId, trustGraph);
   Node *tc = lookupNode(trustCNodeId, trustGraph);

   double trustAB;
   trustAB = estimateTrust(ta, tb, trustGraph);
   printf("Trust between nodes (%d,%d) is %f\n", trustANodeId, trustBNodeId, trustAB);
   CHECKDOUBLE("Trust: (30,9805) ", trustAB, 0.134);

   double trustAC;
   trustAC = estimateTrust(ta, tc, trustGraph);
   printf("Trust between nodes (%d,%d) is %.2f\n", trustANodeId, trustCNodeId, trustAC);	
	CHECKDOUBLE("Trust: (30,9700) ", trustAC, 0.15);

	DestroyGraph(trustGraph);

	printf("**********************************************************************************\n");


	

	DestroyGraph(g);


}



///////////////////////////////////////////////PART3//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Part3(){
	Return_NForums_Graphs(NULL , 0);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////















