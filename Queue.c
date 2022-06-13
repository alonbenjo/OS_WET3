//
// Created by student on 6/13/22.
//

#include "Queue.h"
#include <stdlib.h>


struct Queue {
    int max_size;
    QueueElement* buff_end;
    QueueElement* start_ptr;
    QueueElement* end_ptr;
    QueueElement buff[];
};

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
    queue->buff_end = queue->buff + max_size + 1;
    return queue;
}

void queueDestroy(Queue* queue_ptr){
    if(queue_ptr == NULL)
        return;
    Queue queue = *queue_ptr;
    for (QueueElement* ptr = queue->start_ptr; ptr != queue->end_ptr; inc_buff(queue, &ptr)) {
        destroyQueueElement(&ptr);
    }
    free(queue->buff);
}

QueueResult queueInsert(Queue queue, QueueElement* input){
    if(queue == NULL)
        return QUEUE_BAD_ARG;
    if(queueIsFull(queue)){
        return QUEUE_FULL;
    }
    *(queue->end_ptr) = input;
    inc_buff(queue, &queue->end_ptr);
    return QUEUE_SUCCESS;
}

QueueResult queueRemove(Queue queue, str* output){
    if(queue == NULL)
        return QUEUE_BAD_ARG;
    if(queueIsEmpty(queue)) {
        return QUEUE_EMPTY;
    }
    int len = strlen(*queue->start_ptr);
    output = calloc(len + 1, sizeof (char));
    if(output == NULL){
        return QUEUE_ALLOC_ERROR;
    }
    strcpy(*output, *queue->start_ptr);
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
void inc_buff(Queue queue, str** index_ptr){
    (*index_ptr)++;
    if((*index_ptr) > queue->buff_end)
        *index_ptr = queue->buff;
}