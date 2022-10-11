/* hello_signal.c */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>

bool signaled = true;
int time, alarms;

void handler(int signum)
{ //signal handler
  printf("Hello World!\n");
  // signaled = false;
  alarms++;
  alarm(1);
}

void handler2(int signum) {
  printf("\nAlarms: %d, Duration: %d\n", alarms, time);
  exit(1);
}

int main(int argc, char * argv[])
{
  signal(SIGALRM, handler); //register handler to handle SIGALRM
  signal(SIGINT, handler2);
  alarm(1); //Schedule a SIGALRM for 1 second

  while(signaled) {
    sleep(1);
    time++;
    printf("Turing was right!\n");
  }; //busy wait for signal to be delivered
  
  return 0; //never reached
}