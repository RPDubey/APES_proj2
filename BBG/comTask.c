/*******************************************************************************
   @Filename:socketTask.c
   @Brief: implements com task functions
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

#include <fcntl.h>
#include <termios.h> // using the termios.h library
#include <unistd.h>

#include "messageQue.h"

#include "includes.h"
#include "tasks.h"

extern uint8_t gclose_app;
void *comTask(void *pthread_inf) {

  /*
Aceept connection from socket_client
create a new task to handle new client
send a new port number
*/
  pthread_t task_thread[MAX_TASKS]; // task threads
  int thread_index = -1;
  int ret;

#ifdef USE_SOCKET
  /**********************************Sockets*************************************/
  int sockfd[MAX_TASKS];          // listening FD
  int newsockfd[MAX_TASKS];       // Client connected FD
  struct sockaddr_in server_addr; // structure containing internet addresss.
  int opt = 1;
  do {
    printf("**********************************\n");
    thread_index++;
    /****Create a new socket*******/
    sockfd[thread_index] = socket(AF_INET,     // com domain - IPv4
                                  SOCK_STREAM, // com type - TCP
                                  0);          // protocol
    if (sockfd[thread_index] == -1) {
      printf("fork Error:%s\n", strerror(errno));
      return NULL;
    }
    /*****set options for the socket***********/
    ret = setsockopt(sockfd[thread_index],
                     SOL_SOCKET, // Socket Level Protocol
                     SO_REUSEADDR | SO_REUSEPORT,
                     &opt, // option is enabled
                     sizeof(opt));

    if (ret == -1) {
      printf("setsockopt Error:%s\n", strerror(errno));
      return NULL;
    }
    /***initialize the address structure ****/
    bzero((char *)&server_addr, sizeof(server_addr)); // sets all val to 0
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(COM_TASK_PORT + thread_index);

    /***bind socket to the adress and the port */
    ret = bind(sockfd[thread_index], (struct sockaddr *)&server_addr,
               sizeof(server_addr));
    if (ret == -1) {
      printf("bind Error:%s\n", strerror(errno));
      return NULL;
    }
    /**listen on socket for connections**/
    ret = listen(sockfd[thread_index], 5);
    if (ret == -1) {
      printf("listen Error:%s\n", strerror(errno));
      return NULL;
    }
    /****block until the client connects to the server and gets its
     * address*****/
    struct sockaddr_in client_addr;
    socklen_t addrlen = sizeof(client_addr); // size of address of client
    printf("Waiting for client @ Port:%d\n", (COM_TASK_PORT + thread_index));
    newsockfd[thread_index] =
        accept(sockfd[thread_index], (struct sockaddr *)&client_addr,
               (socklen_t *)&addrlen);
    if (newsockfd < 0) {
      printf("accept Error:%s\n", strerror(errno));
      return NULL;
    }

    /*****beyond this, execution happens only after client is connected******/
    printf("Connected to client\n");
    /***create new thread to handle this****/

    ret = pthread_create(&task_thread[thread_index], DEFAULT_THREAD_ATTR,
                         taskThread, (void *)&newsockfd[thread_index]);
    if (ret != 0) {
      printf("Main pthread_create:%s\n", strerror(errno));
      return NULL;
    }
    sleep(2);
  } while ((thread_index < MAX_TASKS - 1) & (gclose_app = 1));

  printf("No more Tasks can be created\n");
  // wait fpr tasks
  int i;
  for (i = 0; i < thread_index + 1; i++) {
    pthread_join(task_thread[i], NULL);
    close(sockfd[i]);
  }

#endif

#ifdef USE_UART

  // initialize
  int file, count;
  if ((file = open("/dev/ttyO1", O_RDWR)) < 0) {
    perror("UART: Failed to open the file.\n");
    return NULL;
  }
  printf("UART: File open success\n");
  struct termios options;    // The termios structure is vital
  tcgetattr(file, &options); // Sets the parameters associated with file
  if (cfsetispeed(&options, B9600) < 0) {
    printf("not success\n");
  }
  options.c_iflag = 0;
  options.c_oflag = 0;
  options.c_lflag = 0;
  options.c_cc[VMIN] = 45; // works for 1
  options.c_cc[VTIME] = 0;
  tcsetattr(file, TCSANOW, &options);
  msg_struct *rx_buf = (msg_struct *)malloc(sizeof(msg_struct));
  if (rx_buf == NULL) {
    printf("malloc Error: %s\n", strerror(errno));
    return NULL;
  }

  // create Task to handle incoming data
  thread_index = 0;
  ret = pthread_create(&task_thread[thread_index], DEFAULT_THREAD_ATTR,
                       taskThread, NULL);
  if (ret != 0) {
    printf("Main pthread_create:%s\n", strerror(errno));
    return NULL;
  }
  /*******Initialize Notification Message Que*****************/
  mqd_t COM_msgq;

  struct mq_attr msgq_attr = {
      .mq_maxmsg = MQ_MAXMSG,           // max # msg in queue
      .mq_msgsize = sizeof(msg_struct), // max size of msg in bytes
      .mq_flags = 0};

  COM_msgq =
      mq_open(COM_MQ,           // name
              O_CREAT | O_RDWR, // flags. create a new if dosent already exist
              S_IRWXU,          // mode-read,write and execute permission
              &msgq_attr);      // attribute
  if (COM_msgq < 0) {
    printf("mq_open-COM_mq %s\n", strerror(errno));
  }

  struct timespec now, expire;
  int msg_prio = MSG_PRIO;
  int num_bytes;
  /**************************LOOP************************************/
  while (gclose_app) {

    if ((count = read(file, (void *)rx_buf, sizeof(msg_struct))) < 0) {
      perror("UART read failed\n");
      // return -1;
    }
    if (count == 0)
      printf("No data available to read!\n");
    else {

      printf("read in server:%d,%d,%d,%s:%d\n", rx_buf->dev_ID, rx_buf->task_ID,
             rx_buf->msg_type, rx_buf->message, rx_buf->msg_val);

      // put data on Que
      /************Log messages on Que*************/
      clock_gettime(CLOCK_MONOTONIC, &now);
      expire.tv_sec = now.tv_sec + 5;
      expire.tv_nsec = now.tv_nsec;
      num_bytes = mq_timedsend(COM_msgq, (char *)rx_buf, sizeof(msg_struct),
                               msg_prio, &expire);
    }
  }

  pthread_join(task_thread[0], NULL);
  //  pthread_join(task_thread[1], NULL);

  free(rx_buf);
  close(file);
  mq_close(COM_msgq);
#endif

  return NULL;
}
