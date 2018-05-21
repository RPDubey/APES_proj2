
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "includes.h"
#include "messageQue.h"
#include "tasks.h"
#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

extern uint8_t gclose_app;

void *taskThread(void *thread_arg) {

  printf("New Task Created\n");
  int num_char; // No. of characters red/written
  msg_struct *rx_buf = (msg_struct *)malloc(sizeof(msg_struct));
  if (rx_buf == NULL) {
    printf("malloc Error: %s\n", strerror(errno));
    return NULL;
  }

  FILE *pfd = fopen("logfile.txt", "w");
  if (pfd == NULL) {
    printf("Log Task-fopen %s\n", strerror(errno));
  } else {
    printf("Log Task-fopen %s\n", strerror(errno));
  }

  int msg_prio = MSG_PRIO;
#ifdef USE_SOCKET
  int *newsockfd = (int *)thread_arg;
#endif

#ifdef USE_UART
  /*******Initialize Notification Message Que*****************/
  mqd_t COM_msgq;
  int msg_prio_err = MSG_PRIO_ERR;
  int num_bytes_err;
  struct mq_attr msgq_attr_err = {
      .mq_maxmsg = MQ_MAXMSG,           // max # msg in queue
      .mq_msgsize = sizeof(msg_struct), // max size of msg in bytes
      .mq_flags = 0};

  COM_msgq =
      mq_open(COM_MQ,           // name
              O_CREAT | O_RDWR, // flags. create a new if dosent already exist
              S_IRWXU,          // mode-read,write and execute permission
              &msgq_attr_err);  // attribute
  if (COM_msgq < 0) {
    printf("mq_open-COM_mq %s\n", strerror(errno));
  }

#endif

  struct timespec now, expire;
  int num_bytes;
  /*********************** LOOP ****************************/
  while (gclose_app) {

    bzero(rx_buf, sizeof(msg_struct));

#ifdef USE_SOCKET
    num_char = read(*newsockfd, rx_buf, sizeof(msg_struct));
    if (num_char > 0) {
      // log it
      fprintf(pfd, "%d,%d,%d,%s:%d\n", rx_buf->dev_ID, rx_buf->task_ID,
              rx_buf->msg_type, rx_buf->message, rx_buf->msg_val);
      printf("%d,%d,%d,%s:%d\n", rx_buf->dev_ID, rx_buf->task_ID,
             rx_buf->msg_type, rx_buf->message, rx_buf->msg_val);

      fflush(pfd);
    }
#endif

#ifdef USE_UART
    do {
      clock_gettime(CLOCK_MONOTONIC, &now);
      expire.tv_sec = now.tv_sec + 5;
      expire.tv_nsec = now.tv_nsec;
      num_bytes = mq_timedreceive(COM_msgq, (char *)rx_buf, sizeof(msg_struct),
                                  &msg_prio, &expire);

      // log the data
      if (num_bytes > 0) {
        printf("%d,%d,%d,%s:%d\n", rx_buf->dev_ID, rx_buf->task_ID,
               rx_buf->msg_type, rx_buf->message, rx_buf->msg_val);

        fprintf(pfd, "%d,%d,%d,%s:%d\n", rx_buf->dev_ID, rx_buf->task_ID,
                rx_buf->msg_type, rx_buf->message, rx_buf->msg_val);

        fflush(pfd);
      }
    } while (num_bytes > 0);
#endif

    sleep(1);
  }

#ifdef USE_SOCKET
  close(*newsockfd);
#endif
  fclose(pfd);
  free(rx_buf);
#ifdef USE_UART
  mq_close(COM_msgq);
#endif
  return NULL;
}
