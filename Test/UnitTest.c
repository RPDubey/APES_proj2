#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>

#include "sockets.h"

#define BUFFER_SIZE 1024

struct sockaddr_in serverAddress; 
int num_char;
int connectionFileDescriptor = 0;
int receiveFileDescriptor = 0;

char sendBuff[1025];

void testCreateSocket(void **state) {
  receiveFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
  assert_int_not_equal(-1,receiveFileDescriptor);
}

void testCompareSocket(void **state) {
  assert_int_not_equal(0,strcmp("TEST COMPARE SOCKET",sendBuff));
}

void testAcceptSocket(void **state) {
  receiveFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
  memset(&serverAddress, '0', sizeof(serverAddress));
  memset(sendBuff, '0', sizeof(sendBuff)); 

  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
  serverAddress.sin_port = htons(5000); 

  bind(receiveFileDescriptor, (struct sockaddr*)&serverAddress, sizeof(serverAddress)); 
  listen(receiveFileDescriptor, 10); 
  connectionFileDescriptor = accept(receiveFileDescriptor, (struct sockaddr*)NULL, NULL); 
  assert_int_not_equal(0,connectionFileDescriptor);
}

void testReadSocket(void **state) {
  
  num_char = read(connectionFileDescriptor, sendBuff, sizeof(sendBuff));
  assert_int_not_equal(0,num_char);
}

int main(void) {
  const struct UnitTest tests[] = {
      unit_test(testCreateSocket),
      unit_test(testCompareSocket),
      unit_test(testAcceptSocket),
      unit_test(testReadSocket)
  };
  return run_tests(tests);
}
