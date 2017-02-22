#include<stdio.h>
#include<stdlib.h>
#include"hashing.h"
#include"graph.h"
#include<errno.h>
#include"GraphLib.h"

#define PROPERTIES_NUM 2




//sunarthsh linear hashing
int Hashing_function(int round, int m, int value){

	return value%(Pow(round)*m);

}
//sunarthsh dunamhs gia to linear hashing
int Pow(int p){

	int i;
	int x=1;
	if(p == 0)
		return 1;
	else{
		for(i=0;i<p;i++)
			x = x*2;
	}
	return x;

}

TNode *TNode_Creation(int id){//dhmiourgia komvou gia hash_table

	TNode *node;
	node = malloc(sizeof(struct TNode));
	node->id = id;
	node->data = NULL;
	return node;
}


//Dhmiourgia hash_table
Cell  *Create_T_Hash_table(int h_table_size, int bucket_size){

	int i;
	struct Cell *hash_table;
	hash_table = malloc(sizeof(struct Cell));
	//Arxhkopoihseis plhroforiwn gia to hash_table
	hash_table->info.hash_table_size = h_table_size;//trexon mege8os hash_table
	hash_table->info.split_pointer = 0;//deikths gia poio keli 8a kanei split
	hash_table->info.round = 0;//trexon round
	hash_table->info.round_flag =0;
	hash_table->info.sorted = 0;
	hash_table->info.m = h_table_size;//menei sta8ero
	hash_table->info.fixed_bucket_size = bucket_size;	
	//dhmiourgia twn buckets gia ka8e keli tou hash_table
	hash_table->bucket = malloc(sizeof(t_bucket)*h_table_size);


	//Arxhkopoihseis gia ka8e bucket(pinakas)
	for(i=0;i<h_table_size;i++){
		hash_table->bucket[i].t_node = malloc(sizeof(TNode)*bucket_size);
		hash_table->bucket[i].cur_bucket_size =0;//trexon mege8os bucket pou einai arxika mhden
		hash_table->bucket[i].cell_num =i;//ari8mos keliou
		hash_table->bucket[i].max_bucket_size = bucket_size;
		hash_table->bucket[i].splitted_flag =0;//dhlwnei an exei ginei split to bucket
		hash_table->bucket[i].sorted = 0;
	}
	//printf("Dhmiourgia hash_table mege8ous %d kai mege8ous bucket %d\n",hash_table->info.m,bucket_size);	
	return hash_table;
	
}

//Eisagwgh komvwn tou hash_table
void T_InsertNode(TNode *node,Cell *Hash_table){

	int pos,value;
	int cur_b_size;
	value = node->id;
	pos =	Hashing_function(Hash_table->info.round,Hash_table->info.m,value);
	//printf("to %d 8a mpei sto %d\n",value,pos);
	if(Hash_table->bucket[pos].splitted_flag!=0){//enan exei ginei hdh split se auth th 8esh xrhsimopoioume thn h(i+1)
		//printf("To splitted_flag tou %d einai 1\n",pos);
		pos =	Hashing_function(Hash_table->info.round + 1,Hash_table->info.m,value);	
		//printf("to %d tlk 8a mpei sto %d\n",value,pos);
	}	
	//printf("to %d 8a mpei sto %d\n",value,pos);

	if(Hash_table->bucket[pos].cur_bucket_size<Hash_table->bucket[pos].max_bucket_size){
		cur_b_size = Hash_table->bucket[pos].cur_bucket_size;
		Hash_table->bucket[pos].t_node[cur_b_size] = *node;//eisagwgh tou komvou hash_table
		Hash_table->bucket[pos].cur_bucket_size++;
		Hash_table->bucket[pos].sorted = 0;
		//if(Hash_table->bucket[pos].cur_bucket_size>1)
			//T_quicksort(&Hash_table->bucket[pos].t_node,0,Hash_table->bucket[pos].cur_bucket_size-1);
	}
	else{
			T_Overflow_Manager(Hash_table,pos,node);

		
	}	

}

void T_Overflow_Manager(struct Cell *Hash_table,int cell_num,TNode *node){

	//printf("OVERFLOW GIA TO %d sto bucket %d\n",node->id,cell_num);
	int split_pointer;
	split_pointer = Hash_table->info.split_pointer;
	int size_b,size_h,i=0;
	size_b = Hash_table->bucket[cell_num].max_bucket_size;
	TNode *b;
	//printf("diplasiasmos tou bucket %d mege8ous %d\n",cell_num,Hash_table->bucket[cell_num].max_bucket_size);
	Hash_table->bucket[cell_num].max_bucket_size = size_b*2;
	b = realloc(Hash_table->bucket[cell_num].t_node,sizeof(TNode)*Hash_table->bucket[cell_num].max_bucket_size);
	Hash_table->bucket[cell_num].t_node = b;
	//printf("to bucket %d egine mege8ous %d\n",cell_num,Hash_table->bucket[cell_num].max_bucket_size);
	
	
	size_b =Hash_table->bucket[cell_num].cur_bucket_size;//to size_b ginetai to trexon mege8os tou bucket
	Hash_table->bucket[cell_num].t_node[size_b] = *node;
	//printf("eisodos tou %d sto bucket %d sth 8esh %d \n",node->id,cell_num,size_b);
	Hash_table->bucket[cell_num].cur_bucket_size++;

	Hash_table->bucket[cell_num].sorted = 0;
		//T_quicksort(&Hash_table->bucket[cell_num].t_node,0,Hash_table->bucket[cell_num].cur_bucket_size-1);

	

/////////////////////au3hsh tou hash_table kata ena/////////////////////////////////////////////////////////////
	//printf("au3hsh tou hash_table pou einai mege8ous %d\n",Hash_table->info.hash_table_size);
	Hash_table->bucket[split_pointer].splitted_flag =1;
	//printf("to splitted flag tou %d egine %d\n",cell_num,Hash_table->bucket[cell_num].splitted_flag);
	t_bucket *h;
	Hash_table->info.hash_table_size++;
	size_h = Hash_table->info.hash_table_size;
	h = realloc(Hash_table->bucket,sizeof(t_bucket)*size_h);
	Hash_table->bucket = h;
	//printf("arxikopoihsh tou neou bucket 8eshs %d\n",size_h-1);
	Hash_table->bucket[size_h-1].t_node = malloc(sizeof(TNode)*Hash_table->info.fixed_bucket_size);
	Hash_table->bucket[size_h-1].cur_bucket_size =0;
	Hash_table->bucket[size_h-1].cell_num =i;
	Hash_table->bucket[size_h-1].max_bucket_size = Hash_table->info.fixed_bucket_size;
	Hash_table->bucket[size_h-1].splitted_flag =0;
	Hash_table->bucket[size_h-1].sorted =0;
	//printf("au3hsh tou hash_table egine mege8ous %d\n",Hash_table->info.hash_table_size);
//////////////////////anakatanomh twn stoixeiwn tou bucket pou egine split/////////////////////////////////////
	int r_value;//re-hash value : ta stoixeia tou splitted bucket 8a 3anakanoun hashing
	int pos;
	//printf("anakatanomh tou bucket %d mege8ous %d\n",split_pointer,Hash_table->bucket[split_pointer].cur_bucket_size);
	int cur_b_size1=0;//trexon size tou paliou bucket 
	int cur_b_size2=0;//trexon size tou kanourgiou bucket
	int split_size=Hash_table->bucket[split_pointer].cur_bucket_size;
	Hash_table->bucket[split_pointer].cur_bucket_size=0;
	for(i=0;i<split_size;i++){//anakatanomh komvwn
		//tn = Hash_table->bucket[split_pointer].t_node[i];
		r_value = Hash_table->bucket[split_pointer].t_node[i].id;
		//printf("REHASH WITH %d\n",Pow(Hash_table->info.round+1)*Hash_table->info.m);
		pos =	Hashing_function(Hash_table->info.round+1,Hash_table->info.m,r_value);
		if(pos == split_pointer){
			Hash_table->bucket[pos].t_node[cur_b_size1] = Hash_table->bucket[split_pointer].t_node[i];
			cur_b_size1++;
			(Hash_table->bucket[pos].cur_bucket_size)++;
			//printf("To %d 8a paei sto palio bucket 8eshs %d\n",r_value,pos);
		}
		else{
			//diplasiasmos se periptwsh pou to kainourgio bucket den xwraei thn anakatanomh
			if(Hash_table->bucket[pos].cur_bucket_size == Hash_table->bucket[pos].max_bucket_size){
				size_b = Hash_table->bucket[pos].max_bucket_size;
				Hash_table->bucket[pos].max_bucket_size = size_b*2;
				b = realloc(Hash_table->bucket[pos].t_node,sizeof(TNode)*Hash_table->bucket[pos].max_bucket_size);
				Hash_table->bucket[pos].t_node = b;
			}
			Hash_table->bucket[pos].t_node[cur_b_size2] = Hash_table->bucket[split_pointer].t_node[i];
			cur_b_size2++;
			(Hash_table->bucket[pos].cur_bucket_size)++;
			//printf("To %d 8a paei sto kainourgio bucket 8eshs %d\n",r_value,pos);
		}
	
	}

	Hash_table->bucket[split_pointer].sorted=0;

	Hash_table->bucket[size_h-1].sorted = 0;



	int m;

	split_pointer = Hash_table->info.split_pointer;
	m = Hash_table->info.m;
	int round =Hash_table->info.round;
	//ean o split_pointer eftase sto mege8os m*((*Hash_table)->info.round+1) oloklhrw8hke ena round
	if(Hash_table->info.split_pointer == m*(Pow(round))-1){
		//printf("-------------->OLOKLHRW8HKE to round giati %d\n",m*(Pow(round)));		
		
		Hash_table->info.round_flag = 1;
	//arxikopoihsh twn splitted_flag gia ka8e keli tou hash_table
		for(i=0;i<m*(Pow(round));i++){
			Hash_table->bucket[i].splitted_flag =0; 
			///printf("To splitted_flag tou %d egine 0\n",i);
				
		}
		(Hash_table->info.round)++;
	}

	if(Hash_table->info.round_flag == 1){
		Hash_table->info.round_flag=0;
		Hash_table->info.split_pointer =0;
	}
	else
		(Hash_table->info.split_pointer)++;//au3hsh ton split_pointer	
	//printf("Split pointer is %d\n",Hash_table->info.split_pointer);

		
}


void T_quicksort(TNode **node,int first,int last){
 	
   int pivot,j,i;
	TNode *Array,tmp;		
	Array=*node;
	
   if(first<last){
         pivot=first;
         i=first;
         j=last;
         while(i<j){
             while(Array[i].id<=Array[pivot].id&&i<last)
                 i++;
             while(Array[j].id>Array[pivot].id)
                 j--;
             if(i<j){
                 
					  tmp = Array[i];                 
					  Array[i] = Array[j];			
					  Array[j] = tmp;			 
            }
         }

 			tmp=Array[pivot];
         Array[pivot]=Array[j];
			Array[j] = tmp;	

         T_quicksort(&Array,first,j-1);
         T_quicksort(&Array,j+1,last);

    }
}


void Destroy_T_Hash_table(struct Cell *Hash_table){//katastrofh hash_table

	int i=0,j=0,k=0;
	struct Cell *hash_table;
	hash_table = Hash_table;
	for(i=0;i<hash_table->info.hash_table_size;i++){		

		for(j=0;j<hash_table->bucket[i].cur_bucket_size;j++){


			if(hash_table->bucket[i].t_node[j].Nptr->knows_hash_table!=NULL){

				for(k=0;k<hash_table->bucket[i].t_node[j].Nptr->knows_hash_table->info.hash_table_size;k++)
					free(hash_table->bucket[i].t_node[j].Nptr->knows_hash_table->bucket[k].t_node);	
					
				free(hash_table->bucket[i].t_node[j].Nptr->knows_hash_table->bucket);
				free(hash_table->bucket[i].t_node[j].Nptr->knows_hash_table);	
			}


			if(hash_table->bucket[i].t_node[j].Nptr->E_List!=NULL)
				DestroyEList(hash_table->bucket[i].t_node[j].Nptr->E_List);


			if(hash_table->bucket[i].t_node[j].Nptr->likes!=NULL){
				free(hash_table->bucket[i].t_node[j].Nptr->likes->RArray);
				free(hash_table->bucket[i].t_node[j].Nptr->likes);
			}



			if(hash_table->bucket[i].t_node[j].Nptr->interests!=NULL){
				free(hash_table->bucket[i].t_node[j].Nptr->interests->RArray);
				free(hash_table->bucket[i].t_node[j].Nptr->interests);
			}

			if(hash_table->bucket[i].t_node[j].Nptr->posts!=NULL){
				free(hash_table->bucket[i].t_node[j].Nptr->posts->RArray);
				free(hash_table->bucket[i].t_node[j].Nptr->posts);
			}


			if(hash_table->bucket[i].t_node[j].Nptr->MSG!=NULL){
				free(hash_table->bucket[i].t_node[j].Nptr->MSG->GNodeArray);
				free(hash_table->bucket[i].t_node[j].Nptr->MSG);
			}

			if(hash_table->bucket[i].t_node[j].Nptr->WSG!=NULL){
				free(hash_table->bucket[i].t_node[j].Nptr->WSG->GNodeArray);
				free(hash_table->bucket[i].t_node[j].Nptr->WSG);
			}

	
			if(hash_table->bucket[i].t_node[j].Nptr->node_prop!=NULL){
				DestroyPropArray(hash_table->bucket[i].t_node[j].Nptr->node_prop);
				free(hash_table->bucket[i].t_node[j].Nptr);
			}

		}
		free(hash_table->bucket[i].t_node); 

	}			
	free(hash_table->bucket);
	free(hash_table);

}


TNode *T_Search(struct Cell *Hash_table,int value){

	//suntetagmenes pos gia 8esh keliou kai place gia 8esh sto bucket
	int pos=-1;
	int place=-1;
	int first;
	int last;
	int middle;

	

	//arxiko hash gia me thn h(i)
	
	pos =	Hashing_function(Hash_table->info.round, Hash_table->info.m,value);
	
	
	//enan h 8esh pou proekupse einai mikroterh tou split_pointer kai to keli auto exei ginei splitted tote kanoume rehash me thn h(i+1)
	if(pos<Hash_table->info.split_pointer && Hash_table->bucket[pos].splitted_flag!=0)	
		pos =	Hashing_function(Hash_table->info.round + 1, Hash_table->info.m,value);

	if(Hash_table->bucket[pos].sorted ==0){
			T_quicksort(&Hash_table->bucket[pos].t_node,0,Hash_table->bucket[pos].cur_bucket_size-1);
			Hash_table->bucket[pos].sorted=1;
	}


	//koitame thn mesh tou bucket kai an to zhtoumeno einai megalutero h mikrotero kanoume thn analogh anazhthsh pros ta mprosta h pisw
	if(Hash_table->bucket[pos].cur_bucket_size>0){
		//cur = Hash_table->bucket[pos].cur_bucket_size /2;

		first=0;
		last = Hash_table->bucket[pos].cur_bucket_size-1;
		while(first <= last){

			middle = (first + last)/2;
			//printf("vriskomai sto %d\n",Hash_table->bucket[pos].t_node[middle].id);
  
			if(Hash_table->bucket[pos].t_node[middle].id == value){
					//printf("found %d\n",Hash_table->bucket[pos].t_node[middle].id);
					place = middle;
					break;			
			}			
			else if (Hash_table->bucket[pos].t_node[middle].id > value)
				last = middle-1;			
			else
         	first = middle+1;
 
      	
		}

	}	
	if(place == -1){//To stoixeio den uparxei
		//printf("To %d den yparxei\n",value);
		return NULL;
	}
	else{
		TNode *tn = &Hash_table->bucket[pos].t_node[place];
		return tn;
	}
}	

void Destroy_T_Hash_table2(struct Cell *Hash_table){//katastrofh hash_table

	int i=0,j=0;
	struct Cell *hash_table;
	hash_table = Hash_table;
	for(i=0;i<hash_table->info.hash_table_size;i++){		

		for(j=0;j<hash_table->bucket[i].cur_bucket_size;j++){
			if(hash_table->bucket[i].t_node[j].RNptr->E_List!=NULL)
				DestroyEList(hash_table->bucket[i].t_node[j].RNptr->E_List);

			if(hash_table->bucket[i].t_node[j].RNptr->node_prop!=NULL)
				DestroyPropArray(hash_table->bucket[i].t_node[j].RNptr->node_prop);

			if(hash_table->bucket[i].t_node[j].RNptr!=NULL)
				free(hash_table->bucket[i].t_node[j].RNptr);
			
			//if(hash_table->bucket[i].t_node[j].data!=NULL)
				//free(hash_table->bucket[i].t_node[j].data);


			/*if(hash_table->bucket[i].t_node[j].Hptr!=NULL)
				Destroy_T_Hash_table2(hash_table->bucket[i].t_node[j].Hptr);

			if(hash_table->bucket[i].t_node[j].FNptr!=NULL){

				if(hash_table->bucket[i].t_node[j].FNptr->Forum_Members!=NULL){

						free(hash_table->bucket[i].t_node[j].FNptr->Forum_Members->GNodeArray);
						free(hash_table->bucket[i].t_node[j].FNptr->Forum_Members);
						if(hash_table->bucket[i].t_node[j].FNptr->likes_to_from!=NULL)
							Destroy_T_Hash_table2(hash_table->bucket[i].t_node[j].FNptr->likes_to_from);

						if(hash_table->bucket[i].t_node[j].FNptr->replies_to_from!=NULL)
							Destroy_T_Hash_table2(hash_table->bucket[i].t_node[j].FNptr->replies_to_from);


				}


			}*/

		}
		if(hash_table->bucket[i].t_node!=NULL)
			free(hash_table->bucket[i].t_node); 

	}			
	free(hash_table->bucket);
	free(hash_table);

}

void Destroy_T_Hash_table3(struct Cell *Hash_table){//katastrofh hash_table

	int i=0,j=0,k=0,m=0;
	struct Cell *hash_table,*h;
	hash_table = Hash_table;
	for(i=0;i<hash_table->info.hash_table_size;i++){		

		for(j=0;j<hash_table->bucket[i].cur_bucket_size;j++){

			

			if(hash_table->bucket[i].t_node[j].FNptr!=NULL){

				if(hash_table->bucket[i].t_node[j].FNptr->Forum_Members!=NULL){

						free(hash_table->bucket[i].t_node[j].FNptr->Forum_Members->GNodeArray);
						free(hash_table->bucket[i].t_node[j].FNptr->Forum_Members);
						if(hash_table->bucket[i].t_node[j].FNptr->likes_to_from!=NULL){
							h = hash_table->bucket[i].t_node[j].FNptr->likes_to_from;
							for(k=0;k<h->info.hash_table_size;k++){
								
								for(m=0;m<h->bucket[k].cur_bucket_size;m++){
							
										if(h->bucket[k].t_node[m].data!=NULL)					
											free(h->bucket[k].t_node[m].data);			
								}
					
								free(h->bucket[k].t_node); 

							}

						}
						//if(hash_table->bucket[i].t_node[j].FNptr->replies_to_from!=NULL)
							//Destroy_T_Hash_table3(hash_table->bucket[i].t_node[j].FNptr->replies_to_from);
			
					
				}
				free(hash_table->bucket[i].t_node[j].FNptr);

			}

		}
		if(hash_table->bucket[i].t_node!=NULL)
			free(hash_table->bucket[i].t_node); 

	}			
	free(hash_table->bucket);
	free(hash_table);

}








