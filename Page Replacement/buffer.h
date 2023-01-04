//********************************************************************
//
// Jeffrey Mitchell
// Operating Systems
// Programming Project #4: Producer/Consumer Simulation
// September 21, 2022
// Instructor: Dr. Siming Liu
//
//********************************************************************

#ifndef _BUFFER_H_DEFINED_
#define _BUFFER_H_DEFINED_

//includes
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
//declare global variables and semaphore/mutex
typedef int buffer_item;
int totalProduced;
int totalConsumed;
sem_t mutex;
sem_t full;
sem_t empty;
pthread_mutex_t pmutex;
#define BUFFER_SIZE 5
buffer_item buffer[BUFFER_SIZE];
int buffIndex=0;
int buffIndexRemove=0;
int buffIndexInsert=0;
int bufferItems=0;
int bufferFullCounter=0;
int bufferEmptyCounter=0;
int itemsProduced=0;
int itemsConsumed=0;
int bufferFull=0;

//********************************************************************
//
// Buffer Insert Item Function
//
// This function takes a buffer_item(int) and inserts it into the buffer
// it also manages the current index for the producer threads moving it
// in a circular fashion and increments sim stats where necessary
//
// Return Value
// ------------
// bool true                      
//
// Value Parameters
// ----------------
// buffer_item item - randomly generated integer between 1 and 100
// 
//
// Reference Parameters
// --------------------
// none
//
//*******************************************************************
bool buffer_insert_item( buffer_item item )
{
    buffer[buffIndexInsert]=item;
    bufferItems = bufferItems + 1;
    buffIndexInsert = buffIndexInsert + 1;
    itemsProduced = itemsProduced + 1;
    if(buffIndexInsert ==  5)
    {
        bufferFullCounter = bufferFullCounter + 1;
        buffIndexInsert=0;
    }
    return true;
}
//********************************************************************
//
// Buffer Remove Item Function
//
// This function reads a reference to an item in the buffer and then replaces
// that item with the initial value of -1
// 
//
// Return Value
// ------------
// bool true                       
//
// Value Parameters
// ----------------
// none
// 
//
// Reference Parameters
// --------------------
// buffer_item *item reference to item in buffer at current index
//
//*******************************************************************
bool buffer_remove_item( buffer_item *item )
{
    *item = buffer[buffIndexRemove];
    buffer[buffIndexRemove]=-1;
    buffIndexRemove = buffIndexRemove + 1;
    bufferItems = bufferItems - 1;
    itemsConsumed = itemsConsumed + 1;
    if(buffIndexRemove == 5)
    {
        buffIndexRemove = 0;
        bufferEmptyCounter = bufferEmptyCounter + 1;

    }
}
//********************************************************************
//
// Buffer Initialize Function
//
// This function initializes the buffer to -1 at all indexes and also
// intializes the semaphores and mutex's
// 
//
// Return Value
// ------------
// void                     
//
// Value Parameters
// ----------------
// none
// 
//
// Reference Parameters
// --------------------
// none
//
//*******************************************************************
void buffer_initialize()
{
    sem_init(&mutex,0,1);
    sem_init(&full,0,0);
    sem_init(&empty,0,BUFFER_SIZE);
    pthread_mutex_init(&pmutex,NULL);

    for(int i=0;i<BUFFER_SIZE;i++)
    {
        buffer[i] = -1;
    }
}


#endif // _BUFFER_H_DEFINED_
