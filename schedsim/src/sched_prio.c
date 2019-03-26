#include <stdio.h>

#include <os.h>
#include <sched.h>

/** Pointer to the Ready Task Queue */
extern TaskQueue_t * readyQueue;

/** Pointer to the Hard Disk Waiting Task Queue */
extern TaskQueue_t * hardDiskWaitingQueue;

/** Pointer to the Keyboard Waiting Task Queue */
extern TaskQueue_t * keyboardWaitingQueue;

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

    // Nothing to do with a priority-based scheduling policy
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
