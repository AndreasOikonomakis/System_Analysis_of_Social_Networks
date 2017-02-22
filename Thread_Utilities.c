#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"
#include "GraphLib.h"
#include "graph.h"
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "Thread_Utilities.h"


static int active=1;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t fg_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t nf_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t main_thred_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond = PTHREAD_COND_INITIALIZER;


threadPool *threadPool_Creation(int threads_num){

	threadPool *tp_p;
	printf("Creating treadPool with %d threads\n",threads_num);	
	
	//dhmiourgia threadPool
	tp_p = (threadPool*)malloc(sizeof(threadPool));


	tp_p->threads=(pthread_t*)malloc(threads_num*sizeof(pthread_t));

	tp_p->threads_num = threads_num;
	
	//dhmiourgia ouras ergasiwn
	jobQueue_Creation(tp_p);

	//dhmiourgia semaforou gia thn oura
	tp_p->jobqueue->queueSem=(sem_t*)malloc(sizeof(sem_t));
	sem_init(tp_p->jobqueue->queueSem, 0, 0);//arxikopoihsh tou semaforou

	
	//dhmiougria thread tou threadPool
	int t;
	for (t=0; t<threads_num; t++)		
		pthread_create(&(tp_p->threads[t]), NULL, (void *)Thread_Action, (void *)tp_p);
	
	
	return tp_p;
}

void Thread_Action(threadPool *tp_p){

	while(active){
		
		sem_wait(tp_p->jobqueue->queueSem);


		if (active){
			
			void *(*func_buff)(void* arg);
			void *arg_buff;
			job *job_p;
	
/////////////////////Critical section ths ouras ergasiwn/////////////////////////////
			pthread_mutex_lock(&mutex);//kleidwse to mutex
			
			job_p = Return_Job(tp_p);
			func_buff = job_p->function;
			arg_buff = job_p->arg;
			Remove_Job(tp_p);
						
			pthread_mutex_unlock(&mutex);//3ekleidwse to mutex
////////////////Telos critical section ths ouras ergasiwn////////////////////////////			
			func_buff(arg_buff);//ektelese thn ergasia
			 
			free(job_p); 
		}
		else		
			return; 
		
	}
	return;
}

void Add_Job_To_ThreadPool(threadPool *tp_p, void *(*function_p)(void*), void *arg_p){
	job *newJob;
	
	newJob=(job*)malloc(sizeof(job)); 
	
	
	newJob->function=function_p;//deikths ths sunarthshs
	newJob->arg=arg_p;//deikths orismatwn
	
	/* add job to queue */
	pthread_mutex_lock(&mutex);                
	Add_Job(tp_p, newJob);
	pthread_mutex_unlock(&mutex); 
	

}


/* Destroy the threadpool */
void Destroy_ThreadPool(threadPool *tp_p){
	int i;
	
	//e3odos apo to while
	active=0; 

	//energopoihsh twn apenergopoihmenwn tread
	for (i=0; i<(tp_p->threads_num); i++)
		sem_post(tp_p->jobqueue->queueSem);
				
	

	//katastrofh tou semaforou
	sem_destroy(tp_p->jobqueue->queueSem);

	
	//perimenoume ta thread na teleiwsoun
	for (i=0; i<(tp_p->threads_num); i++)
		pthread_join(tp_p->threads[i], NULL);
	
	
	 Empty_jobQueue(tp_p);
	
	free(tp_p->threads);
	free(tp_p->jobqueue->queueSem);
	free(tp_p->jobqueue);                                                 
	free(tp_p);                                                          
}





///////////////////////////////////////////////Sunarthseis gia to jobQueue/////////////////////////////////////////////////////
//Dhmiougria ouras ergasiwn
void jobQueue_Creation(threadPool *tp_p){

	tp_p->jobqueue=(jobQueue*)malloc(sizeof(jobQueue));	
	tp_p->jobqueue->tail=NULL;
	tp_p->jobqueue->head=NULL;
	tp_p->jobqueue->jobs_num=0;
	
}

void Add_Job(threadPool *tp_p, job *newjob){

	newjob->next=NULL;
	newjob->prev=NULL;
	
	job *oldFirstJob;
	oldFirstJob = tp_p->jobqueue->head;
	
	
	switch(tp_p->jobqueue->jobs_num){
		
		//ean h oura ergasiwn einai kenh
		case 0:
					tp_p->jobqueue->tail=newjob;
					tp_p->jobqueue->head=newjob;
					break;
		
		//ean h oura exei hdh ergasies
		default:
					oldFirstJob->prev = newjob;
					newjob->next = oldFirstJob;
					tp_p->jobqueue->head = newjob;

	}

	(tp_p->jobqueue->jobs_num)++;//au3hsh tou ari8mou twn douleiwn
	sem_post(tp_p->jobqueue->queueSem);
	
	int sval;
	sem_getvalue(tp_p->jobqueue->queueSem, &sval);
}

int Remove_Job(threadPool *tp_p){

	job *oldLastJob;
	oldLastJob = tp_p->jobqueue->tail;
	
	switch(tp_p->jobqueue->jobs_num){
		
		//ean den uparxoun ergiasies sthn oura epestrepse -1
		case 0:
					return -1;
					break;
		
		//ean uparxei mia mono douleia
		case 1:
					tp_p->jobqueue->tail=NULL;
					tp_p->jobqueue->head=NULL;
					break;
					
		//ean uparxoun panw apo mia douleia sthn oura
		default: 
					oldLastJob->prev->next=NULL; 
					tp_p->jobqueue->tail=oldLastJob->prev;
					
	}
	
	(tp_p->jobqueue->jobs_num)--;//meiwsh tou ari8mou douleiwn sthn oura
	
	int sval;
	sem_getvalue(tp_p->jobqueue->queueSem, &sval);
	return 0;
}

job *Return_Job(threadPool *tp_p){
	return tp_p->jobqueue->tail;
}

void Empty_jobQueue(threadPool *tp_p){
	
	job *curjob;
	curjob = tp_p->jobqueue->tail;
	
	while(tp_p->jobqueue->jobs_num){
		tp_p->jobqueue->tail = curjob->prev;
		free(curjob);
		curjob = tp_p->jobqueue->tail;
		tp_p->jobqueue->jobs_num--;
	}
	
	/* Fix head and tail */
	tp_p->jobqueue->tail=NULL;
	tp_p->jobqueue->head=NULL;
}

void Find_NForums(struct Task_Args *tsk_args){//sunarthsh pou vriskei ta Nforums metrontas melh

	FNode *fn;
	struct FNodeList *Forums = tsk_args->Forums;
	NForums *NF = tsk_args->NF;

	//printf("Thread %u is counting members of Forum %d\n",(int)pthread_self(),Forums->FArray[Forums->cur]->id);



/////Critical section egrafhs sthn koinh domh me ta NForums////////////////////////////
	pthread_mutex_lock(&nf_mutex);//kleidwse to mutex
	if(Forums->FArray[Forums->cur]->Forum_Members!=NULL){
		fn = Forums->FArray[Forums->cur];

		if(NF->cur_size<NF->max_size || NF->max_size == 1)				
			Insert_NForums(NF,fn);					
		else if(fn->Forum_Members->size>NF->min_members)								
			Fix_NF(NF,fn);
			




	}

	(Forums->cur)++;
	

	if(Forums->cur == Forums->size){
		printf("Thread %u is terminating inserting process\n",(int)pthread_self());
		tsk_args->done = 1;	
		pthread_cond_signal(&cond);
	}

	pthread_mutex_unlock(&nf_mutex);//3ekleidwse to mutex
//////////////////Telos critical section ths koinhs domhs NForums///////////////////////

}

void Create_F_Graphs(struct Task_Args *tsk_args){//sunarthsh pou kalei thn create_forum_graph 


	Graph *F_Graph;
	
	pthread_mutex_lock(&fg_mutex);//kleidwse to mutex

	NF_Node *cur_forum = tsk_args->cur_forum;
	//printf("Thread %u is creating graph for forum %d\n",(int)pthread_self(),cur_forum->FNptr->id);
	
	tsk_args->cur_forum = tsk_args->cur_forum->next;
	
	pthread_mutex_unlock(&fg_mutex);//3ekleidwse to mutex

	F_Graph = Create_Forum_Graph(2,3,cur_forum->FNptr);//Dhmiourgia twn grafwn gia forums


	pthread_mutex_lock(&fg_mutex);//kleidwse to mutex
	
	InsertGraphList(tsk_args->GL,F_Graph);

	tsk_args->cur_NF_forum++;
	if(tsk_args->cur_NF_forum == tsk_args->NF->max_size){
		
		//printf("Terminating %d\n",tsk_args->cur_NF_forum);
		tsk_args->done2= 1;	
		pthread_cond_signal(&cond);
	}
	pthread_mutex_unlock(&fg_mutex);//3ekleidwse to mutex
}

struct GraphList *Return_NForums_Graphs(struct FNodeList *Forums,int N_Forums_num){

	int i=0;
	Task_Args *task_args;
	task_args = malloc(sizeof(Task_Args));

	printf("size of forums %d\n",Forums->size);
	threadPool *th_p;
	int th_p_size = 5;
	th_p = threadPool_Creation(th_p_size);
	NForums *NF;
	NF = Create_NForums(N_Forums_num,&Forums->cur,Forums->size);
	FNode *fn;

	task_args->Forums = Forums;
	task_args->NF = NF;
	task_args->done = 0;
	task_args->cur_NF_forum = 0;
	task_args->done2 = 0;

	int task_num = Forums->size;

	for(i=0;i<task_num;i++)//ana8esh sta threads na metrhsoun ta melh olwn twn forum
		Add_Job_To_ThreadPool(th_p, (void*)Find_NForums, (void*)task_args);
	

	pthread_mutex_lock(&main_thred_mutex);
	
	while(task_args->done!=1){//To main thread perimenei mexri na teleiwsei h diadikasia eureshs twn Nforums
		pthread_cond_wait(&cond,&main_thred_mutex);
	}	
	pthread_mutex_unlock(&main_thred_mutex);

	

	GraphList *GL;
	GL = Create_Graph_List();//Dhmiourgia mias listas grafwn
	task_args->GL = GL;
	task_args->cur_forum = NF->head;

	for(i=0;i<NF->max_size;i++)//ana8esh sta threads na dhmiourghsoun grafous gia ta top forums	
		Add_Job_To_ThreadPool(th_p, (void*)Create_F_Graphs, (void*)task_args);	

	pthread_mutex_lock(&main_thred_mutex);
	while(task_args->done2!=1){
		pthread_cond_wait(&cond,&main_thred_mutex);
	}	
	pthread_mutex_unlock(&main_thred_mutex);

	Destroy_ThreadPool(th_p);//katastrofh threadpool

	free(task_args);

	return GL;
}

