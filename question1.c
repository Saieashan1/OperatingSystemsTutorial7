#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX_NAME_LENGTH 256
#define MAX_LINE_LENGTH 512
//creates the proc stucture
typedef struct proc {
    char name[MAX_NAME_LENGTH];
    int priority;
    int pid;
    int runtime;
} proc;
//creates linked list structure
typedef struct node {
    proc process;
    struct node* next;
} node;
//makes queue for linked list
typedef struct {
    node* front;
    node* rear;
} Queue;

void initializeQueue(Queue* q) {
    q->front = NULL;
    q->rear = NULL;
}
//function that adds process to queue
void push(Queue* q, proc process) {
    node* newnode = (node*)malloc(sizeof(node));
    if (newnode == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    newnode->process = process;
    newnode->next = NULL;
    if (q->rear == NULL) {
        q->front = newnode;
        q->rear = newnode;
    } else {
        q->rear->next = newnode;
        q->rear = newnode;
    }
}
//prints out the contents of the queue
void printQueue(Queue q) {
    node* current = q.front;
    while (current != NULL) {
        printf("Name: %s, Priority: %d, PID: %d, Runtime: %d\n",
               current->process.name, current->process.priority,
               current->process.pid, current->process.runtime);
        current = current->next;
    }
}

int main(void) {
//opens the file processes.txt and reads from it
    FILE* file = fopen("processes.txt", "r");
    //checks to see if the .txt opens up properly or not
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }
//creates a empty queue
    Queue queue;
    initializeQueue(&queue);
//reads each line from the .txt file and adds it the process in the queue
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        //creates new process to store the data from the .txt file
        proc process;
        sscanf(line, "%[^,],%d,%d,%d", process.name, &process.priority,
               &process.pid, &process.runtime);
        push(&queue, process);
    }
    //closes the file
    fclose(file);
//prints out the contents of the queue
    printf("Processes:\n");
    printQueue(queue);
    
    //exit program
    return 0;
}
