//
// Created by student on 6/13/22.
//

#ifndef HW3_QUEUE_H
#define HW3_QUEUE_H

#include <stdbool.h>
#include <string.h>
#include <malloc.h>

typedef char* str;
typedef struct QueueStruct * Queue;

typedef struct {
    int connfd;
    struct timeval* request_arrival;
} * QueueElement;

QueueElement createQueueElement(int connfd);
typedef enum {
    QUEUE_SUCCESS,
    QUEUE_FULL,
    QUEUE_EMPTY,
    QUEUE_BAD_ARG
} QueueResult;

Queue queueCreate(int max_size);


void destroyQueueElement(QueueElement* element);
void queueDestroy(Queue* queue_ptr);

QueueResult queueInsert(Queue queue, QueueElement input);
QueueResult queueRemove(Queue queue, QueueElement* output); // free on user ///
int queueSize(Queue queue);
bool queueIsEmpty(Queue queue);
bool queueIsFull(Queue queue);


void queuePrint(Queue queue);

#endif //HW3_QUEUE_H
