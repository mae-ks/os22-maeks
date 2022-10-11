/* hello_signal.c */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>

bool signaled = true;

void handler(int signum)
{ //signal handler
  signaled = false;
}

int main(int argc, char * argv[])
{
  signal(SIGALRM, handler); //register handler to handle SIGALRM
  alarm(1); //Schedule a SIGALRM for 1 second

  while(signaled) {}; //busy wait for signal to be delivered
  printf("Turing was right!\n");
  
  return 0; //never reached
}