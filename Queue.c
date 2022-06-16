//
// Created by student on 6/13/22.
//

#include "Queue.h"
#include <stdlib.h>
//#include <sys/time.h>
#include <stdio.h>

struct QueueStruct {
    int max_size;
    QueueElement* buff_end;
    QueueElement* start_ptr;
    QueueElement* end_ptr;
    QueueElement* buff;
};

QueueElement createQueueElement(int connfd){
    QueueElement element = (QueueElement) malloc(sizeof(*element));
    if(element == NULL)
        return NULL;
    element->connfd = connfd;
    element->request_arrival = malloc(sizeof(*element->request_arrival));
    gettimeofday(element->request_arrival, NULL);
    return element;
}

void destroyQueueElement(QueueElement* element){
    free((*element)->request_arrival);
    (*element)->request_arrival = NULL;
    free(*element);
    *element = NULL;
}


void inc_buff(Queue queue, QueueElement** index_ptr);

Queue queueCreate(int max_size){
    Queue queue = (Queue) malloc(sizeof (*queue));
    if(queue == NULL)
        return NULL;
    queue->max_size = max_size;
    queue->buff = (QueueElement*) calloc(max_size + 1, sizeof(QueueElement));
    if(queue->buff == NULL) {
        free(queue);
        queue = NULL;
        return NULL;
    }
    queue->start_ptr = queue->end_ptr = queue->buff;
    queue->buff_end = queue->buff + max_size;
    return queue;
}

void queueDestroy(Queue* queue_ptr){
    if(queue_ptr == NULL)
        return;
    Queue queue = *queue_ptr;
    for (QueueElement* ptr = queue->start_ptr; ptr != queue->end_ptr; inc_buff(queue, &ptr)) {
        destroyQueueElement(ptr);
    }
    free(queue->buff);
    queue->buff = NULL;
    free(queue);
    *queue_ptr = NULL;
}

QueueResult queueInsert(Queue queue, QueueElement input){
    if(queue == NULL || input == NULL)
        return QUEUE_BAD_ARG;
    if(queueIsFull(queue)){
        return QUEUE_FULL;
    }
    *queue->end_ptr = input;
    inc_buff(queue, &(queue->end_ptr));
    return QUEUE_SUCCESS;
}

QueueResult queueRemove(Queue queue, QueueElement* output){
    if(queue == NULL || output == NULL)
        return QUEUE_BAD_ARG;
    if(queueIsEmpty(queue)) {
        *output = NULL;
        return QUEUE_EMPTY;
    }
    *output = *queue->start_ptr;
    *queue->start_ptr = NULL;
    inc_buff(queue, &queue->start_ptr);
    return QUEUE_SUCCESS;
}

int queueSize(const Queue queue){
    if(queue == NULL)
        return -1;
    int diff = queue->end_ptr - queue->start_ptr;
    if(diff < 0)
        diff += queue->max_size + 1;
    return diff;
}

bool queueIsEmpty(const Queue queue){
    if(queue == NULL)
        return false;
    return queueSize(queue) == 0;
}

bool queueIsFull(const Queue queue){
    if(queue == NULL)
        return false;
    return queueSize(queue) == queue->max_size;
}

/** * private: */
void inc_buff(Queue queue, QueueElement** index_ptr){
    (*index_ptr)++;
    if((*index_ptr) > queue->buff_end)
        *index_ptr = queue->buff;
}

void queuePrint(Queue queue){
    if(false) {
        printf("QQQQQQQQQQQQQQ\tQueue Print:\tQQQQQQQQQQQQQQ\n");
        printf("max size:\t%d\n", queue->max_size);
        printf("buff end:\tbuff + %ld\n", queue->buff_end - queue->buff);
        printf("start ptr:\tbuff + %ld\n", queue->start_ptr - queue->buff);
        printf("end ptr:\tbuff + %ld\n", queue->end_ptr - queue->buff);
    }
    printf("[");
    QueueElement* ptr;
    bool empty_cell;
    for (ptr = queue->buff; ptr <= queue->buff_end; ++ptr) {
        empty_cell = true;
        if(ptr == queue->start_ptr){ printf("s"); empty_cell = false; }
        if(ptr == queue->end_ptr){ printf("e"); empty_cell = false; }
        if(*ptr == NULL){ printf("N"); empty_cell = false; }
        if (empty_cell) printf(" ");
        printf(ptr == queue->buff_end ? "" :",");
    }
    printf("]\n");

}


