#include <stdio.h>

#include <tasks.h>

void initPCB(PCB_t * pcb, unsigned int PID, char * command,
             unsigned int priority, unsigned int timeslice) {

    pcb->PID = PID;
    pcb->state = INIT;
    pcb->command = command;
    pcb->priority = priority;
    pcb->timeslice = timeslice; 
    
    pcb->next = NULL;
    pcb->prev = NULL;

}

unsigned int getPriority(PCB_t * pcb) {

    return pcb->priority;

}

void setPriority(PCB_t * pcb, unsigned int priority) {

    pcb->priority = priority;

}

unsigned int getTimeslice(PCB_t * pcb) {

    return pcb->timeslice;

}

void setTimeslice(PCB_t * pcb, unsigned int timeslice) {

    pcb->timeslice = timeslice;

}

ProcessState_t getState(PCB_t * pcb) {

    return pcb->state;

}

void setState(PCB_t * pcb, ProcessState_t state) {

    pcb->state = state;

}

void initQueue(TaskQueue_t * queue) {

    queue->first = queue->last = NULL;
    queue->size = 0;

}

void appendPCB(TaskQueue_t * queue, PCB_t * pcb) {

    if (queue->size == 0) {

        queue->first = queue->last = pcb;
        queue->size = 1;

    } else {

        // Previous last element of the queue
        PCB_t * prevLast = queue->last;

        // 1. Make the new PCB point to NULL
        pcb->next = NULL;

        // 2. Make the previous last element point to the new element.
        prevLast->next = pcb;

        // 3. Make the new PCB point to the previous element, i.e.
        // the previous last element.
        pcb->prev = prevLast;

        // 4. Update the queue's last element to the new element
        queue->last = pcb;

        // 5. Update the number of elements of the queue
        queue->size = queue->size + 1;

    }

}

void addPCBByPriority(TaskQueue_t * queue, PCB_t * pcb) {

    // If the queue is empty, then just add it
    if (queue->size == 0) {

        queue->first = queue->last = pcb;
        queue->size = 1;

    } else {
    
        // Previous first element of the queue
        PCB_t * prevFirst = queue->first;

        // Previous last element of the queue
        PCB_t * prevLast = queue->last;
        
        // Check if the element to add should be at the beginning, at the end
        // or in the middle of the queue
        if (prevFirst->priority < pcb->priority) {

            // The priority of the new element is greater than that of the
            // previous first element -> the new element becomes the first
            // element

            // 1. Make the new PCB point to NULL
            pcb->prev = NULL;

            // 2. Make the new PCB point to the previous first
            pcb->next = prevFirst;
            
            // 3. Make the previous first point to the new PCB
            prevFirst->prev = pcb;

            // 4. Update the queue's first element to the new element
            queue->first = pcb;

        } else if (prevLast->priority >= pcb->priority) {

            // The priority of the new element is lower than than that of the
            // previous last element -> the new element becomes the last
            // element

            // 1. Make the new PCB point to NULL
            pcb->next = NULL;

            // 2. Make the previous last element point to the new element.
            prevLast->next = pcb;

            // 3. Make the new PCB point to the previous element, i.e.
            // the previous last element.
            pcb->prev = prevLast;

            // 4. Update the queue's last element to the new element
            queue->last = pcb;

        } else {

            // We need to find the location of the new element
            PCB_t * nextElement = prevFirst->next;

            do {

                // Check if the new element should be located before the next
                // one
                if (nextElement->priority < pcb->priority) {

                    PCB_t * prevElement = nextElement->prev;

                    // 1. Make the new PCB point to the previous element
                    pcb->prev = prevElement;

                    // 2. Make the previous element point to the new element
                    prevElement->next = pcb;

                    // 3. Make the new PCB point to the next element
                    pcb->next = nextElement;

                    // 4. Make the previous next element point to the new PCB
                    nextElement->prev = pcb;

                    break;

                } else {

                    nextElement = nextElement->next;

                }



            } while (nextElement != NULL);

        }

        // Update the number of elements of the queue
        queue->size = queue->size + 1;

    }

}

PCB_t * extractFirst(TaskQueue_t * queue) {

    PCB_t * first = queue->first;
    PCB_t * next = NULL;

    // If the queue is empty -> nothing to return
    if (queue->size == 0) {

        return NULL;

    } else if (queue->size == 1) {

        queue->first = queue->last = NULL;
        queue->size = 0;

        return first;

    } else {

        next = first->next;

        next->prev = NULL; 
        queue->first = next;
        queue->size = queue->size - 1;

        return first;

    }

}

PCB_t * extractLast(TaskQueue_t * queue) {

    PCB_t * last = queue->last;
    PCB_t * prev = NULL;

    // If the queue is empty -> nothing to do
    if (queue->size == 0) {

        return NULL;

    } else if (queue->size == 1) {

        queue->first = queue->last = NULL;
        queue->size = 0;

        return last;

    } else {

        prev = last->prev;

        prev->next = NULL;
        queue->last = prev;
        queue->size = queue->size - 1;

        return last;

    } 

}
