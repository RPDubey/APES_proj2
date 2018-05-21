/*******************************************************************************
   @Filename:main.c
   @Brief: main for project2
   @Author:Ravi Dubey
   @Date:4/27/2018
 ******************************************************************************/
#include <errno.h>
#include <mqueue.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "includes.h"
#include "tasks.h"

uint8_t gclose_app;

void SIGINT_handler(int sig) {
  if (sig == SIGINT) {
    gclose_app = 0;
    printf("\ncleared gclose_app flag\n");
  }
}

int main(int argc, char *argv[]) {

  gclose_app = 1;
  signal(SIGINT, SIGINT_handler);

  printf("Entering Main- PID:%d\n", getpid());
  if (argc > 1) {
    filename = (char *)malloc(sizeof(argv[1]));
    strcpy(filename, argv[1]);
  } else {
    filename = (char *)malloc(sizeof(DEFAULT_FILE_NAME));
    strcpy(filename, DEFAULT_FILE_NAME);
  }
  printf("Logfile name set to %s\n", filename);
  int ret;

  /****Creating Thread to accept new client connections******/
  pthread_t com_thread;
  ret = pthread_create(&com_thread, DEFAULT_THREAD_ATTR, comTask, NULL);
  if (ret != 0) {
    printf("Main pthread_create:%s\n", strerror(errno));
    return -1;
  }

  pthread_join(com_thread, NULL);
  printf("***************Cleaning up***************\n");

  // pthread_join(log_thread, NULL);

  printf("***************Exiting***************\n");
  return 0;
}
