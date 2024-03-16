#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void child_process() {
    printf("Child process PID: %d\n", getpid());
    printf("Child process running...\n");
    for (int i = 0; i < 5; i++) {
        printf("Child process running: %d\n", i);
        sleep(1);
    }
}

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        // Child process
        child_process();
        exit(0);
    } else if (pid > 0) {
        // Parent process
        sleep(5); // Sleep for 5 seconds

        // Send SIGTSTP to suspend the child process
        kill(pid, SIGTSTP);
        printf("Child process suspended for 10 seconds...\n");
        sleep(10); // Sleep for 10 seconds

        // Send SIGCONT to resume the child process
        kill(pid, SIGCONT);

        // Wait for the child process to terminate
        waitpid(pid, NULL, 0);
        printf("Child process terminated\n");
    } else {
        // Error handling for fork failure
        perror("fork");
        exit(EXIT_FAILURE);
    }

    return 0;
}
