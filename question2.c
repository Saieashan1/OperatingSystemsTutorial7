#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<string.h>

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

//removes next item from queue and returns the process
proc pop(Queue* q){
if(q->front==NULL){
printf("queue is empty\n");
proc empty_proc={"",0,0,0};
return empty_proc;
}
node* temp=q->front;
proc popped_process=temp->process;
q->front =q->front->next;
if(q->front==NULL){
q->rear=NULL;}
free(temp);
return popped_process;
}
// deletes a process from the queue given the name and returns process
proc delete_name(Queue* q,char* name){
node*current=q->front;
node* prev=NULL;
while(current != NULL){
 if (strcmp(current->process.name, name) == 0){
proc deleted_process=current->process;
if(prev==NULL){
q->front=current->next;
}else{
prev->next=current->next;
}
if(q->rear==current){
q->rear=prev;
}
free(current);
return deleted_process;
}
prev=current;
current=current->next;
}
proc empty_proc={"",0,0,0};

}
//this function deletes a process from PID from the returns the process
proc delete_pid(Queue* q, int pid) {
    node* current = q->front;
    node* prev = NULL;
    while (current != NULL) {
        if (current->process.pid == pid) {
            proc deleted_process = current->process;
            if (prev == NULL) {
                q->front = current->next;
            } else {
                prev->next = current->next;
            }
            if (q->rear == current) {
                q->rear = prev;
            }
            free(current);
            return deleted_process;
        }
        prev = current;
        current = current->next;
    }
    proc empty_proc = {"", 0, 0, 0};
    return empty_proc;
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
        
    }
    //closes the file
    fclose(file);
    //deletes the process "emacs"
     proc deleted_proc_name = delete_name(&queue, "emacs");
    printf("Deleted process named 'emacs': %s\n", deleted_proc_name.name);

    // Deletes process PID 12235
    proc deleted_proc_pid = delete_pid(&queue, 12235);
    printf("Deleted process with PID 12235: %s\n", deleted_proc_pid.name);
printf("remaing process:\n");
//iterate through the remaining processes in the queue and prints it out
while(true){
proc process=pop(&queue);
if(strcmp(process.name,"")==0){
}
 printf("Name: %s, Priority: %d, PID: %d, Runtime: %d\n",
process.name, process.priority, process.pid, process.runtime);
}
return 0;}
