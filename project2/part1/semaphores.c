#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>

#include <semaphore.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>

void ChildProcess(int []);
void ParentProcess(int []);

int  main(int  argc, char *argv[])
{
  int fd, j=0, zero=0, *counter_ptr;
  sem_t *mutex;

  //open a file and map it into memory this is to hold the shared counter
  fd = open("log.txt",O_RDWR|O_CREAT,S_IRWXU);
  write(fd,&zero,sizeof(int));
  counter_ptr = mmap(NULL,sizeof(int),PROT_READ |PROT_WRITE,MAP_SHARED,fd,0);
  close(fd);

  /* create, initialize semaphore */
 if ((mutex = sem_open("examplesemaphore", O_CREAT, 0644, 1)) == SEM_FAILED) {
    perror("semaphore initilization");
    exit(1);
  }

  int ShmID = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
  int *ShmPTR = (int *) shmat(ShmID, NULL, 0);
  ShmPTR[0] = 0; // BankAccount

  pid_t pid = fork();

  if (pid == 0) {
    while (1) {
      sem_wait(mutex);

      srand(time(NULL));
      *counter_ptr = rand()%3 + 1;
      for (j = 0; j < *counter_ptr; j++) {
        ChildProcess(ShmPTR);
      }
      // printf("\n");
      sleep(2);

      sem_post(mutex);

      sleep(1);
    }
    exit(0);
  }
  while (1) {
    sem_wait(mutex);

    srand(time(NULL));
    *counter_ptr = rand()%3 + 1;
    for (j = 0; j < *counter_ptr; j++) {
      ParentProcess(ShmPTR);
    }
    // printf("\n");
    sleep(2);

    sem_post(mutex);

    sleep(1);
  }
  exit(0);
}

void ParentProcess(int SharedMem[]) {
  sleep(rand()%2);
  printf("Dear Old Dad: Attempting to Check Balance\n");
  int localBalance = SharedMem[0];
  int random_num = rand()%4 + 1;

  if (random_num % 2 == 0) {
    if (localBalance < 100) {
      int amount = rand()%101;
      if (amount % 2 == 0) {
        localBalance += amount;
        printf("Dear old Dad: Deposits $%d / Balance = $%d\n", amount, localBalance);
      }
      else {
        printf("Dear old Dad: Doesn't have any money to give\n");
      }
    }
  }
  else {
    printf("Dear Old Dad: Last Checking Balance = $%d\n", localBalance);
  }
  SharedMem[0] = localBalance;
}

void ChildProcess(int SharedMem[]) {
  sleep(rand()%2);
  printf("Poor Student: Attempting to Check Balance\n");
  
  int localBalance = SharedMem[0];
  int random_num = rand()%4 + 1;

  if (random_num % 2 == 0) {
    int need = rand()%51;
    printf("Poor Student needs $%d\n", need);
    if (need <= localBalance) {
      localBalance -= need;
      printf("Poor Student: Withdraws $%d / Balance = $%d\n", need, localBalance);
    }
    else {
      printf("Poor Student: Not Enough Cash ($%d)\n", localBalance);
    }
  }
  SharedMem[0] = localBalance;
}