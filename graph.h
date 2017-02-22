#ifndef GRAPH_H
#define GRAPH_H
#include"defines.h"

typedef struct edge_bet_counter{

	int bet_num;
	int state;
	double edge_betweenness;
	struct Edge *e_ptr;

}edge_bet_counter;


typedef struct like_counter{

	int likes;

}like_counter;


typedef struct reply_counter{

	int replies;

}reply_counter;


typedef struct FNodeList{

	struct FNode **FArray;	
	int cur;
	int size;
	int max_size;
}FNodeList;

void InsertFNodeList(struct FNodeList *,struct FNode *);

typedef struct TNodeList{

	struct TNode **TArray;	
	int size;
	int max_size;
}TNodeList;

void InsertTNodeList(struct TNodeList *,struct TNode *);


typedef struct FNode{

	int id;
	char name[MAX_STRING_LENGTH];
	struct GNodeList *Forum_Members;
	struct Cell *likes_to_from;
	struct Cell *replies_to_from;

}FNode;

FNode *FNL_Search(struct FNodeList* ,int);
//komvos interest/post
typedef struct RNode{

	int id;
	struct properties *node_prop;
	int matched;//an exei mpei hdh sthn ekastote lista
	int freq;//poses fores exei emfanistei
	struct e_list *E_List;
	struct Cell *matched_hash_table;
	int max_freq;
	struct Node *owner;
	struct FNode *container_forum;


}RNode;


typedef struct RNodeList{

	struct RNode **RArray;	
	int size;
	int max_size;
}RNodeList;

void InsertRNodeList(RNodeList *,RNode *);
struct RNodeList *Create_RNodeList();


typedef struct GNodeList{

	struct Node **GNodeArray;	
	int size;
	int max_size;
}GNodeList;

void InsertSGNodeList(GNodeList *,struct Node *);

struct Node *GNL_Search(struct GNodeList* ,int);
GNodeList *Create_GNodeList();

/* Pair of integers*/
struct pair {
	int ID; // the id of a node
   int distance; //the distance from the node	
};
typedef struct pair Pair;

struct resultSet {

	struct VQueue *Visited_Queue;
	struct Cell *Visited_hash;
}resultSet;
typedef struct resultSet ResultSet;



struct properties{//domh properties

	char **pArray;//pinakas apo strings gia ta en merh attributes
	int prop_num;//ari8mos attribute

};
typedef struct properties Properties;

//Komvos tou grafou
typedef struct Node{
	int id;
	int visited;//xrhsimopoieitai sto modified DFS gia na elegxetai an exei ginei episkepsh ston komvo
	int Stalker_likes;
	struct Node *came_from;//xrhsimopoieitai gia thn euresh suntomoterou monopatiou
	Properties *node_prop;//domh pou periexei ta attributes
	struct e_list *E_List;//lista me tous komvous pou sundeetai o ka8e komvos/3erei/einai geitones	
	struct Cell *knows_hash_table;//hash_table me tous komvous pou 3erei/einai geitones
	struct Cell *levels;


	int dist;
	int ins_num;
	GNodeList *MSG;//lista/pinakas geitniashs andrwn
	GNodeList *WSG;//lista/pinakas geitniashs gunaikwn

	int level;
	int level1;
	int SGraph_ID;//to anagnwristikou enos coherent stalker grafou sto opoio anhkei o komvos	
	int Graph_ID;//to anagnwristikou enos coherent grafou sto opoio anhkei o komvos			
	int Graph_M_ID;////to anagnwristikou enos coherent grafou andrwn sto opoio anhkei o komvos	
	int Graph_W_ID;////to anagnwristikou enos coherent grafou gunaikwn sto opoio anhkei o komvos
	RNode *currently_working;
	char Working_At[MAX_STRING_LENGTH];//pinakas gia to pou douleuei
	char Studying_At[MAX_STRING_LENGTH];//pinakas gia to pou spoudzei
	char Living_At[MAX_STRING_LENGTH];//pinakas gia to pou menei
	char Year_work[MAX_STRING_LENGTH];
	int matched;//xrhsimopoieitai gia to Query 1 gia na dhlwnei oti exei ginei upopshfio match

	double Trust;


	struct RNodeList *posts;//lista apo posts
	struct RNodeList *interests;//lista endiaferontwn
	struct RNodeList *likes;//lista me posts pou exei kanei like
	struct e_list *working_with;//lista me atoma pou douleuoun me auto to atomo
	struct e_list *studying_with;//lista me atoma pou spoudazoun me auto to atomo
	struct e_list *living_with;//lista me atoma pou menoun sthn idia perioxh me auto to atomo

	struct FNodeList *M_F;//lista me ta forums pou einai melos
	

}Node;


//komvos mias ouras h opoia xrhsimopoietai sto BFS
typedef struct VQNode{

	Node *node;//o komvos tou grafou pou periexetai ston komvo auths ths ouras
	struct VQNode *next;//epomenos
	struct VQNode *prev;//proigoumenos	
	

}VQNode;

//Oura pou xrhsimopoieitai sto BFS
typedef struct VQueue{
	
	int startID;//kratame ws eswterikh plhroforia to ID tou komvou pou 3ekinaei to BFS
	VQNode *head;//Arxh ouras
	VQNode *tail;//Telos ouras
	int total_nodes;//plh8os komvwn
	int size;//mege8os ouras

}VQueue;

//domh ths akmhs
typedef struct Edge{

	int EdgeID;
	int bet_num;
	int id;//h arxh ths akmhs
	int end;//to telos ths akmhs
	Node *n_node;//deikths ston geitoniko komvo
	Properties *edge_prop;//domh pou perixei ta attributes ths akmhs
	struct Edge *next;//epomenos
	struct Edge *prev;//prohgoumenos
	

}Edge;

int Fix_EdgeID(int,int);
//lista pou periexei tous geitones ka8e komvou
typedef struct e_list{

	int owner_id;
	Edge *head;//Arxh
	Edge *tail;//telos
	int size;//mege8os
}e_list;

//Melh tou sunektikou grafou
typedef struct CGMebers{

	Node *Member;

}CGMebers;

//komvos ths listas me id ekproswpwn sunektikwn grafwn
typedef struct CGLNode {

	int Repr_node_id;//ID antiproswpou
	int node_num;//posous komvous exei to ka8e sunektiko grafima
	CGMebers *CGmembers;//pinakas komvwn pou einai melh tou sunektikou grafou
	int max_size;
	struct CGLNode *next;
	struct CGLNode *prev;

}CGLNode;


//Lista me ekproswpous sunektikwn grafwn
typedef struct Coherent_Graph_List {

	CGLNode *head;//Arxh
	CGLNode *tail;//telos
	int CGmax_ID;//to id tou megaluterou sunektikou grafou
	int max_nodes_of_CG;//megistos ari8mos komvwn tou megalyterou sunektikou grafou
	int size;//mege8os
	

}Coherent_Graph_List;

//domh tou grafou
typedef struct Graph{

	char Forum_Name[MAX_STRING_LENGTH];
	int Forum_ID;
	struct Cell *hash_table;//deikths se hash_table me to opoio ginetai katakermatismos komvwn
	struct Cell *edge_hash;
	struct e_list *graph_edge_list;

	int total_nodes;//ph8os komvwn
	int max_edges;//to plh8os twn perissoterwn akmwn apo olous tous komvous
	int total_edges;//to sunoliko plh8os twn akmwn
	Coherent_Graph_List *CGL;//lista pou periexei id ekproswpwn sunektikwn grafwn	
	Coherent_Graph_List *prev_CGL;
	int cur_bridgeID;//xrhsimopoieitai gia na vre8oun ola ta pi8ana monopatia pou empleketai o sugkekrimenos komvos ws gefura
	int betweeness_num;//poses fores exei vre8ei ws gefura o epilegmenos komvos
	
	struct FNodeList *Forums;
	struct Cell *h_ORG;//hash_table gia organismous
	struct Cell *h_INT;//hash_table gia endiaferonta	
	struct Cell *h_PL;//hash_table gia topo8esies pou menoun ta atoma
	struct Cell *h_POSTS;//hash_table gia ta posts
	struct Cell *h_FORUMS;//hash_table gia ta forums
	struct Cell *h_COM;//hash_table gia ta comments	
	
	struct Graph *M_graph;//grafos gia andres
	struct Graph *W_graph;//grafos gia gunaikes
}Graph;

//sunarthseis gia sunektikous grafous
void RemoveCGL(Coherent_Graph_List *,int);
void Initialize_CGL(Coherent_Graph_List **);
void DestroyCGL(Coherent_Graph_List *);
void CGL_Link(CGLNode * ,Coherent_Graph_List *);
void IncreaseNodesofCG(Coherent_Graph_List *,int,Node *);//pros8etei melh sthn CGL
void Connect_Coherent_Graphs(CGLNode*,CGLNode*,Graph*);
void Fix_graph_Members(Graph *,int ,Node *);	

void DestroyPropArray(Properties *);//katastrofh tou pinaka me ta attributes

void PrintEList(Node *);//ektupwnei tous geitones ka8e komvou(gia debugging mono)
void ELink(Edge * ,e_list *);//sundeei komvous ths listas geitniashs
Edge *Create_Edge(int , int ,Properties *);//dhmiourgei mia akmh kai epistrefei ena deikth se auth
void Initialize_E_List(e_list **,int);//Dhmiourgei kai arxikopoiei thn lista geitniashs
Node *GNode_Creation(int,Properties*);//Dhmiourgei enan komvo kai epistrefei ena deikth se auton
Graph *Graph_Create(int, int);//Dhmiourgei enan grafo kai epistrefei enan deikth se auton
void InsertNode(Node *,Graph *);//Eisagei komvous ston grafo
void InsertEdge(int,Edge*, Graph *);//Eisagei akmes ston grafo
void DestroyEList(e_list *);//kastastrefei thn lista geitniashs
Node *LookupNode(Graph *,int);//epistrefei enan deikth se enan komvo tou grafou
void DestroyGraph(Graph *);//katastefei ton grafo
int ReachNode1(int , int ,Graph *);//epistrefei to shntomotero monopati apo enan komvo ston allon

/*BFS functions*/
void Initialize_VQueue(VQueue **,int,int);//Arxikopoiei kai dhmiourgei thn oura pou xrhsimopoieitai sto BFS
void VLink(Node *,VQueue *,Node*);//Sundeei tous komvous ths ouras gia to BFS
void RecursiveBFS(Node *node,int endID,VQueue *,struct TNodeList *,int *,Graph *,struct Cell *);
void RemoveVQueue(VQueue *);//Afairei ton prwto komvo ths ouras

int Backtrack_path(TNodeList *,Node *,Graph *,int);
void DestroyVQueue(VQueue *Vqueue);//katastrefei thn oura tou BFS
ResultSet* ReachNodeN(int,Graph *g);//Epistrefei to resultSet me ta suntomotera monopatia gia enan komvo pros olous
void BFS_ALL(Node *,VQueue *,VQueue *);//BFS gia enan komvo pros olous

Node* setPersonProperties(int, char*, char*, int,char*);/* Creates a node person and sets its properties */
Edge* setEdgeProperties(int, int, char*, double);/* Creates an edge between two persons and sets its properties */
void printPersonProperties(Node* );/* Prints a person's properties */
void printEdgeProperties(Edge*);//* Prints an edge's properties */

////////////////////////////////*****Read from files*****///////////////////////////////////////////////////
void Insert_Persons(Graph*,char *);
void Insert_Edges(Graph*,char *);
void Table_Input(struct Cell*,char*,struct Graph *,char *);
void Insert_Relation(Graph *,char*,char*);

void Insert_Tables(Graph *,int);
void Insert_Infos(struct RNode *,Node *,char *);

void Insert_Levels(Node *n,Graph *g);

int ReachNode12(int ,int,Graph *);

FNode *create_Forum_node(int , char *);
void Forum_container_of_posts(struct Cell *,struct Cell *);




#endif



