//metadoc PriorityQueue copyright Lewis Weinberger, 2021
//metadoc PriorityQueue license MIT
//metadoc PriorityQueue category Algorithms
/*metadoc PriorityQueue description
An addon providing a simple min-heap priority queue.
*/

#include "IoState.h"
#include "IoNumber.h"

#include "IoPriorityQueue.h"

#define DATA(self) ((IoPriorityQueueData *)IoObject_dataPointer(self))

static const char *protoId = "PriorityQueue";

IoTag *IoPriorityQueue_newTag(void *state) {
    IoTag *tag = IoTag_newWithName_(protoId);
    IoTag_state_(tag, state);
    IoTag_freeFunc_(tag, (IoTagFreeFunc *)IoPriorityQueue_free);
    IoTag_cloneFunc_(tag, (IoTagCloneFunc *)IoPriorityQueue_rawClone);
    return tag;
}

IoPriorityQueue *IoPriorityQueue_proto(void *state) {
    IoObject *self = IoObject_new(state);
    IoObject_tag_(self, IoPriorityQueue_newTag(state));

    IoObject_setDataPointer_(self, calloc(1, sizeof(IoPriorityQueueData)));
    DATA(self)->heap = calloc(ALLOC_SIZE, sizeof(Node));
    DATA(self)->size = 0;
    DATA(self)->memSize = ALLOC_SIZE;

    IoState_registerProtoWithId_(state, self, protoId);

    {
        IoMethodTable methodTable[] = {
            {"with", IoPriorityQueue_with},
            {"push", IoPriorityQueue_push},
            {"pop", IoPriorityQueue_pop},
            {"size", IoPriorityQueue_size},
            {NULL, NULL},
        };
        
        IoObject_addMethodTable_(self, methodTable);
    }

    return self;
}

IoPriorityQueue *IoPriorityQueue_rawClone(IoPriorityQueue *proto) {
    IoPriorityQueue *self = IoObject_rawClonePrimitive(proto);
    IoObject_setDataPointer_(self, cpalloc(DATA(proto), sizeof(IoPriorityQueueData)));
    DATA(self)->heap = cpalloc(DATA(proto)->heap, DATA(proto)->memSize * sizeof(Node));
    return self;
}

IoPriorityQueue *IoPriorityQueue_new(void *state) {
    IoObject *proto = IoState_protoWithId_(state, protoId);
    return IOCLONE(proto);
}

void IoPriorityQueue_free(IoPriorityQueue *self) {
    free(DATA(self)->heap);
    free(IoObject_dataPointer(self));
}


/* Methods ---------------------------------------------------- */


/* Append to the heap, resizing as necessary */
static void appendToHeap(IoPriorityQueueData *queue, void *value, int priority) {
    if(queue->size == queue->memSize) {
        queue->memSize *= ALLOC_RESIZE;
        queue->heap = realloc(queue->heap, queue->memSize * sizeof(Node));
    }
    queue->heap[queue->size].value = value;
    queue->heap[queue->size++].priority = priority;
}

/* Heapify the queue */
static void minHeapify(IoPriorityQueueData *queue, int index) {
    int i, min, child;
    Node tmp;
    
    min = index;
    for(i = 1; i < 3; i++) {
        child = i + 2 * index;

        if(child < queue->size 
                && queue->heap[min].priority > queue->heap[child].priority) {
            min = child;
        }
    }

    if(min != index) {
        tmp = queue->heap[index];
        queue->heap[index] = queue->heap[min];
        queue->heap[min] = tmp;
        minHeapify(queue, min);
    }
}

IO_METHOD(IoPriorityQueue, with) {
    /*doc PriorityQueue with(maxSize)
    Create a queue with preallocated heap of maxSize bytes.
    */

    int maxSize = IoMessage_locals_intArgAt_(m, locals, 0);

    IOASSERT(maxSize > 0, "PriorityQueue with() requires a positive maxSize");

    IoPriorityQueue *ioPriorityQueue = IOCLONE(self);
    DATA(self)->memSize = maxSize;
    DATA(self)->heap = realloc(DATA(self)->heap, maxSize * sizeof(Node));
    return ioPriorityQueue;
}

IO_METHOD(IoPriorityQueue, push) {
    /*doc PriorityQueue push(aValue, aPriority)
    Push the new value onto the queue with the given integer priority.
    */
    
    int i, priority;
    IoObject *value;

    IOASSERT(IoMessage_argCount(m) == 2, "PriorityQueue push() requires two arguments");
    
    value = IoMessage_locals_valueArgAt_(m, locals, 0);
    priority = IoMessage_locals_intArgAt_(m, locals, 1);

    appendToHeap(DATA(self), IOREF(value), priority);
    if(DATA(self)->size > 0) {
        for(i = DATA(self)->size / 2 - 1; i > -1; i--) {
            minHeapify(DATA(self), i);
        }
    }

    IoObject_isDirty_(self, 1);

    return self;
}

IO_METHOD(IoPriorityQueue, pop) {
    /*doc PriorityQueue pop
    Remove and return the next element from the queue.
    Note: as this is a min-heap queue,
    the highest "priority" is actually the lowest value.
    */
    
    IoObject *result;

    if(DATA(self)->size == 0) {
        return IONIL(self);
    }
    result = DATA(self)->heap[0].value;
    DATA(self)->heap[0] = DATA(self)->heap[--(DATA(self)->size)];
    minHeapify(DATA(self), 0);

    IoObject_isDirty_(self, 1);

    return result;
}

IO_METHOD(IoPriorityQueue, size) {
    /*doc PriorityQueue size
    Returns the size of the queue.
    */

    return IONUMBER(DATA(self)->size);
}
