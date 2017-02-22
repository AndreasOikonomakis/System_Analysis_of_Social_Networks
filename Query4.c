#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"graph.h"
#include"hashing.h"
#include"defines.h"
#include"Queries.h"
#include "CHECKS.h"

#define EDGE_PROPERTIES_NUM 2

Graph* buildTrustGraph(int forumID, Graph* g){

	int i=0,j=0;
	int m =2,c=3;
	Edge *Trust_Edge;
	Graph *trustGraph = createGraph(m, c);
	FNode *forum;
	Node *cur_member,*input_node;
	Edge *cur_e;
	TNode *tn,*tn1,*tn2,*t1,*t2,*tn11,*tn22;
	double Trust=0;
	double like_factor=0;//paragontas twn like	
	double reply_factor=0;//paragontas twn reply
	like_counter *lc,*m_lc;
	reply_counter *rc,*m_rc;

	printf("building trust Graph for forum %d\n",forumID);
	tn = T_Search(g->h_FORUMS,forumID);





	if(tn!=NULL){
		forum = tn->FNptr;

		for(i=0;i<forum->Forum_Members->size;i++){
					
			cur_member = forum->Forum_Members->GNodeArray[i];
			input_node =  createNode(cur_member->id, NULL);					
			insertNode(input_node, trustGraph);			
		}




		for(i=0;i<forum->Forum_Members->size;i++){
					
			cur_member = forum->Forum_Members->GNodeArray[i];


			cur_e = cur_member->E_List->head;
			for(j=0;j<cur_member->E_List->size;j++){
				
				tn1 = T_Search(tn->FNptr->likes_to_from,cur_e->n_node->id);
				tn2 = T_Search(tn->FNptr->replies_to_from,cur_e->n_node->id);


				if(tn1!=NULL && tn2!=NULL){			
			
					//printf("%d-%d\n",cur_member->id,cur_e->n_node->id);
					t1=NULL;
					t2=NULL;					

						
					t1 = T_Search(tn->FNptr->likes_to_from,cur_member->id);
					t2 = T_Search(tn->FNptr->replies_to_from,cur_member->id);
					m_lc = (like_counter*)t1->data;
					m_rc = (reply_counter*)t2->data;

					


					if(tn1!=NULL && tn1->data!=NULL &&tn1->Hptr!=NULL){
				
						tn11 = T_Search(tn1->Hptr,cur_member->id);
						//printf("o %d exei kanei %d likes genika\n",cur_member->id,m_lc->likes);
						if(m_lc->likes !=0 &&tn11!=NULL ){
							lc = tn11->data;
							like_factor = ((double)lc->likes/(double)m_lc->likes)*0.3;
							//printf("o %d kanei %d likes ston %d\n",cur_member->id,lc->likes,cur_e->n_node->id);
							//printf("o %d exei kanei %d likes genika\n",cur_member->id,m_lc->likes);
							//getchar();
						}
		
					}

					if(tn2!=NULL && tn2->data!=NULL &&tn2->Hptr!=NULL){
						tn22 = T_Search(tn2->Hptr,cur_member->id);
						//printf("o %d exei kanei %d replies genika\n",cur_member->id,m_rc->replies);
						if(m_rc->replies !=0 &&tn22!=NULL){
							rc = tn22->data;
							reply_factor = ((double)rc->replies/(double)m_rc->replies)*0.7;
							//printf("o %d kanei %d replies ston %d\n",cur_member->id,rc->replies,cur_e->n_node->id);
							//printf("o %d exei kanei %d replies genika\n",cur_member->id,m_rc->replies);
							//getchar();
						}
					}
					
					Trust = like_factor + reply_factor;

					if(Trust!=0){			
						Trust_Edge = setEdgeTrustProperties(cur_member->id, cur_e->n_node->id,Trust);
						insertEdge(cur_member->id, Trust_Edge, trustGraph);
						//printf("%d-%d Trust %.3lf\n",cur_member->id,cur_e->n_node->id,Trust);
						//getchar();
					}

					Trust=0;
					like_factor=0;	
					reply_factor=0;

				

				}


				cur_e = cur_e->next;
			}
		}

	}

	return trustGraph;
} 


Edge* setEdgeTrustProperties(int startNodeID, int endNodeID,double weight) {

    /*create edge properties*/
    Properties* propEdge = createProperties(EDGE_PROPERTIES_NUM);
    setStringProperty("null", 0, propEdge);
    setfloatProperty(weight, 1, propEdge);

    /*create an edge*/
    Edge* e = createEdge(startNodeID, endNodeID, propEdge);
    return e;
}

double estimateTrust(Node* a, Node* b, Graph* trustGraph){


	int i=0;
	int m =2,c=3;
	Edge *cur_e;
	int max_level;
	double Trust;
	Cell *visited,*levels;
	Node *cur_node;
	visited = Create_T_Hash_table(m,c);
	levels = Create_T_Hash_table(m,c);
	
	VQueue *Vqueue;
	Initialize_VQueue(&Vqueue,a->id,0);
	
	TNode t_node;

	max_level = ReachNode12(a->id,b->id,trustGraph);
	//printf("max_level %d\n",max_level);
	

	a->level =0;	
	t_node.id = a->id;
	T_InsertNode(&t_node,levels);


	VLink(a,Vqueue,NULL);
	
	
	
	while(Vqueue->size!=0){
			

		cur_node = Vqueue->head->node;
		//printf("Episkeptomai ton %d\n",cur_node->id);
		RemoveVQueue(Vqueue);	
		
		t_node.id = cur_node->id;
		T_InsertNode(&t_node,visited);

		cur_e = cur_node->E_List->head;
		for(i=0;i<cur_node->E_List->size;i++){
			
			if(T_Search(levels,cur_e->n_node->id)==NULL){
				t_node.id = cur_e->n_node->id;
				
				cur_e->n_node->level = cur_node->level +1;

				T_InsertNode(&t_node,levels);
			}

			if(T_Search(visited,cur_e->n_node->id)==NULL && cur_e->n_node->level > cur_node->level && cur_e->n_node->level<= max_level){
				if(cur_e->n_node->E_List!=NULL)
					VLink(cur_e->n_node,Vqueue,cur_node);

				if(cur_e->n_node->level == 1)
					cur_e->n_node->Trust = atof(cur_e->edge_prop->pArray[1]);
				else{
					
					//printf("%.2lf*%.2lf ston %d\n",atof(cur_e->edge_prop->pArray[1]),cur_node->Trust,cur_e->n_node->id);
					
					if(cur_e->n_node->Trust<atof(cur_e->edge_prop->pArray[1])*cur_node->Trust)						
						cur_e->n_node->Trust = atof(cur_e->edge_prop->pArray[1])*cur_node->Trust;
																		
				}
				
				//printf("id %d trust %.2lf apo %d\n",cur_e->n_node->id,cur_e->n_node->Trust,cur_node->id);	
				if(cur_e->n_node->id == b->id)
					Trust = cur_e->n_node->Trust;
			}	



			cur_e = cur_e->next;
		}

	}

	return Trust;
}

void testTidalTrust(int bucketsNumber, int bucketSize) {
    //create small graph for testing tidal's trust algorithm result
    Graph* gtt = createGraph(bucketsNumber, bucketSize);

    Node* n1tt = createNode(1, NULL);
    Node* n2tt = createNode(2, NULL);
    Node* n3tt = createNode(3, NULL);
    Node* n4tt = createNode(4, NULL);
    Node* n5tt = createNode(5, NULL);
    Node* n6tt = createNode(6, NULL);
    Node* n7tt = createNode(7, NULL);
    Node* n8tt = createNode(8, NULL);
    Node* n9tt = createNode(9, NULL);
    Node* n10tt = createNode(10, NULL);
    Node* n11tt = createNode(11, NULL);

    insertNode(n1tt, gtt);
    insertNode(n2tt, gtt);
    insertNode(n3tt, gtt);
    insertNode(n4tt, gtt);
    insertNode(n5tt, gtt);
    insertNode(n6tt, gtt);
    insertNode(n7tt, gtt);
    insertNode(n8tt, gtt);
    insertNode(n9tt, gtt);
    insertNode(n10tt, gtt);
    insertNode(n11tt, gtt);


    Edge* e1tt = setEdgeTrustProperties(1, 2, 1.0);
    Edge* e2tt = setEdgeTrustProperties(1, 5, 1.0);
    Edge* e3tt = setEdgeTrustProperties(2, 3, 0.9);
    Edge* e4tt = setEdgeTrustProperties(2, 4, 0.9);
    Edge* e5tt = setEdgeTrustProperties(3, 6, 0.8);
    Edge* e6tt = setEdgeTrustProperties(4, 6, 0.3);
    Edge* e7tt = setEdgeTrustProperties(4, 7, 0.9);
    Edge* e8tt = setEdgeTrustProperties(5, 10, 0.9);
    Edge* e9tt = setEdgeTrustProperties(6, 9, 1.0);
    Edge* e10tt = setEdgeTrustProperties(7, 8, 1.0);
    Edge* e11tt = setEdgeTrustProperties(8, 9, 1.0);
    Edge* e12tt = setEdgeTrustProperties(9, 11, 1.0);
    Edge* e13tt = setEdgeTrustProperties(10, 11, 0.4);

    /* Insert edges in graph */
    insertEdge(1, e1tt, gtt);
    insertEdge(1, e2tt, gtt);
    insertEdge(2, e3tt, gtt);
    insertEdge(2, e4tt, gtt);
    insertEdge(3, e5tt, gtt);
    insertEdge(4, e6tt, gtt);
    insertEdge(4, e7tt, gtt);
    insertEdge(5, e8tt, gtt);
    insertEdge(6, e9tt, gtt);
    insertEdge(7, e10tt, gtt);
    insertEdge(8, e11tt, gtt);
    insertEdge(9, e12tt, gtt);
    insertEdge(10, e13tt, gtt);

    Node *att = lookupNode(1, gtt);

    Node *btt = lookupNode(11, gtt);
    //Estimate trust(1,11)
    double trust1_11 = estimateTrust(att, btt, gtt);
	 printf("Graph estimate trust (1,11) %.2lf\n",trust1_11);	 
    CHECKDOUBLE("Graph estimate trust (1,11)", trust1_11, 0.36);

    //Estimate trust(1,9)
    Node *ctt = lookupNode(9, gtt);
    double trust1_9 = estimateTrust(att, ctt, gtt);
	 printf("Graph estimate trust (1,9) %.2lf\n",trust1_9);
    CHECKDOUBLE("Graph estimate trust (1,9)", trust1_9, 0.72);

}






