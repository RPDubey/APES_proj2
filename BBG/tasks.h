#ifndef TASKS_H
#define TASKS_H





/**
*@brief:Implements log Task
*Requests and receives logs from other tasks and writes them synchronously on file
*@param:pointer to thread info structure
*@return: returns NULL pointer
*/
void *logTask(void *thread_arg);

/**
*@brief:Implements com Task
*Creates a new task to handle connection request from a new client
*@param:pointer to thread info structure
*@return: returns NULL pointer
*/
void *comTask(void *thread_arg);

void* taskThread(void* thread_arg);

#endif
