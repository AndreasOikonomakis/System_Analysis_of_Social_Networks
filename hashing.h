#ifndef HASHING_H
#define HASHING_H


#include"graph.h"

//////////////////////////////////////////TABLES/////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


typedef struct TNode{// komvos tou hash_table

	int id;//id tou komvou tou hash_table
	struct RNode *RNptr;//deikths se opoiodhpote komvo ektos komvou grafou
	struct Node *Nptr;//deikths se komvo grafou	
	struct FNode *FNptr;//deikths se komvo forum
	struct Cell *Hptr;//deikths se hash_table
	void *data;	
}TNode;

typedef struct t_bucket{//bucket tou hash_table

	struct TNode *t_node;//deikths se komvo tou hash_table
	int cur_bucket_size;//trexwn mege8os tou bucket
	int cell_num;//ari8mos keliou
	int max_bucket_size;//megisto mege8os bucket
	int splitted_flag;//metavlhth gia ton an exei ginei split h oxi
	int sorted;
	
}t_bucket;

//domh gia diafores plhrofories tou hash_table
typedef struct t_info {

	int m;//arxiko mege8os hash_table
	int hash_table_size;//trexon mege8os tou hash_table
	int split_pointer;//o deikths tou keliou pou 8a ginei split
	int round;//o guros tou hashing
	int fixed_bucket_size;//to standar mege8os tou bucket
	int round_flag;//dhlwnei to an exei alla3ei round
	int sorted;
}t_info;


//domh cell , sthn ousia to hash_table
typedef struct Cell{

	struct t_bucket *bucket;//to bucket ka8e keliou
	struct t_info info;//plhrofories sxetika me to hash_table

}Cell;

TNode *TNode_Creation(int);
RNode *R_setProperties(int ,char *);
Cell *Create_T_Hash_table(int,int);//dhmiourgia hash_table
void Destroy_T_Hash_table(struct Cell *);//katastrofh hash_table pou periexei komvous grafou
void T_quicksort(TNode **,int ,int);//ta3inomish komvwn
void T_Overflow_Manager(struct Cell *,int,TNode*);//dieu8ethsh tou overflow me katallhles energeies
TNode *T_Search(struct Cell *,int);//anazhthsh
void T_InsertNode(TNode *,Cell*);//Eisagei komvous ston hash_table
void Destroy_T_Hash_table2(struct Cell *);//katastrofh hash_table pou periexei komvous tupou RNode
void Destroy_T_Hash_table3(struct Cell *);

int Hashing_function(int,int,int);//sunarthsh linear hashing
int Pow(int);//sunarthsh dunamhs gia thn sunarthsh linear hashing

#endif


