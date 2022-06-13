//
// Created by student on 6/13/22.
//

#ifndef HW3_QUEUE_H
#define HW3_QUEUE_H

#include <stdbool.h>
#include <string.h>

typedef char* str;
typedef struct Queue * Queue;

typedef struct {
    int connfd;
} * QueueElement;

QueueElement createQueueElement(int connfd){
    QueueElement* element = malloc(sizeof(QueueElement));
    if(element == NULL)
        return NULL;
    element->connfd = connfd;
    return element;
}

void destroyQueueElement(QueueElement* element){
    free(*element);
    *element = NULL;
}

typedef enum {
    QUEUE_SUCCESS,
    QUEUE_ALLOC_ERROR,
    QUEUE_FULL,
    QUEUE_EMPTY,
    QUEUE_BAD_ARG,
    QUEUE_RESULT_SIZE
} QueueResult;

Queue queueCreate(int max_size);
void queueDestroy(Queue* queue_ptr);

QueueResult queueInsert(Queue queue, QueueElement* input);
QueueResult queueRemove(Queue queue, QueueElement* output); // free on user ///
int queueSize(Queue queue);
bool queueIsEmpty(Queue queue);
bool queueIsFull(Queue queue);




#endif //HW3_QUEUE_H
