#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

// Define structure for process
typedef struct Process {
    char name[256];
    int priority;
    pid_t pid;
    int runtime;
    struct Process* next;
} Process;

// Define structure for queue
typedef struct Queue {
    Process* front;
    Process* rear;
} Queue;

// Function to create a new node
Process* createProcessNode(char name[], int priority, int runtime) {
    Process* newProcess = (Process*)malloc(sizeof(Process));
    strcpy(newProcess->name, name);
    newProcess->priority = priority;
    newProcess->pid = 0; // Initialize pid to 0
    newProcess->runtime = runtime;
    newProcess->next = NULL;
    return newProcess;
}

// Function to enqueue a process
void enqueue(Queue* queue, char name[], int priority, int runtime) {
    Process* newProcess = createProcessNode(name, priority, runtime);
    if (queue->rear == NULL) {
        queue->front = queue->rear = newProcess;
        return;
    }
    queue->rear->next = newProcess;
    queue->rear = newProcess;
}

// Function to dequeue a process
Process* dequeue(Queue* queue) {
    if (queue->front == NULL)
    return NULL;
    Process* temp = queue->front;
    queue->front = queue->front->next;
    if (queue->front == NULL)
    queue->rear = NULL;
    return temp;
}

int main() {
    // Initialize queue
    Queue queue = { NULL, NULL };

    // Read processes from file and add to queue
    FILE* file = fopen("processes_q5.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    char name[256];
    int priority, runtime;
    while (fscanf(file, "%s %d %d", name, &priority, &runtime) != EOF) {
        enqueue(&queue, name, priority, runtime);
    }
    fclose(file);

    // Execute processes with priority 0 first
    Process* current = queue.front;
    while (current != NULL) {
        if (current->priority == 0) {
            pid_t pid = fork();
            if (pid == 0) { // Child process
                // Execute process binary
                // Set pid member in process struct to the process id returned from exec
                execl(current->name, current->name, NULL);
                perror("Error executing process");
                exit(1);
            } else if (pid > 0) { // Parent process
                current->pid = pid;
                // Run the process for the specified runtime
                sleep(current->runtime);
                // Send SIGINT signal to terminate process
                kill(pid, SIGINT);
                // Wait until the process has terminated
                waitpid(pid, NULL, 0);
                // Print process details
                printf("Name: %s, Priority: %d, PID: %d, Runtime: %d\n", current->name, current->priority, current->pid, current->runtime);
            } else {
                perror("Fork failed");
                exit(1);
            }
        }
        current = current->next;
    }

    // Execute remaining processes
    current = queue.front;
    while (current != NULL) {
        pid_t pid = fork();
        if (pid == 0) { // Child process
            // Execute process binary
            // Set pid member in process struct to the process id returned from exec
            execl(current->name, current->name, NULL);
            perror("Error executing process");
            exit(1);
        } else if (pid > 0) { // Parent process
            current->pid = pid;
            // Run the process for the specified runtime
            sleep(current->runtime);
            // Send SIGINT signal to terminate process
            kill(pid, SIGINT);
            // Wait until the process has terminated
            waitpid(pid, NULL, 0);
            // Print process details
            printf("Name: %s, Priority: %d, PID: %d, Runtime: %d\n", current->name, current->priority, current->pid, current->runtime);
        } else {
            perror("Fork failed");
            exit(1);
        }
        current = current->next;
    }

    // Free allocated memory
    while (queue.front != NULL) {
        Process* temp = dequeue(&queue);
        free(temp);
    }

    return 0;
}
