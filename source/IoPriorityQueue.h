//metadoc PriorityQueue copyright Lewis Weinberger, 2021
//metadoc PriorityQueue license MIT

#ifndef IOPRIORITYQUEUE_DEFINED
#define IOPRIORITYQUEUE_DEFINED 1

#include <stdlib.h>
#include "IoObject.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ALLOC_SIZE 1024
#define ALLOC_RESIZE 2

typedef IoObject IoPriorityQueue;

typedef struct
{
    void *value;
    int priority;
} Node;

typedef struct
{
    Node *heap;
    size_t size;
    size_t memSize;
} IoPriorityQueueData;

IoPriorityQueue *IoPriorityQueue_proto(void *);
IoPriorityQueue *IoPriorityQueue_rawClone(IoPriorityQueue *);
IoPriorityQueue *IoPriorityQueue_new(void *);
void IoPriorityQueue_free(IoPriorityQueue *);

IoObject *IoPriorityQueue_with(IoObject *, IoObject *, IoObject *);
IoObject *IoPriorityQueue_push(IoObject *, IoObject *, IoObject *);
IoObject *IoPriorityQueue_pop(IoObject *, IoObject *, IoObject *);
IoObject *IoPriorityQueue_size(IoObject *, IoObject *, IoObject *);
IoObject *IoPriorityQueue_capacity(IoObject *, IoObject *, IoObject *);

#ifdef __cplusplus
}
#endif
#endif
