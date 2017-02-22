#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"graph.h"
#include"hashing.h"
#include"defines.h"
#include"Queries.h"

RNode *RNode_Creation(int id,Properties *prop){

	struct RNode *node;
	node = malloc(sizeof(struct RNode));
	node->node_prop = prop;
	node->freq = 0;
	node->matched = 0;
	node->matched_hash_table = NULL;
	node->max_freq=0;
	node->id = id;
	node->E_List = NULL;
	node->owner = NULL;
	node->container_forum = NULL;
	return node;
}

RNode *R_setProperties(int id,char *name){

	 /*create properties*/
    Properties* prop = createProperties(PROPERTIES_NUM);
    setStringProperty(name, 0, prop);
    /*create t_node*/
	
    RNode* n = RNode_Creation(id, prop);

    return n;

} 

 


//k num_of_common_inter , h num of steps, x age diffrence, limit max matches
Matches *matchSuggestion(Node *node, int k2, int h, int x, int limit, Graph* g){ 

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////**********Anazhthsh twn matches*****************////////////////////////////////////////////////////////////
	int i;
	int age;//hlikia tou endiaferomenou
	int age_cand;//hlikia upopshfiou
	int m =2,c=3;
	
	
	Matches *MA;
	MA = malloc(sizeof(Matches));
	MA->size =0;
	MA->max_size = 4;
	MA->mArray = malloc(sizeof(Match)*MA->max_size);
	struct Cell *matched_hash_table;
	matched_hash_table = Create_T_Hash_table(m,c);
	TNode *tnode = NULL;

	for(i=0;i<MA->max_size;i++){
		MA->mArray[i].m_node = NULL;
		MA->mArray[i].score = -1;
	}
	age = atoi(node->node_prop->pArray[2]);
	Edge *m1,*m2,*m3;
	
	//zoune sthn idia perioxh/spoudazoun-douleuoun ston idio organismo/einai anti8etou filou
	if(node->living_with!=NULL){
		m1 = node->living_with->head;
		for(i=0;i<node->living_with->size;i++){
			age_cand = atoi(m1->n_node->node_prop->pArray[2]);
			if(strcmp(m1->n_node->node_prop->pArray[3],node->node_prop->pArray[3])&&ReachNode1(node->id,m1->n_node->id,g)>1 &&(age_dif			(age,age_cand)<=x) && ReachNode1(node->id,m1->n_node->id,g)<=h){
			
				tnode = malloc(sizeof(TNode));
				tnode->id = m1->n_node->id;			
				T_InsertNode(tnode,matched_hash_table);
				MArray_Insert(m1->n_node,0,0,MA);
				free(tnode);
			}
			m1 = m1->next;
		}
	}
	if(node->working_with!=NULL){
		m2 = node->working_with->head;

		for(i=0;i<node->working_with->size;i++){
			age_cand = atoi(m2->n_node->node_prop->pArray[2]);
			if(strcmp(m2->n_node->node_prop->pArray[3],node->node_prop->pArray[3]) && ReachNode1(node->id,m2->n_node->id,g)>1&&(age_dif(age,age_cand)<=x) &&ReachNode1(node->id,m2->n_node->id,g)<=h && T_Search(matched_hash_table,m2->n_node->id)==NULL){
				
				
				tnode = malloc(sizeof(TNode));					
				tnode->id = m2->n_node->id;			
				T_InsertNode(tnode,matched_hash_table);
				MArray_Insert(m2->n_node,0,0,MA);
				free(tnode);
			}
			m2 = m2->next;
		}
	}


	if(node->studying_with!=NULL){
		m3 = node->studying_with->head;
		for(i=0;i<node->studying_with->size;i++){
			age_cand = atoi(m3->n_node->node_prop->pArray[2]);
			if(strcmp(m3->n_node->node_prop->pArray[3],node->node_prop->pArray[3])&& ReachNode1(node->id,m3->n_node->id,g)>1&&(age_dif(age,age_cand)<=x) &&ReachNode1(node->id,m3->n_node->id,g)<=h && T_Search(matched_hash_table,m3->n_node->id)==NULL){
							
			tnode = malloc(sizeof(TNode));
			tnode->id = m1->n_node->id;			
			T_InsertNode(tnode,matched_hash_table);		
			MArray_Insert(m3->n_node,0,0,MA);
			free(tnode);
			}
			m3 = m3->next;
		}
	}

	int j,k;
	RNodeList *cur_list;

	//printf("opposite genre neighbors/working/studying together are\n");
	for(i=0;i<MA->size;i++){

		cur_list = MA->mArray[i].m_node->interests;
		for(j=0;j<cur_list->size;j++)
			
			for(k=0;k<node->interests->size;k++){

				if(cur_list->RArray[j]->id == node->interests->RArray[k]->id)
					(MA->mArray[i].com_inter_num)++;
			}

	}

	for(i=0;i<MA->size;i++){
		MA->mArray[i].m_node->matched =0;
		if(MA->mArray[i].com_inter_num<k2)
			MA->mArray[i].score = -1;
		
	}
	
	for(i=0;i<MA->size;i++){	

	 if(MA->mArray[i].m_node == NULL || MA->mArray[i].score == -1)
		break;

		
	MA->mArray[i].score=1-(double)(MA->mArray[i].com_inter_num)/(double)(node->interests->size+MA->mArray[i].m_node->interests->size);
				

		
	 }

	quicksortMA(&MA->mArray,0,MA->size-1,MA->size);

	
	Matches *M;
	M = malloc(sizeof(Matches)*limit);
	M->mArray = malloc(sizeof(Match));
	
	for(i=0;i<limit;i++){
		M->mArray[i].m_node = MA->mArray[i].m_node;
		M->mArray[i].com_inter_num = MA->mArray[i].com_inter_num;
		M->mArray[i].score = MA->mArray[i].score;
	}


	free(MA->mArray);
	free(MA);


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////Katastrofh tou matched_hash_table///////////////////////////////////////////////////////
	for(j=0;j<matched_hash_table->info.hash_table_size;j++)
			free(matched_hash_table->bucket[j].t_node); 
	
	free(matched_hash_table->bucket);
	free(matched_hash_table);
//////////////////////////////////////////////////////////////////////////////////////////////////////////

	//printf("--->%d\n",M->mArray[i].m_node->id);
	return M;

} 

void MArray_Insert(Node *m_node,int inter_num,double score,Matches *MA){

	Match *ma;
	MA->size++;
	int size = MA->size;
   int i=size -1;
	if(MA->size == MA->max_size){
		MA->max_size = MA->max_size*2;
		ma = realloc(MA->mArray,MA->max_size*sizeof(Match));
		MA->mArray = ma;
		for(i;i<MA->max_size;i++){
			MA->mArray[size-1].m_node = NULL;
			MA->mArray[size-1].score = -1;

		}

	}
		
	MA->mArray[size-1].m_node = m_node;
	MA->mArray[size -1].com_inter_num=0;
	MA->mArray[size -1].score =0;
		

}

int age_dif(int age1,int age2){

	int dif;
	dif = age1 - age2;
	if(dif<0)
		dif=dif*(-1);
	return dif;
}

void quicksortMA(Match **MA,int first,int last,int size){
 
	
   int pivot,j,i;
	Match *Array,tmp;		
	Array =*MA;

  if(first<last){
         pivot=first;
         i=first;
         j=last;

         while(i<j){
				 
             while(Array[i].score <= Array[pivot].score && i<last)
                 i++;
             while(Array[j].score>Array[pivot].score)
                 j--;
             if(i<j){
										  
					  tmp = Array[i];
					  Array[i] = Array[j];	
					  Array[j] = tmp;	
             }
         }


 			tmp=Array[pivot];
			//printf("->Allazei to %lf me to %lf\n",tmp.score,Array[j].score);
         Array[pivot]=Array[j];
 
			Array[j] = tmp;	
         quicksortMA(&Array,first,j-1,size);
         quicksortMA(&Array,j+1,last,size);

    }
}

int getMatch(int i,Matches *match){

	return match->mArray[i].m_node->id;

}



