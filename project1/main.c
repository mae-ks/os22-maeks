#include <iostream.h>
#define DO_LS 1
#define DO_PS 2
#define DO_EXIT 3

int get_response() {
    int response = 0;
    do {
        printf("-------------");
        printf("1. Directory Listing (ls)\n");
        printf("2. Process Status (ps)\n");
        printf("3. Exit Shell\n");
        printf("-------------");
        printf("Enter option (1-3):\n");
        take input
    } while (input != DO_LS && response != DO_PS && response != DO_EXIT);
    return response;
}

int main() {
    int choice, status;
    pid_t pid, child_pid;
    choice = get_response();
    while (choice != DO_EXIT) {
        pid = fork();
        switch(pid) {
            case 0:
            pid = getpid();
            printf("In child (pid = %d)\n", pid);
            pid = getppid();
            printf("With parent (pid = %d)\n", pid);
            switch (choice) {
                case DO_LS: execl("/bin/ls", "ls", "-C", 0);
                break;
                case DO_PS: execl("/bin/ps", "ps", "-l", 0);
                break;
                default: printf("This should never occur\n");
            }
            exit(1);

            case -1:
            printf("Unable to fork child process.\n");
            exit(1);

            default:
            child_pid = wait(&status);
            printf("Process (pid = %d) exited with status %d\n", child_pid, status);
            break;
        }
        choice = get_response();
    }
}