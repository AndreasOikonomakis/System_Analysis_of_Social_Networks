#ifndef TR_U_H
#define TR_U_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"
#include "GraphLib.h"
#include "graph.h"
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "Forum_graph.h"

typedef struct Task_Args{

	struct FNodeList *Forums;
	struct NForums *NF;	
	struct GraphList *GL;
	struct NF_Node *cur_forum;
	int done;
	int cur_NF_forum;
	int done2;

}Task_Args;


//komvos job ths ouras douleiwn/energeiwn
typedef struct job{
	void *(*function)(void* arg);//deikths se sunartish
	void *arg;// orismata sunarthshs
	struct job *next;//epomeno job
	struct job *prev;//proigoumeno job
}job;

//lista me douleies
typedef struct jobQueue{
	job *head;//deikths sthn arxh ths ouras
	job *tail;//deikths sto telos ths ouras
	int jobs_num;//ari8mos douleiwn sthn oura
	sem_t *queueSem;//semaforos ths ouras douleiwn
}jobQueue;

//domh tou threadpool
typedef struct threadPool{
	pthread_t *threads;//deikths se tread id
	int threads_num;//ari8mos thread
	jobQueue *jobqueue;//deikths sthn oura me douleies
}threadPool;

//domh me antikeimena gia ka8e thread
typedef struct thread_infos{                            
	pthread_mutex_t *mutex_p;//deikths se mutex
	threadPool *threadPool_pointer;//deikths se threadPool
}thread_infos;

//////////////////////////////////////////////Sunarthseis//////////////////////////////////////////////////////////////

void InsertGraphList(struct GraphList *,struct Graph *);

threadPool *threadPool_Creation(int );//dhmiourgia threadPool
void Thread_Action(threadPool *);//sunarthsh energeiwn twn tread
void Add_Job_To_ThreadPool(threadPool *,void *(*function_p)(void*), void *);//ana8esh douleias sth oura ergasiwn tou thread pool
void Destroy_ThreadPool(threadPool *);//katastrofh tou threadPool
void jobQueue_Creation(threadPool *);//dhmiourgia ouras ergasiwn
void Add_Job(threadPool *, job *);//pros8hkh ergasias sthn oura
int Remove_Job(threadPool *);//Afairesh ergasias apo thn oura
job *Return_Job(threadPool *);//Epistrofh ths ergasias pou einai na ektelestei
void Empty_jobQueue(threadPool *);

struct GraphList *Return_NForums_Graphs(struct FNodeList *,int);

void Find_NForums(struct Task_Args *);
void Create_F_Graphs(struct Task_Args *);

#endif


