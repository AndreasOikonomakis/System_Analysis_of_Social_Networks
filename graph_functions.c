#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"graph.h"
#include"hashing.h"
#include"defines.h"
#include"GraphLib.h"
#include"Queries.h"

#define PERSON_PROPERTIES_NUM 4
#define EDGE_PROPERTIES_NUM 2

FNode *create_Forum_node(int id, char *name){

	int m=2,c=3;
	FNode *fn = malloc(sizeof(FNode));
	fn->id = id;
	fn->Forum_Members=NULL;
	fn->likes_to_from = Create_T_Hash_table(m,c);
	fn->replies_to_from = Create_T_Hash_table(m,c);
	strcpy(fn->name,name);


	return fn;

} 

void InsertFNodeList(struct FNodeList *list,struct FNode *fnode){

	FNode **f;
	(list->size)++;
	if(list->size > list->max_size){
		
		list->max_size = 2*(list->max_size);
		f = realloc(list->FArray,sizeof(struct FNode*)*list->max_size);
		list->FArray = f;
		list->FArray[list->size -1] = fnode;
	}
	else{		
		list->FArray[list->size -1] = fnode;
	}

}

FNode *FNL_Search(struct FNodeList *FNL ,int key){

	int first;
	int last;
	int middle;
	int place = -1;

	if(FNL->size > 0){

		first=0;
		last = FNL->size-1;
		while(first <= last){
			middle = (first + last)/2;
  
			if(FNL->FArray[middle]->id == key){
					place = middle;
					break;			
			}			
			else if (FNL->FArray[middle]->id > key)
				last = middle-1;			
			else
         	first = middle+1;      	
		}

	}

	if(place == -1)
		return NULL;
	else
		return FNL->FArray[place];

}




void InsertTNodeList(struct TNodeList *list,struct TNode *tnode){

	TNode **t;
	(list->size)++;
	if(list->size > list->max_size){
		
		list->max_size = 2*(list->max_size);
		t = realloc(list->TArray,sizeof(struct TNode*)*list->max_size);
		list->TArray = t;
		list->TArray[list->size -1] = tnode;
	}
	else{		
		list->TArray[list->size -1] = tnode;
	}


}

struct RNodeList *Create_RNodeList(){

	RNodeList *list;
	list = malloc(sizeof(RNodeList));
	list->size =0;
	list->max_size = 2;
	list->RArray = malloc(sizeof(RNode*)*list->max_size);
	return list;
}


void InsertRNodeList(RNodeList *list,RNode *rnode){

	RNode **r;
	(list->size)++;
	if(list->size > list->max_size){
		
		list->max_size = 2*(list->max_size);
		r = realloc(list->RArray,sizeof(RNode*)*list->max_size);
		list->RArray = r;
		list->RArray[list->size -1] = rnode;
	}
	else{		
		list->RArray[list->size -1] = rnode;
	}


}

struct GNodeList *Create_GNodeList(){

	GNodeList *list;
	list = malloc(sizeof(GNodeList));
	list->size =0;
	list->max_size = 2;
	list->GNodeArray = malloc(sizeof(Node*)*list->max_size);
	return list;
}

void InsertSGNodeList(GNodeList *list,struct Node *node){

	Node **n;
	(list->size)++;
	if(list->size > list->max_size){
		
		list->max_size = 2*(list->max_size);
		n = realloc(list->GNodeArray,sizeof(Node*)*list->max_size);
		list->GNodeArray = n;
		list->GNodeArray[list->size -1] = node;
	}
	else{		
		list->GNodeArray[list->size -1] = node;
	}


}



struct Node *GNL_Search(struct GNodeList *GNL ,int key){

	int first;
	int last;
	int middle;
	int place = -1;

	if(GNL->size > 0){

		first=0;
		last = GNL->size-1;
		while(first <= last){
			middle = (first + last)/2;
  
			if(GNL->GNodeArray[middle]->id == key){
					place = middle;
					break;			
			}			
			else if (GNL->GNodeArray[middle]->id > key)
				last = middle-1;			
			else
         	first = middle+1;      	
		}

	}

	if(place == -1)
		return NULL;
	else
		return GNL->GNodeArray[place];

}




//Dhmiourgia komvou
Node *GNode_Creation(int id,Properties *prop){

	int m=2;
	int c=3;
	Node *node;
	node = malloc(sizeof(struct Node));
	node->node_prop = prop;
	node->knows_hash_table = Create_T_Hash_table(m,c);
	node->came_from = NULL;//Dhlwnei apo pou prohl8e o komvos sto monopati/xrhsimopoietai gia to BFS 
	node->id = id;
	node->E_List = NULL;	
	node->MSG = NULL;
	node->WSG = NULL;
	strcpy(node->Year_work,"0");
	node->Stalker_likes = 0;
	node->visited = 0;//Xrhsimopoieitai sto BFS
	node->Graph_ID = -1;
	node->Graph_M_ID = -1;
	node->Graph_W_ID = -1;
	node->SGraph_ID = -1;
	node->likes = NULL;
	node->currently_working = NULL;
	node->working_with = NULL;
	node->studying_with = NULL;
	node->living_with = NULL;
	node->interests = NULL;
	node->posts = NULL;

	
	node->M_F = NULL;
	node->matched =0;
	node->Trust=0;

	return node;
}
//Dhmiourgia grafou
Graph *Graph_Create(int m , int c){

	struct Graph *gr;
	gr = malloc(sizeof(struct Graph));
	gr->hash_table = Create_T_Hash_table(m,c);
	gr->edge_hash = NULL;
	gr->total_nodes=0;
	gr->total_edges=0;
	gr->cur_bridgeID=-1;
	gr->betweeness_num=0;
	gr->max_edges = -1;
	gr->h_ORG = NULL;
	gr->h_INT = NULL;
	gr->h_PL = NULL;
	gr->h_POSTS = NULL;
	gr->h_FORUMS = NULL;
	gr->h_COM = NULL;

	gr->Forums = malloc(sizeof(FNodeList));
	gr->Forums->size =0;
	gr->Forums->cur=0;
	gr->Forums->max_size = 2;
	gr->Forums->FArray = malloc(sizeof(FNode*)*gr->Forums->max_size);	




	Initialize_CGL(&(gr->CGL));

	gr->M_graph = M_W_Graph_Create(m,c);//Dhmiourgia grafou andrwn
	gr->W_graph = M_W_Graph_Create(m,c);//Dhmiourgia grafou gunaikwn
	return gr;
}

//katastrofh grafou
void DestroyGraph(Graph *graph){

	int i=0,j=0;	

	Destroy_T_Hash_table(graph->hash_table);
	
	DestroyCGL(graph->CGL);
	
	if(graph->h_ORG!=NULL)	
		Destroy_T_Hash_table2(graph->h_ORG);
	if(graph->h_INT!=NULL)	
		Destroy_T_Hash_table2(graph->h_INT);
	if(graph->h_PL!=NULL)	
		Destroy_T_Hash_table2(graph->h_PL);
	if(graph->h_POSTS!=NULL)
		Destroy_T_Hash_table2(graph->h_POSTS);
	if(graph->h_COM!=NULL)
		Destroy_T_Hash_table2(graph->h_COM);

	if(graph->Forums!=NULL){
		free(graph->Forums->FArray);
		free(graph->Forums);	

	}

	//if(graph->h_FORUMS!=NULL)
		//Destroy_T_Hash_table3(graph->h_FORUMS);


		
	if(graph->M_graph!=NULL){
		for(i=0;i<graph->M_graph->hash_table->info.hash_table_size;i++)
			free(graph->M_graph->hash_table->bucket[i].t_node);
		free(graph->M_graph->hash_table->bucket);
		free(graph->M_graph->hash_table);
		DestroyCGL(graph->M_graph->CGL);
		free(graph->M_graph);
	}

	if(graph->W_graph!=NULL){
		for(i=0;i<graph->W_graph->hash_table->info.hash_table_size;i++)
			free(graph->W_graph->hash_table->bucket[i].t_node);
		free(graph->W_graph->hash_table->bucket);
		free(graph->W_graph->hash_table);
		DestroyCGL(graph->W_graph->CGL);
		free(graph->W_graph);

	}
	free(graph);

}

//arxhkopoihsh listas geitniashs
void Initialize_E_List(e_list **list,int o_id){

	e_list *LIST;
	LIST = malloc(sizeof(e_list));
	LIST->owner_id = o_id;
	LIST->size = 0;
	LIST->head = NULL;
	LIST->tail = NULL;
	*list = LIST;

}

//dhmiourgia akmhs
Edge *Create_Edge(int id , int end,Properties *prop){

	Edge *new_Edge;	
	new_Edge = malloc(sizeof(struct Edge));
	new_Edge->bet_num = 0;
	new_Edge->id = id;
	new_Edge->end = end;
	new_Edge->edge_prop = prop;	
	return new_Edge;
}

//ftiaxnei ta melh sunektikwn grafimatwn
void Fix_graph_Members(Graph *g,int repr_id,Node *cur_node){

	int i=0;
	Edge *e;
	if(cur_node->E_List!=NULL){
		e = cur_node->E_List->head;
		for(i=0;i<cur_node->E_List->size;i++){
			if(e->n_node->Graph_ID==-1){
				//printf("vazw ston %d repr_id to %d\n",e->n_node->id,repr_id);
				e->n_node->Graph_ID = repr_id;

				IncreaseNodesofCG(g->CGL,repr_id,e->n_node);
				RemoveCGL(g->CGL,e->n_node->id);
				Fix_graph_Members(g,repr_id,e->n_node);
				
			}
			e = e->next;
		}
	}
}



//Eisagwgh akmhs
void InsertEdge(int startID,Edge *new_Edge, Graph* graph){
	
	int n_id;
	int pos1,pos2;
	int place1,place2;
	int CGnode_id_start,CGnode_id_end;
	CGLNode *CG1,*CG2;

	Node *nodeS,*nodeE;//nodeStart,node End
	

	nodeS = LookupNode(graph,startID);
	nodeE = LookupNode(graph,new_Edge->end);

	
	new_Edge->n_node = nodeE;

	//Arxikopoihsh ths lista geitniashs otan mpainei h prwth akmh gia auton ton komvo	
	if(nodeS->E_List == NULL)
		Initialize_E_List(&nodeS->E_List,nodeS->id);

	TNode *t_node;
	//eisagwgh sthn lista geitniashs
	ELink(new_Edge,nodeS->E_List);
////////////////eisodos sto hash_table twn geitonwn/////////////////////
	t_node = TNode_Creation(nodeE->id);		
	t_node->Nptr = nodeE;
	T_InsertNode(t_node,nodeS->knows_hash_table);
	free(t_node);	
/////////////////////////////////////////////////////////////////////////////

	graph->total_edges++;

	if(graph->max_edges < nodeS->E_List->size)
		graph->max_edges = nodeS->E_List->size;
		

	if(nodeS->node_prop!=NULL && nodeE->node_prop!=NULL){
	//an oi duo komvoi pou sundeontai ston geniko grafo einai idiou filou kai andres
		if(!strcmp(nodeS->node_prop->pArray[3],nodeE->node_prop->pArray[3]) && !strcmp(nodeS->node_prop->pArray[3],"male")){

			//Arxikopoihsh listas geitonwn idiou filou
			if(nodeS->MSG == NULL){
				nodeS->MSG = malloc(sizeof(GNodeList));
				nodeS->MSG->size =0;
				nodeS->MSG->max_size = 2;
				nodeS->MSG->GNodeArray = malloc(sizeof(Node*)*nodeS->MSG->max_size);
			}
			InsertSGNodeList(nodeS->MSG,nodeE);//eisagwgh sth lista geitonwn idiou filou andrwn

		}		
	
		//an oi duo komvoi pou sundeontai ston geniko grafo einai idiou filou kai gunaikes
		if(!strcmp(nodeS->node_prop->pArray[3],nodeE->node_prop->pArray[3]) && !strcmp(nodeS->node_prop->pArray[3],"female")){

			//Arxikopoihsh listas geitonwn idiou filou
			if(nodeS->WSG == NULL){
				nodeS->WSG = malloc(sizeof(GNodeList));
				nodeS->WSG->size =0;
				nodeS->WSG->max_size = 2;
				nodeS->WSG->GNodeArray = malloc(sizeof(Node*)*nodeS->WSG->max_size);
			}
			InsertSGNodeList(nodeS->WSG,nodeE);//eisagwgh sth lista geitonwn idiou filou gunaikwn

		}	

	}


}

//sundeei tous kovmous ths listas geitniashs
void ELink(Edge *newNode ,e_list *list){
	
	newNode->next = NULL;
	newNode->prev = NULL;
		
	if(list->head == NULL)
		list->head = list->tail = newNode;
  	else{
		list->tail->next = newNode;
		newNode->prev =list->tail;
		list->tail = newNode;
				
  }
	list->size++;	
}

//ektupwnei thn lista geitniashs(Debugging)
void PrintEList(Node *node){

	e_list *list;
	list = node->E_List;
	Edge *Current = list->head;
	int i = list->size;
	printf("node %d neighbors\n",node->id);
	while(i>0){
		printf("node %s\n",Current->n_node->node_prop->pArray[0]);
		Current = Current->next;
		i--;
	}
	putchar('\n');
}
//katastrefei thn lista geitniashs
void DestroyEList(e_list *list){
	
	int i=0;
	if(list == NULL)
		return;
	
	Edge *Current = list->tail;	
	Edge *temp = Current;
	while(list->size>0){		
		list->size--;					
		Current = Current->prev;
		if(temp->edge_prop!=NULL)
			DestroyPropArray(temp->edge_prop);
		free(temp);
		temp = Current;
		
	}
	
	free(list);
}
//epistrefei deikth ston zhtoumeno komvo apo ton grafo
Node *LookupNode(Graph *graph,int id){

	TNode *tn;
	//anazhthsh ston grafo kai epistrofh "suntetagmenwn" ths topo8esias tou komvou sto hashtable pos gia thn sthlh kai place gia grammh
	tn = T_Search(graph->hash_table,id);
	if(tn == NULL)
		return NULL;
	//epistfofh tou zhtoumenou komvou
	return tn->Nptr;	

}
//Eisagwgh komvou ston grafo
void InsertNode(Node *node,Graph *graph){

	int i,j;
	int pos =1;
	int value;
	TNode t_node;

	t_node.id = node->id;
	t_node.Nptr = node;									
	T_InsertNode(&t_node,graph->hash_table);
	

	
	(graph->total_nodes)++;//au3hsh twn komvwn tou grafou gia ton komvo pou mphke omala
	node->ins_num = graph->total_nodes;
	//dhmiourgia kovmou gia thn lista CGL gia olous tous komvous tou grafou ws uposphfious antiproswpous gia coherent grafous
	CGLNode *newCGLnode;
	newCGLnode = malloc(sizeof(CGLNode));
	newCGLnode->Repr_node_id = node->id;
	newCGLnode->node_num =0;
	newCGLnode->CGmembers = malloc(sizeof(Node)*4);	
	newCGLnode->max_size = 4;
	CGL_Link(newCGLnode ,graph->CGL);



	if(node->node_prop!=NULL){
		//eisodos komvwn ston grafo andrwn
		if(!strcmp(node->node_prop->pArray[3],"male"))
			M_W_InsertNode(node,graph->M_graph);		
	
		//eisodos komvwn sto grafo gunaikwn
		if(!strcmp(node->node_prop->pArray[3],"female"))
			M_W_InsertNode(node,graph->W_graph);

	}
	

}
//katastrfofh tou pinaka me ta attributes
void DestroyPropArray(Properties *prop){

	int i=0;
	for(i;i<prop->prop_num;i++){
		free(prop->pArray[i]);
	}
	free(prop->pArray);
	free(prop);	
}

//Euresh suntomoterou monopatiou meta3u duo komvwn mesw BFS
//Bidirectional search
int ReachNode1(int Start,int End,Graph *g){

	Node *nodeStart,*nodeEnd;
	nodeStart = LookupNode(g,Start);
	nodeEnd = LookupNode(g,End);

	if(nodeStart->E_List == NULL || nodeEnd->E_List==NULL)
		return INFINITY_REACH_NODE;

	int m=2,c=3;
	TNode t_node;
	if(nodeStart->id == nodeEnd->id)
		return 0;
		
	if(T_Search(nodeStart->knows_hash_table,nodeEnd->id)!=NULL)
		return 1;

	struct Cell *visited_hash_table_Start,*visited_hash_table_End;

	visited_hash_table_Start = Create_T_Hash_table(m,c);
	visited_hash_table_End = Create_T_Hash_table(m,c);
	int i=0;
	int BFS_start=1;
	int BFS_end=0;
	Node *cur_node;
	VQueue *VqueueStart,*VqueueEnd;	
	Edge *cur_e;
	
	Initialize_VQueue(&VqueueStart,nodeStart->id,0);
	Initialize_VQueue(&VqueueEnd,nodeStart->id,0);	
	VLink(nodeStart,VqueueStart,NULL);
	VLink(nodeEnd,VqueueEnd,NULL);

	t_node.id = nodeStart->id;
	nodeStart->level1 = 0;
	T_InsertNode(&t_node,visited_hash_table_Start);
	T_InsertNode(&t_node,visited_hash_table_End);

	t_node.id =nodeEnd->id;
	nodeEnd->level1 = 0;
	T_InsertNode(&t_node,visited_hash_table_Start);
	T_InsertNode(&t_node,visited_hash_table_End);
	
	
	int cur_s_path;
	int s_path = INT_MAX; 

	struct TNode *meet;
	int j;
	int insert=0;
	do{

		if(BFS_start==1){


			while(VqueueStart->size!=0){
			

				//printf("kanw BFS_start\n");	
				cur_node = VqueueStart->head->node;

				RemoveVQueue(VqueueStart);
				//printf("BFS start episkeptomai ton %d sto level %d\n",cur_node->id,cur_node->level1);

				cur_e = cur_node->E_List->head;
				for(i=0;i<cur_node->E_List->size;i++){



					if(T_Search(visited_hash_table_Start,cur_e->n_node->id)==NULL){	
																	
																		
						t_node.id = cur_e->n_node->id;
						t_node.Nptr = cur_e->n_node;

						meet = T_Search(visited_hash_table_End,cur_e->n_node->id);
						if(meet!=NULL){
							T_InsertNode(meet,visited_hash_table_Start);
							//printf("Meet at %d Bfs start\n",meet->Nptr->id);
							cur_s_path = cur_node->level1 + meet->Nptr->level1;
							if(cur_s_path<s_path)
								s_path = cur_s_path;

						}

						if(meet==NULL){
							VLink(cur_e->n_node,VqueueStart,cur_node);	
							cur_e->n_node->level1 = 0;
							cur_e->n_node->level1 = cur_node->level1+1;
							//printf("bfs start  vazw %d level %d\n",cur_e->n_node->id,cur_e->n_node->level);
							T_InsertNode(&t_node,visited_hash_table_Start);
						}
					}

					cur_e = cur_e->next;	

				}

				BFS_start=0;
				if(VqueueEnd->size!=0)
					BFS_end=1;
				break;
			}
		}
	if(BFS_end==1){
			while(VqueueEnd->size!=0){
						
				cur_node = VqueueEnd->head->node;
				
				RemoveVQueue(VqueueEnd);
				//printf("BFS end episkeptomai ton %d sto level %d\n",cur_node->id,cur_node->level1);
	
				cur_e = cur_node->E_List->head;
				for(i=0;i<cur_node->E_List->size;i++){
	
				
					if(T_Search(visited_hash_table_End,cur_e->n_node->id)==NULL){	
							
																						
							t_node.id = cur_e->n_node->id;
							t_node.Nptr = cur_e->n_node;
							meet = T_Search(visited_hash_table_Start,cur_e->n_node->id);
							if(meet!=NULL){
							T_InsertNode(meet,visited_hash_table_End);
								
								//printf("Meet at %d Bfs end\n",meet->Nptr->id);
								cur_s_path = cur_node->level1 + meet->Nptr->level1;
								if(cur_s_path<s_path)
									s_path = cur_s_path;
							
															
							}

							if(meet==NULL){

								VLink(cur_e->n_node,VqueueEnd,cur_node);
								cur_e->n_node->level1 = 0;
								cur_e->n_node->level1 = cur_node->level1+1;
								//printf("bfs end  vazw %d level %d\n",cur_e->n_node->id,cur_e->n_node->level1);
								T_InsertNode(&t_node,visited_hash_table_End);
							}
						}

					cur_e = cur_e->next;	

				}

				BFS_end=0;
				if(VqueueStart->size!=0)
					BFS_start=1;
				break;
		
			}
	}

	}while(BFS_start==1 || BFS_end==1);

	DestroyVQueue(VqueueStart);	
	DestroyVQueue(VqueueEnd);
								
	for(j=0;j<visited_hash_table_End->info.hash_table_size;j++)
		free(visited_hash_table_End->bucket[j].t_node); 
	
	free(visited_hash_table_End->bucket);
	free(visited_hash_table_End);


	for(j=0;j<visited_hash_table_Start->info.hash_table_size;j++)
		free(visited_hash_table_Start->bucket[j].t_node); 
	
	free(visited_hash_table_Start->bucket);
	free(visited_hash_table_Start);

	if(s_path == INT_MAX)
		return INFINITY_REACH_NODE;
	
	return s_path+1;
}


int ReachNode12(int startID, int endID,Graph *g){
 
	int m=2,c=3;

	TNode *t_node;
	struct Cell *visited_hash_table;
///////////Dhmiourgia visited_hash_table////////////////////////////////////////////////////////
	visited_hash_table = Create_T_Hash_table(m,c);
////////////////////////////////////////////////////////////////////////////////////////////////
	if(startID == endID)//ean zhtame monopati apo enan komvo ston eauto tou tote to monopati einai mhden
		return 0;

	VQueue *Vqueue;
	Node *node;
	Edge *current;
////////////////////Anazhthsh tou komvou start//////////////////////////////////////////////////////
	node = LookupNode(g,startID);

	int spath=0;//arxikopoihsh monopatiou me mhden
//////////////////Arxikopoihshs ths ouras gia to BFS///////////////////////////////////////////////
	Initialize_VQueue(&Vqueue,startID,g->total_nodes);//arxikopoihsh ths Vqueue
	VLink(node,Vqueue,NULL);//Eisagwgh tou komvou sthn Vqueue	
//////////////////Eisagwgh tou arxikou komvou sto visited hash_table//////////////////////////////
	t_node = malloc(sizeof(TNode));
	t_node->id = node->id;
	t_node->Nptr = node;
	t_node->RNptr = NULL;
	T_InsertNode(t_node,visited_hash_table);
///////////////////Dhmiourgia pinaka/listas gia back tracking tou path/////////////////////////////////////////
	TNodeList *Store_Path;
	Store_Path = malloc(sizeof(TNodeList));
	Store_Path->TArray = malloc(sizeof(TNode*)*2);
	Store_Path->size = 0;
	Store_Path->max_size = 2;
	InsertTNodeList(Store_Path,t_node);

	free(t_node);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////anadromiko BFS//////////////////////////////////////////////////////////////////
	RecursiveBFS(node,endID,Vqueue,Store_Path,&spath,g,visited_hash_table);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////Katastrofh tou pinaka/listas gia back tracking tou path kai Vqueue////////////////////////////////////////////
	DestroyVQueue(Vqueue);	
	free(Store_Path->TArray);	
	free(Store_Path);

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////Katastrofh tou visited_hash_table///////////////////////////////////////////////////////
	int j;
	for(j=0;j<visited_hash_table->info.hash_table_size;j++)
			free(visited_hash_table->bucket[j].t_node); 
	
	free(visited_hash_table->bucket);
	free(visited_hash_table);
//////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	
	if(spath != -1)//ean to spath den einai -1 tote to monopati vre8hke alliws den vre8hke
		return spath;
	else
		return INFINITY_REACH_NODE; 
}
//Arxikopoihsh ths Ouras gia to BFS/ xrhsimopoietai kai apo thn Vqueue kai apo thn Squeue
void Initialize_VQueue(VQueue **Queue,int startID,int t_nodes){

	VQueue *queue;
	queue = malloc(sizeof(VQueue));
	queue->startID = startID;
	queue->total_nodes = t_nodes;
	queue->size = 0;
	queue->head = NULL;
	queue->tail = NULL;
	*Queue = queue;

}

//sundesh komvwn ouras gia to BFS/ xrhsimopoietai kai apo thn Vqueue kai apo thn Squeue
void VLink(Node *gnode,VQueue *queue,Node *c){

	
	VQNode *newNode;
	newNode = malloc(sizeof(VQNode));

	newNode->node = gnode;//o trexwn komvos
	newNode->node->came_from = c;//o komvos apo ton opoio prohl8e sumfwna me to BFS
	newNode->next = NULL;
	newNode->prev = NULL;
	
//sundesh sthn oura		
	if(queue->head == NULL)
		queue->head = queue->tail = newNode;
  	else{
		queue->tail->next = newNode;
		newNode->prev =queue->tail;
		queue->tail = newNode;
				
  }
	queue->size++;//au3hsh mege8ous ouras	

}

//katastorfh ouras / xrhsimopoietai kai apo thn Vqueue kai apo thn Squeue
void DestroyVQueue(VQueue *queue){

	if(queue == NULL)
		return;

	
	VQNode *Current = queue->tail;	
	VQNode *temp = Current;
	while(queue->size>0){		
		queue->size--;					
		temp = Current;
		Current = Current->prev;
		free(temp);		
	}
	free(queue);

}
//aferaish tou prwtou komvou ths ouras/xrhsimopoietai mono apo tohn Vqueue
void RemoveVQueue(VQueue *Vqueue){

	if(Vqueue->size == 0){
		Vqueue->head =NULL;
		return;
	}

	VQNode *tmp;
	tmp = Vqueue->head;
	Vqueue->head = Vqueue->head->next;
	Vqueue->size--;
	free(tmp);
}

//Anadromiko BFS
void RecursiveBFS(Node *node,int endID,VQueue *Vqueue,TNodeList *Store_Path,int *spath,Graph *g,Cell *visited){


	int i;
	TNode *t_node;
	VQNode *Current;

	if(Vqueue->size !=0)//E3agwgh tou prwtou komvou ths ouras efoson auth den einai kenh		
		RemoveVQueue(Vqueue);
	
	if(node->E_List == NULL){//Enan den uparxoun geitoninkoi komvoi shmainei oti o komvos einai asundetos
		(*spath)=-1;//epistroh -1 ws monopati afou o komvos einai asundetos
		return;
	
	}

	Edge *current = node->E_List->head;//8etoume ws current ton prwto geitona tou trexontos komvou
	
	//statiko T_Node
	for(i=0;i<node->E_List->size;i++){
		if(T_Search(visited,current->n_node->id)==NULL){	
			VLink(current->n_node,Vqueue,node);//eisagwgh sthn Vqueue gia to BFS
				t_node = malloc(sizeof(TNode));
				t_node->id = current->n_node->id;
				t_node->Nptr = current->n_node;
				t_node->RNptr = NULL;
				T_InsertNode(t_node,visited);
				InsertTNodeList(Store_Path,t_node);

				free(t_node);
		}
		current = current->next;
	}
	

	Current = Vqueue->head;//8etoume se auton ton current ton prwto komvo ths Vqueue
	
	//printf("episkeptomai ton %d\n",Current->node->id);

	if(endID == Current->node->id){//ean o komvos autos exei to id pou zhtame tote upologizoume to monopati mesw ths Backtrack_path
				
		//printf("--->apo to %d sto %d ->",Vqueue->startID,Current->node->id);
		(*spath) = Backtrack_path(Store_Path,Current->node,g,Vqueue->startID);
		//printf(" spath %d",*spath);
		//printf("\n");
		return;		
	}

	//anadromiko kalesma tou BFS kai h diadikasia arxizei apo thn arxh
	RecursiveBFS(Current->node,endID,Vqueue,Store_Path,spath,g,visited);


}
//h sunarhsh auth upologizei to monopati mesw tou melous came_from tou komvou
int Backtrack_path(TNodeList *Store_Path,Node *end,Graph *g,int startID){

	int found=0;
	int i=0,spath=0;
	VQNode *current;
	Node *c;

	if(end->came_from==NULL)
		return 0;
	c = end;
	//printf("%d ",c->id);
	//printf("apo to %d sto %d\n",Squeue->startID,endID);
	for(i=0;i<Store_Path->size;i++){
		
		spath++;//au3hsh tou monopatiou kata 1 gia ka8e komvo came_from
		if(c->came_from->id == startID){//ean to came_from exei to zhtoumeno id stamatame kai 8etoume found =1			
			//printf(" %d ",c->came_from->id);
			found = 1;
			break;

		}
		if(c->came_from==NULL)	//stamatame ean ftasame ston arxiko komvo
			break;
		
		c = c->came_from;
		//printf(" %d ",c->id);
	}
	
	if(found == 0)//vre8hke h den vre8hke kai pratoume analoga
		return -1;
	else
		return spath;
}

//Ginetai BFS se olous tous komvous kai mpainoun sthn Squeue opws kai sto aplo BFS
void BFS_ALL(Node *node,VQueue *Vqueue,VQueue *Squeue){

	int i;
	if(node->E_List == NULL)
		return;
	VQNode *Current;
	/*if(Vqueue->size !=0)		
		RemoveVQueue(Vqueue);
	
	Edge *current = node->E_List->head;
	for(i=0;i<node->E_List->size;i++){
		if(current->n_node->visited == 0){	
			VLink(current->n_node,Vqueue,node);
			VLink(current->n_node,Squeue,node);
		}
		current = current->next;
	}
	

	Current = Vqueue->head;
	if(Current == NULL){
		return;
	}*/

	//BFS_ALL(Current->node,Vqueue,Squeue);


}


//////////////////////////////////////////////////////////////////////////////////////////

void RemoveCGL(Coherent_Graph_List *list,int node_id){

	CGLNode *tmp;
   tmp = list->head;
 	while(tmp!=NULL){

		if(tmp->Repr_node_id == node_id){

			//Remove from start
			if(node_id == list->head->Repr_node_id){
				list->head = tmp->next;
				list->head->prev = tmp->prev;
				free(tmp->CGmembers);
				free(tmp);
				list->size--;
				return;
			}
			//Remove from end		
			if(node_id == list->tail->Repr_node_id){
				list->tail = tmp->prev;	
				list->tail->next = tmp->next;
				free(tmp->CGmembers);
				free(tmp);
				list->size--;
				return;
			}
			//Remove from middle
			tmp->prev->next = tmp->next;			
			tmp->next->prev = tmp->prev;	
			free(tmp->CGmembers);
			free(tmp);
			list->size--;
			return;
		}
			

		
		tmp = tmp->next;
	}

}

void Initialize_CGL(Coherent_Graph_List **list){

	Coherent_Graph_List *LIST;
	LIST = malloc(sizeof(e_list));
	LIST->max_nodes_of_CG =0;
	LIST->size = 0;
	LIST->head = NULL;
	LIST->tail = NULL;
	*list = LIST;

}

void DestroyCGL(Coherent_Graph_List *list){

	if(list == NULL)
		return;

	
	CGLNode *Current = list->tail;	
	CGLNode *temp = Current;
	while(list->size>0){		
		list->size--;					
		temp = Current;
		Current = Current->prev;
		free(temp->CGmembers);
		free(temp);		
	}
	
	free(list);

}
void CGL_Link(CGLNode *newNode ,Coherent_Graph_List *list){
	
	newNode->next = NULL;
	newNode->prev = NULL;
		
	if(list->head == NULL)
		list->head = list->tail = newNode;
  	else{
		list->tail->next = newNode;
		newNode->prev =list->tail;
		list->tail = newNode;
				
  }
	list->size++;	
}

void IncreaseNodesofCG(Coherent_Graph_List *list,int CGnode_id,Node *n){

	CGLNode *current;
	current = list->head;
	CGMebers *m;
	while(current!=NULL){

		if(current->Repr_node_id == CGnode_id){
			current->node_num++;

			
			if(current->node_num <= current->max_size){
				current->CGmembers[current->node_num-1].Member = n;	
			}
			else{
				current->max_size = current->max_size*2;
				m =realloc(current->CGmembers,current->max_size*sizeof(struct CGMebers));
				current->CGmembers =m;
				current->CGmembers[current->node_num-1].Member = n;
				
			}

			if(list->max_nodes_of_CG < current->node_num){//ean exoume neo megisto komvwn sunektikou grafou
				list->max_nodes_of_CG = current->node_num;//allagh tou megistou me to trexon megisto
				list->CGmax_ID = CGnode_id;
			}
			break;
		
		}
		current = current->next;
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Node* setPersonProperties(int id, char* name, char* surname, int age,char *genre) {

	 	
    /*create properties*/
    Properties* prop = createProperties(PERSON_PROPERTIES_NUM);
    setStringProperty(name, 0, prop);
    setStringProperty(surname, 1, prop);
    setIntegerProperty(age, 2, prop);
	 setStringProperty(genre, 3, prop);
    /*create node*/
 	 //printf(" id %d attrs-->%p\n",id,prop);	
    Node* n = createNode(id, prop);

    return n;
}


int Fix_EdgeID(int ins1,int ins2){

	char eID[MAX_STRING_LENGTH];
	char str1[MAX_STRING_LENGTH];
	char str2[MAX_STRING_LENGTH];
	sprintf(str1,"%d",ins1);
	sprintf(str2,"%d",ins2);
	if(ins1<ins2){
		strcpy(eID,str1);
		strcat(eID,str2);
		strcat(eID,"\0");
	}
	else{
		strcpy(eID,str2);
		strcat(eID,str1);
		strcat(eID,"\0");
	}

	return atoi(eID);
}

Edge* setEdgeProperties(int startNodeID, int endNodeID, char* type, double weight) {

    /*create edge properties*/
    Properties* propEdge = createProperties(EDGE_PROPERTIES_NUM);
    setStringProperty(type, 0, propEdge);
    setfloatProperty(weight, 1, propEdge);

    /*create an edge*/
    Edge* e = createEdge(startNodeID, endNodeID, propEdge);
    return e;
}

void printPersonProperties(Node* n) {

   int i;
	for(i=0;i<n->node_prop->prop_num;i++)
		printf("%s ",n->node_prop->pArray[i]);

	putchar('\n');
}

void printEdgeProperties(Edge* e) {
   int i;
	for(i=0;i<e->edge_prop->prop_num;i++)
		printf("%s ",e->edge_prop->pArray[i]);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////*****Read from files*****///////////////////////////////////////////////////
void Insert_Persons(Graph *g,char *path){

	FILE * fp;	 
	char STR[MAX_STRING_LENGTH];
	char strID[MAX_STRING_LENGTH],strFN[MAX_STRING_LENGTH],strLN[MAX_STRING_LENGTH],strGEN[MAX_STRING_LENGTH];
	char strCRDATE[MAX_STRING_LENGTH],strLOCIP[MAX_STRING_LENGTH],strBU[MAX_STRING_LENGTH];
	char strBIRTH[MAX_STRING_LENGTH];
	char strAGE[MAX_STRING_LENGTH];
	char strYEAR[MAX_STRING_LENGTH];
	char c,c2;
	int i=0;
	int k=0;
	int j=0;
	int m=0;
	int h=0;
	int ignore=1;
	Node *node;	
 	int age;
	
	//id|firstName|lastName|gender|birthday|creationDate|locationIP|browserUsed

	 fp = fopen (path, "r");	
	 
	 while(fgets (STR,MAX_STRING_LENGTH, fp)!=NULL){	
				
		k=0;
		j=0;
		for(i=0;i<strlen(STR);i++){
			c = STR[i];
			if(STR[i]=='|'){
				k++;
				j=0;
			}
			else{
				if(k==0){
					strID[j]=c;
					j++;
					strID[j]='\0';
				}
				if(k==1){
					strFN[j]=c;
					j++;
					strFN[j]='\0';
				}
				if(k==2){
					strLN[j]=c;
					j++;
					strLN[j]='\0';
				}				
				if(k==3){
					strGEN[j]=c;
					j++;
					strGEN[j]='\0';
				}
				if(k==4){
					strBIRTH[j]=c;
					j++;
					strBIRTH[j]='\0';
				}
				if(k==5){
					strCRDATE[j]=c;
					j++;
					strCRDATE[j]='\0';
				}		
				if(k==6){
					strLOCIP[j]=c;
					j++;
					strLOCIP[j]='\0';
				}
				if(k==7){
					strBU[j]=c;
					j++;
					strBU[j]='\0';
				}


			}
			
		}
		
		h=0;
		for(m=0;m<strlen(strBIRTH);m++){
			c2 = strBIRTH[m];
			if(c2!='-'){
				strYEAR[h]=c2;														
				h++;
			}
			else
				break;

		}
		strYEAR[h]='\0';
		//id|firstName|lastName|gender|birthday|creationDate|locationIP|browserUsed
		age = 2014 - atoi(strYEAR);
		//printf("%d %s %s %s %s %s %s %s %s\n",age,strID,strFN,strLN,strGEN,strBIRTH,strCRDATE,strLOCIP,strBU);

		if(ignore!=1){	
			node = setPersonProperties(atoi(strID),strFN,strLN,age,strGEN);		
			insertNode(node, g);	
		}
		strID[0]='\0';
	   strFN[0]='\0';
		strLN[0]='\0';
		strGEN[0]='\0';
		strBIRTH[0]='\0';
		strCRDATE[0]='\0';
		strLOCIP[0]='\0';
		strBU[0]='\0';
		ignore =0;

		
	 }


	fclose(fp);
}

void Insert_Edges(Graph*g,char *path){

	int i,j,k;
	FILE * fp;
	Edge *e;
	int ignore=1;
	char c;
	char STR[MAX_STRING_LENGTH];
	char Start[MAX_STRING_LENGTH],End[MAX_STRING_LENGTH];
	fp = fopen (path, "r");	
	 
	 while(fgets (STR,MAX_STRING_LENGTH, fp)!=NULL){

		k=0;
		j=0;
		for(i=0;i<strlen(STR);i++){
			c = STR[i];
			if(STR[i]=='|'){
				k++;
				j=0;
			}
			else{
				if(k==0){
					Start[j]=c;
					j++;
					Start[j]='\0';
				}
				if(k==1){
					End[j]=c;
					j++;
					End[j]='\0';
				}

			}

		}
		//printf("%d knows %d\n",atoi(Start),atoi(End));
		Node *NodeStart,*NodeEnd;
		if(ignore!=1){
	
			NodeStart = LookupNode(g,atoi(Start));
			NodeEnd = LookupNode(g,atoi(End));

			if(NodeStart!=NULL && NodeEnd !=NULL){

				if(T_Search(NodeStart->knows_hash_table,NodeEnd->id)==NULL){
					e = setEdgeProperties(atoi(Start),atoi(End),"knows",0);
					e->EdgeID = g->total_edges;
					insertEdge(atoi(Start), e, g);
					
				}
			}

		}
		ignore = 0;
	}

	fclose(fp);

}

void Table_Input(struct Cell *Table,char *filename,struct Graph *g,char *table_type){

	TNode t_node;
	RNode *r_node;
	FNode *f_node;
	int i,j,k;	
	int ignore =1;
	FILE *fp;

	char c;
	char STR[MAX_STRING_LENGTH];
	char ID[MAX_STRING_LENGTH],NAME[MAX_STRING_LENGTH];
	char ORG_TYPE[MAX_STRING_LENGTH];
	printf("%s\n",filename);
	//getchar();
	fp = fopen (filename, "r");	

	 while(fgets (STR,MAX_STRING_LENGTH, fp)!=NULL){	

		k=0;
		j=0;
		for(i=0;i<strlen(STR);i++){
			c = STR[i];
			if(STR[i]=='|'){
				k++;
				j=0;
			}
			else{
				if(k==0){
					ID[j]=c;
					j++;
					ID[j]='\0';
				}
				if(!strcmp(table_type,"organisation")){
	
					if(k==1){
						ORG_TYPE[j]=c;
						j++;
						ORG_TYPE[j]='\0';
					}
					
					if(k==2){
						NAME[j]=c;
						j++;
						NAME[j]='\0';
					}

				} 
				else{
		
					if(k==1){
						NAME[j]=c;
						j++;
						NAME[j]='\0';
					}
				}

			}

		}

		if(!strcmp(table_type,"forum") && ignore == 0){
			//printf("%s %s\n",ID,NAME);
			f_node = create_Forum_node(atoi(ID),NAME);
			InsertFNodeList(g->Forums,f_node);
			t_node.id = f_node->id;
			t_node.FNptr = f_node;

			T_InsertNode(&t_node,Table);
		}
		else if(ignore == 0){	
			r_node = R_setProperties(atoi(ID),NAME);		

			t_node.id = r_node->id;
			t_node.RNptr = r_node;

			T_InsertNode(&t_node,Table);
		}	

		ignore = 0;
	}

	fclose(fp);
	


}

void Insert_Relation(Graph *g,char *type,char *filename){

	struct TNode *t_node,post_input,*tn;
	RNode *n1;
	Node *n2;
	FNode *n3;
	int i,j,k;	
	FILE * fp;
	int m=2,c1=3;
	int ignore=1;
	char c;
	char STR[MAX_STRING_LENGTH];
	char ID[MAX_STRING_LENGTH],R_ID[MAX_STRING_LENGTH];
	char YEAR[MAX_STRING_LENGTH];
	fp = fopen (filename, "r");	
	printf("%s\n",filename); 
	 while(fgets (STR,MAX_STRING_LENGTH, fp)!=NULL){	

		k=0;
		j=0;
		for(i=0;i<strlen(STR);i++){
			c = STR[i];
			if(STR[i]=='|'){
				k++;
				j=0;
			}
			else{
				if(k==0){
					ID[j]=c;
					j++;
					ID[j]='\0';
				}
				if(k==1){
					R_ID[j]=c;
					j++;
					R_ID[j] ='\0';
				}
				if(!strcmp(type,"working")){
			
					if(k==2){
						YEAR[j]=c;
						j++;
						YEAR[j] ='\0';
					}

				}

			}

		}
		
		if(!strcmp(type,"working") && ignore ==0 ){
			//printf("person %d works at %d year %s\n",atoi(ID),atoi(R_ID),YEAR);
			t_node = T_Search(g->h_ORG,atoi(R_ID));
			n2 = LookupNode(g,atoi(ID));
			
			if(t_node!=NULL && n2!=NULL){
				if(atoi(YEAR)>atoi(n2->Year_work)){					
					n1 = t_node->RNptr;
					strcpy(n2->Year_work,YEAR);
					Insert_Infos(n1,n2,"working");
				}
			}
		}
		
		if(!strcmp(type,"studying") && ignore ==0 ){
			//printf("person %d studies at %d\n",atoi(ID),atoi(R_ID));
			t_node = T_Search(g->h_ORG,atoi(R_ID));
			n2 = LookupNode(g,atoi(ID));		

			if(t_node!=NULL && n2!=NULL){
				n1 = t_node->RNptr;	
				Insert_Infos(n1,n2,"studying");
			}
		}

		if(!strcmp(type,"interest") && ignore ==0 ){
			//printf("person %d has interest  %d\n",atoi(ID),atoi(R_ID));
			t_node = T_Search(g->h_INT,atoi(R_ID));
			n2 = LookupNode(g,atoi(ID));		

			if(t_node!=NULL && n2!=NULL){
				n1 = t_node->RNptr;
				Insert_Infos(n1,n2,"interest");
	
			}
	
		}

		if(!strcmp(type,"living") && ignore ==0 ){			
			t_node = T_Search(g->h_PL,atoi(R_ID));
			
			n2 = LookupNode(g,atoi(ID));		
			if(t_node!=NULL && n2!=NULL){
				n1 = t_node->RNptr;
				Insert_Infos(n1,n2,"living");
			}
		}

		if(!strcmp(type,"post") && ignore == 0){
			//printf("post %d owned by %d\n",atoi(ID),atoi(R_ID));
			t_node = T_Search(g->h_POSTS,atoi(ID));
			

			n2 = LookupNode(g,atoi(R_ID));
			if(n2!=NULL && t_node!=NULL){	

				n1 = t_node->RNptr;				
				Insert_Infos(n1,n2,"post");
			}
		}

		if(!strcmp(type,"likes") && ignore ==0 ){

			t_node = T_Search(g->h_POSTS,atoi(R_ID));
			n2 = LookupNode(g,atoi(ID));
			if(n2!=NULL && t_node!=NULL){	
				n1 = t_node->RNptr;						
				//printf("person %d  likes post %d\n",n2->id,n1->id);
				Insert_Infos(n1,n2,"likes");

				like_counter *lc,*l;
				TNode *like_receiver,*forum,*t,*liker_hash;
				TNode like_input;
				if(t_node->RNptr->container_forum!=NULL && t_node->RNptr->owner!=NULL){
					forum = T_Search(g->h_FORUMS,t_node->RNptr->container_forum->id);
					like_receiver = T_Search(forum->FNptr->likes_to_from,t_node->RNptr->owner->id);
					liker_hash = T_Search(forum->FNptr->likes_to_from,n2->id);
					if(liker_hash!=NULL){
						l = (like_counter*)liker_hash->data;
						l->likes++;
					}
				}
				else{
					forum = NULL;
					like_receiver = NULL;

				}


				if(like_receiver != NULL && forum!=NULL){
					
			

					if(like_receiver->Hptr == NULL)
						like_receiver->Hptr = Create_T_Hash_table(m,c1);

							
					t = T_Search(like_receiver->Hptr,n2->id);
					if(t==NULL){
						lc = malloc(sizeof(like_counter));
						like_input.id = n2->id;
						lc->likes =1;
						like_input.data = lc;
						l = lc;
						//printf("person %d  likes post %d of %d\n",n2->id,n1->id,n1->owner->id);
						T_InsertNode(&like_input,like_receiver->Hptr);

					}
					else{
							l = (like_counter*)t->data;
							l->likes++;							
							//printf("-->person %d  likes  %d\n",n2->id,t_node->RNptr->owner->id);
							//printf("%d was liked by %d %d times\n",like_receiver->id,n2->id,l->likes);	
					}

					//printf("%d was liked by %d %d times\n",like_receiver->id,n2->id,l->likes);	
					//getchar();

				}



			}
		}	

		if(!strcmp(type,"member") && ignore == 0){
			
			TNode tn_input_like,tn_input_reply;
			like_counter *lc;		
			reply_counter *rc;
			
			t_node = T_Search(g->h_FORUMS,atoi(ID));
			
			n2 = LookupNode(g,atoi(R_ID));
			
			if(n2!=NULL && t_node!=NULL){	
				
				n3 = t_node->FNptr;						
				
				//printf("person %d  member of forum %d\n",n2->id,n3->id);
				if(n2->M_F == NULL){
					n2->M_F = malloc(sizeof(FNodeList));
					n2->M_F->size =0;
					n2->M_F->max_size = 2;
					n2->M_F->FArray = malloc(sizeof(FNode*)*n2->M_F->max_size);					
				}
				InsertFNodeList(n2->M_F,n3);

				lc = malloc(sizeof(like_counter));
				lc->likes =0;	
				rc = malloc(sizeof(reply_counter));
				rc->replies =0;
				tn_input_like.id = n2->id;
				tn_input_like.Hptr = NULL;
				tn_input_like.data = lc;

				tn_input_reply.id = n2->id;
				tn_input_reply.Hptr = NULL;
				tn_input_reply.data = rc;


				T_InsertNode(&tn_input_like,n3->likes_to_from);//eisagwgh se hash_table gia likes apo kapoion se kapoion
				T_InsertNode(&tn_input_reply,n3->replies_to_from);//eisagwgh se hash_table gia replies apo kapoion se kapoion
		
				if(n3->Forum_Members == NULL){
					n3->Forum_Members = malloc(sizeof(GNodeList));
					n3->Forum_Members->size =0;
					n3->Forum_Members->max_size = 2;
					n3->Forum_Members->GNodeArray = malloc(sizeof(Node*)*n3->Forum_Members->max_size);
				}
				InsertSGNodeList(n3->Forum_Members,n2);

			}

		}
		if(!strcmp(type,"comment") && ignore == 0){
			
				n2 = LookupNode(g,atoi(R_ID));
				if(n2!=NULL){	
					n1 = malloc(sizeof(RNode));
					n1->id = atoi(ID);
					n1->owner = n2;
					n1->node_prop = NULL;
					n1->E_List = NULL;
					post_input.id = n1->id;
					post_input.RNptr = n1;

					T_InsertNode(&post_input,g->h_COM);

			}
		}

		if(!strcmp(type,"reply") && ignore == 0){
			
			//printf("com %d  replies post %d\n",atoi(ID),atoi(R_ID));

			reply_counter *rc,*r;
			TNode *reply_receiver,*forum,*t,*replier_hash;
			Node *replier;
			TNode reply_input;
			t_node = T_Search(g->h_POSTS,atoi(R_ID));
			tn = T_Search(g->h_COM,atoi(ID));
			if(t_node!=NULL && tn!=NULL && tn->RNptr->owner!=NULL && t_node->RNptr->owner!=NULL && t_node->RNptr->container_forum!=NULL){


				forum = T_Search(g->h_FORUMS,t_node->RNptr->container_forum->id);
				//printf("com %d  replies post %d\n",atoi(ID),atoi(R_ID));


				replier = tn->RNptr->owner;
				reply_receiver = T_Search(forum->FNptr->replies_to_from,t_node->RNptr->owner->id);
				
				replier_hash = T_Search(forum->FNptr->replies_to_from,replier->id);

				
				if(replier_hash!=NULL){
					r = (reply_counter*)replier_hash->data;
					r->replies++;
					
				}

				

				if(reply_receiver != NULL && forum!=NULL){
					
					//printf("forum %d / %d was replied by %d for post %d\n",forum->id,reply_receiver->id,replier->id,t_node->id);
					if(reply_receiver->Hptr == NULL)
						reply_receiver->Hptr = Create_T_Hash_table(m,c1);

							
					t = T_Search(reply_receiver->Hptr,replier->id);
					if(t==NULL){
						rc = malloc(sizeof(reply_counter));
						reply_input.id = replier->id;
						rc->replies =1;
						reply_input.data = rc;
						T_InsertNode(&reply_input,reply_receiver->Hptr);

					}
					else{
							r = (reply_counter*)t->data;
							r->replies++;							
					}

					//printf("%d was replied by %d %d times\n",reply_receiver->id,replier->id,r->replies);
					//getchar();

				}		
				//printf("rec %d forum %d\n",forum->id,reply_receiver->id);


			}

		}







		ignore = 0;
		STR[0]='\0';
		ID[0]='\0';
		R_ID[0]='\0';
		YEAR[0]='\0';


	}

	fclose(fp);


}

void Insert_Infos(RNode *info_node,Node *person_node,char *type){

	Edge *e;
	
	if(!strcmp(type,"working")){
		
		e = malloc(sizeof(struct Edge));	
		strcpy(person_node->Working_At,info_node->node_prop->pArray[0]);
		//sprintf("%d works at %s\n",person_node->id,person_node->Working_At);
		person_node->currently_working = info_node;
	
		if(info_node->E_List == NULL)
			Initialize_E_List(&info_node->E_List,info_node->id);

		
		e->n_node = person_node;
		e->edge_prop = NULL;
		ELink(e,info_node->E_List);
		person_node->working_with = info_node->E_List;
	}

	if(!strcmp(type,"studying")){
		
		e = malloc(sizeof(struct Edge));
		strcpy(person_node->Studying_At,info_node->node_prop->pArray[0]);
	
		if(info_node->E_List == NULL)
			Initialize_E_List(&info_node->E_List,info_node->id);

		e->n_node = person_node;
		e->edge_prop = NULL;
		ELink(e,info_node->E_List);
		person_node->studying_with = info_node->E_List;
	}

	if(!strcmp(type,"interest")){
		e = malloc(sizeof(struct Edge));
		//Arxikopoihsh tou pinaka endiaferontwn tou trexon atomou
		if(person_node->interests == NULL){
			person_node->interests = malloc(sizeof(struct RNodeList));
			person_node->interests->RArray = malloc(sizeof(struct RNode)*2);
			person_node->interests->size =0;
			person_node->interests->max_size = 2;			
		}
	
		//Arxikopoihsh ths listas pou periexei komvous atomwn pou exoun auto to endiaferon	
		if(info_node->E_List == NULL)
			Initialize_E_List(&info_node->E_List,info_node->id);
				

		e->n_node = person_node;
		e->edge_prop = NULL;
		//Eisodos sth lista tou sugkekrimenou endiaferontos to atomo
		ELink(e,info_node->E_List);

		//Eisodos tou sugkekrimenou endiaferontos sthn lista me ta endiaferonta tou atomou
		InsertRNodeList(person_node->interests,info_node);
	}

	if(!strcmp(type,"living")){
		e = malloc(sizeof(struct Edge));
		strcpy(person_node->Living_At,info_node->node_prop->pArray[0]);
		if(info_node->E_List == NULL)
			Initialize_E_List(&info_node->E_List,info_node->id);


		e->n_node = person_node;
		e->edge_prop = NULL;
		ELink(e,info_node->E_List);
		person_node->living_with = info_node->E_List;
	}

	if(!strcmp(type,"post")){
			
		//Arxikopoihsh tou pinaka posts tou trexon atomou
		if(person_node->posts == NULL){
			person_node->posts = malloc(sizeof(RNodeList));
			person_node->posts->RArray = malloc(sizeof(RNode)*2);
			person_node->posts->size =0;
			person_node->posts->max_size = 2;			
		}
	
		//idiokthths tou sugkekrimenou post
		info_node->owner = person_node;
		//printf("vazw to %s\n",info_node->node_prop->pArray[0]);
		//Eisodos tou sugkekrimenou endiaferontos sthn lista me ta endiaferonta tou atomou
		InsertRNodeList(person_node->posts,info_node);
	}
	if(!strcmp(type,"likes")){

		if(person_node->likes == NULL){
			person_node->likes = malloc(sizeof(RNodeList));
			person_node->likes->RArray = malloc(sizeof(RNode)*2);
			person_node->likes->size =0;
			person_node->likes->max_size = 2;			
		}

		InsertRNodeList(person_node->likes,info_node);

	}

}

void Forum_container_of_posts(struct Cell *post_hash,struct Cell *forum_hash){


	FILE *fp;
	TNode *tn1,*tn2;
	int i,j,k;
	char STR[MAX_STRING_LENGTH];
	char ID[MAX_STRING_LENGTH],R_ID[MAX_STRING_LENGTH];
	char c;
	int ignore=1;
	fp = fopen ("dataset2/forum_containerOf_post.csv", "r");	
	 
	
	 while(fgets (STR,MAX_STRING_LENGTH, fp)!=NULL){	
		
		k=0;
		j=0;
		for(i=0;i<strlen(STR);i++){
			c = STR[i];
			if(STR[i]=='|'){
				k++;
				j=0;
			}
			else{
				if(k==0){
					ID[j]=c;
					j++;
					ID[j]='\0';
				}
				if(k==1){
					R_ID[j]=c;
					j++;
					R_ID[j] ='\0';
				}


			}

		}
		if(ignore!=1){
			
			//printf("searching %d\n",atoi(ID));
			tn1 =  T_Search(forum_hash,atoi(ID));
			tn2 =  T_Search(post_hash,atoi(R_ID));	

			if(tn1!=NULL && tn2!=NULL && tn2->RNptr->owner!=NULL)								
				tn2->RNptr->container_forum = tn1->FNptr;
				
		}
		ignore = 0;
	
	}
	fclose(fp);
}


void Insert_Tables(Graph *g,int part){

	int m = 2;
   int c = 3;


	if(part == 3){
		g->h_FORUMS = Create_T_Hash_table(m,c);//dhmiougira hash_table gia ta forums
		Table_Input(g->h_FORUMS,"dataset3/forum.csv",g,"forum");
		Insert_Relation(g,"member","dataset3/forum_hasMember_person.csv");


	}
	else if(part == 2){
		g->h_ORG = Create_T_Hash_table(m,c);//dhmiourgia hash_table gia tous organismous
		g->h_INT = Create_T_Hash_table(m,c);//dhmiourgia hash_table gia ta endiaferonta
		g->h_PL  = Create_T_Hash_table(m,c);//dhmiourgia hash_table gia tis perioxes pou menoun ta atoma
		g->h_POSTS = Create_T_Hash_table(m,c);//dhmiougira hash_table gia ta posts
		g->h_COM = Create_T_Hash_table(m,c);//dhmiougira hash_table gia ta comments
		g->h_FORUMS = Create_T_Hash_table(m,c);//dhmiougira hash_table gia ta forums

		Table_Input(g->h_FORUMS,"dataset2/forum.csv",g,"forum");
		Insert_Relation(g,"member","dataset2/forum_hasMember_person.csv");


		Table_Input(g->h_ORG,"dataset2/organisation.csv",g,"organisation");

		Insert_Relation(g,"working","dataset2/person_workAt_organisation.csv");


		Insert_Relation(g,"studying","dataset2/person_studyAt_organisation.csv");

		Table_Input(g->h_INT,"dataset2/tag.csv",g,"tag");
		
		Insert_Relation(g,"interest","dataset2/person_hasInterest_tag.csv");

		Table_Input(g->h_PL,"dataset2/place.csv",g,"place");

		Insert_Relation(g,"living","dataset2/person_isLocatedIn_place.csv");

		Table_Input(g->h_POSTS,"dataset2/post.csv",g,"post");

	
		Insert_Relation(g,"post","dataset2/post_hasCreator_person.csv");
	
		Forum_container_of_posts(g->h_POSTS,g->h_FORUMS);


		Insert_Relation(g,"comment","dataset2/comment_hasCreator_person.csv");

		Insert_Relation(g,"reply","dataset2/comment_replyOf_post.csv");
		Insert_Relation(g,"likes","dataset2/person_likes_post.csv");

	}	
}


void Insert_Levels(Node *n,Graph *g){

	int i=0;
	CGLNode *c = g->CGL->head;
	Node *nodeEnd;

	while(c!=NULL){
		
		if(n->Graph_ID == c->Repr_node_id)
			break;		
			c = c->next;
	}		

	for(i=0;i<c->node_num;i++){
				nodeEnd = c->CGmembers[i].Member;	
				if(nodeEnd->id != n->id)	
					nodeEnd->level = ReachNode1(n->id, nodeEnd->id,g);									
	}
		

}


//upologismos monopatiou apo enan komvo pros olous
ResultSet *ReachNodeN(int startID, Graph *g){
	 
	int m=2,c=3;
	int i=0;
	TNode tn;
	Edge *cur_e;
	Node *node = LookupNode(g,startID);


	node->dist = 0;
	ResultSet *res = malloc(sizeof(struct resultSet));

	res->Visited_hash = Create_T_Hash_table(m,c);
	Initialize_VQueue(&res->Visited_Queue,node->id,0);
	tn.id = node->id;
	T_InsertNode(&tn,res->Visited_hash);
	

	cur_e = node->E_List->head;
	for(i=0;i<node->E_List->size;i++){

		tn.id = cur_e->n_node->id;
		T_InsertNode(&tn,res->Visited_hash);

		cur_e->n_node->dist = node->dist+1;
		//printf("To %d pairnei level %d\n",cur_e->n_node->id,cur_e->n_node->dist);
		VLink(cur_e->n_node,res->Visited_Queue,node);

		cur_e = cur_e->next;
	}	

	return res;	
}

int next(ResultSet* res, Pair *pair){

	TNode tn;
	int ret =0;
	Edge *cur_e;
	Node *cur_node;
	int i=0;

	if(res->Visited_Queue->size !=0){		
		ret = 1;
		cur_node = res->Visited_Queue->head->node;
		tn.id = cur_node->id;
		T_InsertNode(&tn,res->Visited_hash);
		RemoveVQueue(res->Visited_Queue);		
		cur_e = cur_node->E_List->head;
		for(i=0;i<cur_node->E_List->size;i++){

			if(T_Search(res->Visited_hash,cur_e->n_node->id)==NULL){
				
				tn.id = cur_e->n_node->id;
				T_InsertNode(&tn,res->Visited_hash);


				cur_e->n_node->dist = cur_node->dist+1;
				//printf("To %d pairnei level %d\n",cur_e->n_node->id,cur_e->n_node->dist);
				VLink(cur_e->n_node,res->Visited_Queue,cur_node);

			}
			cur_e = cur_e->next;
		}

	}

	if(ret == 0) 
		return 0;
	else{
		pair->ID = cur_node->id;
		pair->distance = cur_node->dist;	
		return 1;

	}
}

void Destroy_ResultSet(ResultSet* res){

	int i=0;

	DestroyVQueue(res->Visited_Queue);

	for(i=0;i<res->Visited_hash->info.hash_table_size;i++)
		free(res->Visited_hash->bucket[i].t_node); 
	
	free(res->Visited_hash->bucket);
	free(res->Visited_hash);

	free(res);
}






