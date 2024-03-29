#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

// Function to handle the child process
void child_process() {
    printf("Child process PID: %d\n", getpid());
    printf("Child process is running...\n");
    while (1) {
        // Generate some random information
        int info = rand() % 100 + 1;
        printf("\033[1;32mInfo from child process: %d\033[0m\n", info); // Print in green color
        sleep(1);
    }
}

// Function to handle the parent process
void parent_process(pid_t child_pid) {
    printf("Parent process PID: %d\n", getpid());
    printf("Child process PID: %d\n", child_pid);
    printf("Parent process is waiting for 5 seconds...\n");
    sleep(5);
    printf("Sending SIGINT to child process...\n");
    kill(child_pid, SIGINT);
}

int main() {
    srand(time(NULL)); // Seed for random number generation

    // Fork the process
    pid_t pid = fork();

    if (pid == -1) { // Fork failed
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { // Child process
        child_process();
    } else { // Parent process
        parent_process(pid);
    }

    return 0;
}
