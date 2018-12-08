#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <math.h>
#include "connector.h"
#define N 2

sem_t* semHost;
sem_t* semClientArr[N];
//const char semNames[N][50] = {"/semClient1", "/semClient2", "/semClient3", "/semClient4", "/semClient5", "/semClient6", "/semClient7"};
const char semNames[N][50] = {"/semClient1", "/semClient2"};
unsigned int wolfSeed = 1;
unsigned int goatsSeeds[2] = {2, 3};



void clean(){
	int i = 0;
	if (semHost != NULL && semHost != SEM_FAILED){
		sem_close(semHost);
		sem_unlink("/semHost");
	}
	for(;i<N;i++)
		if(semClientArr[i] != NULL && semClientArr[i] != SEM_FAILED){
			sem_close(semClientArr[i]);
			sem_unlink(semNames[i]);
		}
	if(conn_isCreated())
		conn_destroy();

}

int calcAliveState(int wolfNumber, int goatNumber, int aliveStatus){
	if(aliveStatus == 1){
		if(abs(wolfNumber-goatNumber) <= (float)70 / N)
			return 1;
		else 
			return 0;
	}
	else{
		if(abs(wolfNumber-goatNumber) <= floor((float)20 / N))
			return 1;
		else
			return 0;
	}

}

int checkGoatsStates(int *aliveStates, int size){
	int i=0;
	for(;i<size;i++)
		if(aliveStates[i] == 1)
			return 1;
	return 0;
}
void host_function(){
	int i=0;
	int wolfNumber;
	int goatsNumbers[N];
	//Initial game data
	int aliveStates[N];
	
	for(i=0; i < N; i++)
		aliveStates[i] = 1;
	int deadTurnsNumber = 0;


	printf("Wolf started\n");

    while(1)
    {


    wolfNumber = rand_r(&wolfSeed)%100 + 1;
    printf("Wolf generated number %i\n", wolfNumber);

	for(i=0; i < N; i++){
		printf("Wolf sent alive state to goat%i\n", i);
		
		if (conn_write(&aliveStates[i], sizeof(int)) == 0)
			printf("Connector problem\n");
		sem_post(semClientArr[i]);
		sem_wait(semHost);
		if(conn_read(&goatsNumbers[i], sizeof(int)) == 0)
			printf("Connector problem\n");
		printf("Wolf got number %i from goat%i\n", goatsNumbers[i], i);
		aliveStates[i] = calcAliveState(wolfNumber, goatsNumbers[i], aliveStates[i]);
	}

	if(checkGoatsStates(aliveStates, N))
		deadTurnsNumber = 0;
	else
		deadTurnsNumber++;
	printf("Round finished\n");
	if(deadTurnsNumber == 1)
		break;
}
    for(i=0; i < N; i++)
    {
    	printf("Wolf sent end signal to goat%i\n", i);
    	aliveStates[i] = -1;
		
		if (conn_write(&aliveStates[i], sizeof(int)) == 0)
			printf("Connector problem\n");
		sem_post(semClientArr[i]);
		sem_wait(semHost);
    }
    printf("Wolf finished\n");


	clean();
	return;
}

void client_function(int i){
	int aliveState;
	int goatNumber;
	sem_wait(semClientArr[i]);
	while(1){
	
	if (conn_read(&aliveState, sizeof(int)) == 0)
			printf("Connector problem\n");
	printf("Goat %i get data %i\n", i, aliveState);
	//end signal
	if(aliveState == -1){
		sem_post(semHost);
		break;
	}

	if(aliveState == 1)
	{
		goatNumber = rand_r(&goatsSeeds[i])%100 + 1;
	}
	else if(aliveState == 0){
		goatNumber = rand_r(&goatsSeeds[i])%50 + 1;
	}


	printf("Goat %i generate data %i\n", i, goatNumber);
	if (conn_write(&goatNumber, sizeof(int)) == 0)
			printf("Connector problem\n");
	sem_post(semHost);
	sem_wait(semClientArr[i]);
}
    printf("Goat %i end\n", i);
	return;
}
int main(void)
{
	int i=0;
	srand(23);

	printf("Start of game\n");
	conn_create();
	semHost = sem_open("/semHost", O_CREAT, S_IRWXU, 0);
	if(semHost == SEM_FAILED)
  {
    printf("Something went wrong with semaphore parent...\n");
  }
 
  for(; i<N; i++){

  	semClientArr[i] = sem_open(semNames[i], O_CREAT, S_IRWXU, 0);
	if(semClientArr[i] == SEM_FAILED)
  {
    printf("Something went wrong with semaphore son%i...\n", i);
  }
  }
   
  
  printf("Here!!!\n");
  
  for(i = 0; i < N; i++)
{
  if(fork())
  {
    continue;
  }
  else
  {
    client_function(i);
    return 0;
  }
}
host_function();
return 0;
 
}