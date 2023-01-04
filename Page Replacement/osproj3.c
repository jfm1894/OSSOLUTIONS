//********************************************************************
//
// Jeffrey Mitchell
// Operating Systems
// Programming Project #4: Producer/Consumer Simulation
// September 21, 2022
// Instructor: Dr. Siming Liu
//
//********************************************************************
//includes
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "buffer.h"
#include <pthread.h>
#include <unistd.h>

//forward function declarations
void printBuffer();
void *consumer(void *param);
void *producer(void *param);
int isPrime(int num);
void printBuffPos();
void displayStats();
//input variables
int timeToSleepMain;
int timeToSleepThread;
int numProducer ;
int numConsumer;
int outputFlag;



//********************************************************************
//
// Main Function
//
// This function is the main body of our program that collects input
// from command line, intializes the buffer, creates threads, sleeps,
// joins threads and then displays statistics
//
// Return Value
// ------------
// int 0                       
//
// Value Parameters
// ----------------
// int argc - number of arguments from command line
// char* argv[] array of user input
//
// Reference Parameters
// --------------------
// none
//
//*******************************************************************
int main(int argc, char* argv[])
{
    //Get command line arguments
    timeToSleepMain = atoi(argv[1]);
    timeToSleepThread = atoi(argv[2]);
    numProducer = atoi(argv[3]);
    numConsumer = atoi(argv[4]);
    outputFlag=0;

    pthread_t producerIDS[numProducer];
    pthread_t consumerIDS[numConsumer];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    if (strcmp(argv[5],"yes")==0){
    outputFlag = 1;
    }
    //Initialize buffer
    buffer_initialize();
    //Create producer thread(s)
    printf("Starting Threads...\n");
   for (int i=0; i<numProducer;i++)
   {
        producerIDS[i]=pthread_create(&producerIDS[i],&attr,producer,NULL);
   }
    //Create consumer thread(s)
   for (int i=0; i<numConsumer;i++)
   {
        consumerIDS[i]=pthread_create(&consumerIDS[i],&attr,consumer,NULL);
   }
    //Sleep
   sleep(timeToSleepMain);
    //Join Threads
   for (int i=0;i<numProducer;i++)
   {
    pthread_join(producerIDS[i],NULL);
   }
   for(int i=0;i<numConsumer;i++)
   {
    pthread_join(consumerIDS[i],NULL);
   }
    //Display Statistics
   displayStats();
    //Exit
    return 0;
}

//********************************************************************
//
// Statistics Display Function
//
// This function prints out the stats collected during the simulation
// to the terminal.
//
// Return Value
// ------------
// none                        
//
// Value Parameters
// ----------------
// none
//
// Reference Parameters
// --------------------
// none
//
//*******************************************************************
void displayStats()
{
    printf("PRODUCER / CONSUMER SIMULATION COMPLETE\n");
    printf("========================================\n");
    printf("Simulation Time:\t\t\t%d\n",timeToSleepMain);
    printf("Maximum Thread Sleep time:\t\t%d\n",timeToSleepThread);
    printf("Number of Producer Threads:\t\t%d\n",numProducer);
    printf("Number of Consumer Threads:\t\t%d\n",numConsumer);
    printf("Size of Buffer:\t\t\t\t%d\n",BUFFER_SIZE);
    printf("\n");
    printf("Total Number of Items Produced: %d\n", itemsProduced);
    printf("Total Number of items Consumed: %d\n", itemsConsumed);
    printf("Number Of Items Remaining in Buffer:%d\n", itemsProduced-itemsConsumed);
    printf("Number Of Times Buffer Was Full: %d\n", bufferFullCounter);
    printf("Number of Times Buffer Was Empty: %d\n", bufferEmptyCounter);
}

//********************************************************************
//
// Producer Function
//
// This function is called in the producer threads to add items to the buffer
// 
//
// Return Value
// ------------
// void                     
// 
//
// Value Parameters
// ----------------
// none
//
// Reference Parameters
// --------------------
// void *param
//
//*******************************************************************

void *producer(void *param)
  {
    do{
    srand(time(NULL));
    sleep(timeToSleepThread);
    buffer_item randNum;
    randNum=rand()%100+1;
    sem_wait(&empty);
    sem_wait(&mutex);
    pthread_mutex_lock(&pmutex);
    buffer_insert_item(randNum);
    if (outputFlag == 1)
    {
    printf("Producer %lu produces %d\n",pthread_self(),randNum);
    printBuffer();
    }
    pthread_mutex_unlock(&pmutex);
    sem_post(&mutex);
    sem_post(&full);
    }while(true);
    pthread_exit(0);
  }

//********************************************************************
//
// Consumer Function
//
// This function reads items from the buffer to determine if they are prime
// then replaces them with -1
//
// Return Value
// ------------
//                      
// void
//
// Value Parameters
// ----------------
// none
//
// Reference Parameters
// --------------------
// void *param
//
//*******************************************************************

  void *consumer(void *param)
  {
    buffer_item itemToRemove;
    do
    {
      sleep(timeToSleepThread);
      sem_wait(&full);
      sem_wait(&mutex);
      pthread_mutex_lock(&pmutex);
      buffer_remove_item(&itemToRemove);
      if(outputFlag == 1)
      {
      printf("Consumer %lu consumes %d\n",pthread_self(),itemToRemove);
      if (isPrime(itemToRemove) == 1)
      {
        printf("*****%d is prime*******\n",itemToRemove);
      }
      printBuffer();
      }
      pthread_mutex_unlock(&pmutex);
      sem_post(&mutex);
      sem_post(&empty);
    } while (true);
      pthread_exit(0);
  }
//********************************************************************
//
// Prime Detector Function
//
// This function checks to see whether a number is prime or not
// 
//
// Return Value
// ------------
// 1 if the number is prime                       
// 0 if the number is not prime
//
// Value Parameters
// ----------------
// int num - number to be checked for prime status
//
// Reference Parameters
// --------------------
// none
//
//*******************************************************************
  int isPrime(int num)
  {
    if(num == 1)
    {
      return 0;
    }
    int rval =1;
    for (int i=2; i<num;i++)
    {
        if (num%i == 0)
        {
            rval= 0;
        }
    }
    return rval;
  }
//********************************************************************
//
// Buffer printing function
//
// This function prints the buffer to the terminal and also displays
// which indexes are currently being accessed
//
// Return Value
// ------------
// void                    
// 
//
// Value Parameters
// ----------------
// none
//
// Reference Parameters
// --------------------
// none
//
//*******************************************************************
void printBuffer()
{
  printf("(buffers occupied: %d)\n",bufferItems);
  printf("Buffers: ");
  for(int i=0;i<BUFFER_SIZE;i++)
  {
    printf("%d  ", buffer[i]);
  }
  printf("\n");
  printf("\t --- --- --- --- ---\n");
  printBuffPos();
  printf("\n\n");
}
//********************************************************************
//
// Index Printing Function
//
// This function checks all cases of buffer positions and prints
// appropriate output
//
// Return Value
// ------------
// void
//
// Value Parameters
// ----------------
// none
//
// Reference Parameters
// --------------------
// none
//
//*******************************************************************
void printBuffPos()
{
  printf("\t ");
  if (buffIndexInsert == 0 && buffIndexRemove == 0)
  {
    printf("WR");
  }
    if (buffIndexInsert == 0 && buffIndexRemove == 1)
  {
    printf("W   R");
  }
    if (buffIndexInsert == 0 && buffIndexRemove == 2)
  {
    printf("W      R");
  }
    if (buffIndexInsert == 0 && buffIndexRemove == 3)
  {
    printf("W         R");
  }
    if (buffIndexInsert == 0 && buffIndexRemove == 4)
  {
    printf("W            R");
  }
    if (buffIndexInsert == 1 && buffIndexRemove == 0)
  {
    printf("R   W");
  }
    if (buffIndexInsert == 1 && buffIndexRemove == 1)
  {
    printf("   WR");
  }
    if (buffIndexInsert == 1 && buffIndexRemove == 2)
  {
    printf("   W   R");
  }
    if (buffIndexInsert == 1 && buffIndexRemove == 3)
  {
    printf("   W      R");
  }
    if (buffIndexInsert == 1 && buffIndexRemove == 4)
  {
    printf("   W         R");
  }
    if (buffIndexInsert == 2 && buffIndexRemove == 0)
  {
    printf("R      W");
  }
    if (buffIndexInsert == 2 && buffIndexRemove == 1)
  {
    printf("   R      W");
  }
    if (buffIndexInsert == 2 && buffIndexRemove == 2)
  {
    printf("      WR");
  }
    if (buffIndexInsert == 2 && buffIndexRemove == 3)
  {
    printf("      W   R");
  }
    if (buffIndexInsert == 2 && buffIndexRemove == 4)
  {
    printf("      W      R");
  }
    if (buffIndexInsert == 3 && buffIndexRemove == 0)
  {
    printf("R         W");
  }
    if (buffIndexInsert == 3 && buffIndexRemove == 1)
  {
    printf("   R      W");
  }
    if (buffIndexInsert == 3 && buffIndexRemove == 2)
  {
    printf("      R   W");
  }
    if (buffIndexInsert == 3 && buffIndexRemove == 3)
  {
    printf("         WR");
  }
    if (buffIndexInsert == 3 && buffIndexRemove == 4)
  {
    printf("         W   R");
  }
    if (buffIndexInsert == 4 && buffIndexRemove == 0)
  {
    printf("R         W");
  }
    if (buffIndexInsert == 4 && buffIndexRemove == 1)
  {
    printf("   R         W");
  }
    if (buffIndexInsert == 4 && buffIndexRemove == 2)
  {
    printf("      R      W");
  }
    if (buffIndexInsert == 4 && buffIndexRemove == 3)
  {
    printf("         R   W");
  }
    if (buffIndexInsert == 4 && buffIndexRemove == 4)
  {
    printf("            WR");
  }
}
