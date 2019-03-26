#include <stdio.h>

#include <os.h>
#include <sched.h>

/** Pointer to the Ready Task Queue */
extern TaskQueue_t * readyQueue;

/** Pointer to the Hard Disk Waiting Task Queue */
extern TaskQueue_t * hardDiskWaitingQueue;

/** Pointer to the Keyboard Waiting Task Queue */
extern TaskQueue_t * keyboardWaitingQueue;

#define TIMESLICE 2

PCB_t * schedule() {

    // Return the first element of the ready queue
    return extractFirst(readyQueue);

}

void startTask(PCB_t * pcb) {

    // TODO: Complete the function

}

void exitTask(PCB_t * pcb) {

    // TODO: Complete the function

}

void clockTick(PCB_t * pcb) {

    PCB_t * nextToRun = NULL;

    // Check if there is a task currently running on the CPU
    if (pcb != NULL) {

        // If there is a task currently running, its timeslice must be updated
        unsigned int timeslice = getTimeslice(pcb);

        timeslice = timeslice - 1;

        setTimeslice(pcb, timeslice);

        // Check if its quantum has expired
        if (timeslice == 0) {

            // If its quantum has expired, put the task to ready state and
            // append it to the end of the ready queue
            setState(pcb, READY);
            appendPCB(readyQueue, pcb);

            // Check if there is a candidate for running on the CPU
            nextToRun = schedule();

            if (nextToRun != NULL) {

                // If there is a candidate, then set it to running state,
                // initialize its timeslice and dispatch it to the CPU
                setState(nextToRun, RUNNING);
                setTimeslice(nextToRun, TIMESLICE);

                dispatch(nextToRun);

            }

        }

    }

    return;

}

void yieldHardDisk(PCB_t * pcb) {

    // TODO: Complete the function

}

void ioHardDiskIRQ(PCB_t * pcb) {

    // TODO: Complete the function

}

void yieldKeyboard(PCB_t * pcb) {

    // TODO: Complete the function

}

void ioKeyboardIRQ(PCB_t * pcb) {

    // TODO: Complete the function

}
