#include <stdio.h>
#include <limits.h>

#include <sched.h>
#include <os.h>
#include <descriptors.h>

/** 
 * THE clock. Counts the number of ticks since the beginning of the 
 * execution
 */ 
unsigned int clock;

/** Next PID to be assigned */
unsigned int nextPID;

/** Number of tasks currenty living on the system */
unsigned int livingTasks;

/** Pointer to the task that is currently running */
static PCB_t * runningTask;

/** Pointer to the task that is currently using the hard disk */
static PCB_t * hardDiskTask;

/** Pointer to the task that is currently waiting for a keypress */
static PCB_t * keyboardTask;

/** THE ready queue */
static TaskQueue_t privateReadyQueue;
/** THE HD waiting queue */
static TaskQueue_t privateHardDiskWaitingQueue;
/** THE keyboard waiting queue */
static TaskQueue_t privateKeyboardWaitingQueue;

/** 
 * These pointers are used so that students do not
 * need to use any pointer operator whatsoever
 */

/** Pointer to the ready queue */
TaskQueue_t * readyQueue;
/** Pointer to the HD waiting queue */
TaskQueue_t * hardDiskWaitingQueue;
/** Pointer to the keyboard waiting queue */
TaskQueue_t * keyboardWaitingQueue;

void printStatus() {

    PCB_t * ready = NULL;
    PCB_t * keyboardWaiting = NULL;
    PCB_t * hardDiskWaiting = NULL;

    printf("%d\t%s\t\t", clock, runningTask != NULL ? runningTask->command : "(none)");

    if (readyQueue->size == 0) {

        printf("(none)");

    } else {

        for (ready = readyQueue->first; ready != NULL; ready = ready->next) {

            printf("%s", ready->command);

            if (ready->next != NULL) {

                printf(" -> ");

            }
        }
    }

    printf("\t\t");

    if (keyboardTask == NULL) {

        printf("(none)");

    } else {

        printf("%s", keyboardTask->command);

    }

    printf("\t\t");

    if (keyboardWaitingQueue->size == 0) {

        printf("(none)");

    } else {

        for (keyboardWaiting = keyboardWaitingQueue->first; keyboardWaiting != NULL; keyboardWaiting = keyboardWaiting->next) {

            printf("%s", keyboardWaiting->command);

            if (keyboardWaiting->next != NULL) {

                printf(" -> ");

            }
        }
    }

    printf("\t\t");

    if (hardDiskTask == NULL) {

        printf("(none)");

    } else {

        printf("%s", hardDiskTask->command);

    }

    printf("\t\t");

    if (hardDiskWaitingQueue->size == 0) {

        printf("(none)");

    } else {

        for (hardDiskWaiting = hardDiskWaitingQueue->first; hardDiskWaiting != NULL; hardDiskWaiting = hardDiskWaiting->next) {

            printf("%s", hardDiskWaiting->command);

            if (hardDiskWaiting->next != NULL) {

                printf(" -> ");

            }
        }
    }

    printf("\n");

}

void runOS(TaskDescriptorList_t * list) {

    int iterations = INT_MAX;

    PCB_t * pcb = NULL;
    PCB_t * previousRunningTask = NULL;
    PCB_t * previousHardDiskTask = NULL;
    PCB_t * previousKeyboardTask = NULL;

    TaskDescriptor_t * desc = NULL;

    readyQueue = &privateReadyQueue;
    hardDiskWaitingQueue = &privateHardDiskWaitingQueue;
    keyboardWaitingQueue = &privateKeyboardWaitingQueue;

    livingTasks = list->size;
    desc = list->first;

    printf("Time\tRunning\t\tReady\t\tKeyboard\tKbd Queue\tHard Disk\tHD Queue\n");

    // Start all tasks that start at boot time
    while (desc != NULL) {

        if (desc->startTime == 0) {

            pcb = (PCB_t *)desc;

            pcb->PID = nextPID;
            nextPID = nextPID + 1;
            
            startTask(pcb);

            printStatus();

        }

        desc = desc->next;

    }

    while (livingTasks != 0 && iterations != 0) {

        clock = clock + 1;

        previousRunningTask = runningTask;
        previousHardDiskTask = hardDiskTask;
        previousKeyboardTask = keyboardTask;

        // EXECUTION
        // 1. Check End Execuction Burst
        // 2. Tick interrupt
        // I/O
        // 3. End IO Keyboard Burst
        // 4. End IO Hard Disk Burst
        // START NEW TASK
        // 5. Start of a Task

        if (previousRunningTask != NULL) {

            desc = (TaskDescriptor_t *)previousRunningTask;
            desc->current->remainingTime = desc->current->remainingTime - 1;

            if (desc->current->remainingTime == 0) {

                desc->current = desc->current->next;

                if (desc->current == NULL) {

                    runningTask = NULL;

                    // If it was the last behaviour item -> exit task
                    livingTasks = livingTasks - 1;
                    exitTask(previousRunningTask);

                } else if (desc->current->type == IO_HARD_DISK) {

                    runningTask = NULL;

                    // If the next item is a hard disk burst -> block
                    yieldHardDisk(previousRunningTask);

                } else if (desc->current->type == IO_KEYBOARD) {

                    runningTask = NULL;

                    // If the next item is a keyboard burst -> block
                    yieldKeyboard(previousRunningTask);

                } // else -> current->type == CPU -> nothing

            }

        }
        
        // Launch tick interrupt
        
        if (runningTask != previousRunningTask) {

            clockTick(NULL);

        } else {

            clockTick(runningTask);

        }

        // Launch IO Hard Disk interrupt
        
        if (previousHardDiskTask != NULL) {

            desc = (TaskDescriptor_t *)previousHardDiskTask;
            desc->current->remainingTime = desc->current->remainingTime - 1;

            if (desc->current->remainingTime == 0) {

                desc->current = desc->current->next;

                if (desc->current == NULL) {
                    
                    hardDiskTask = NULL;

                    // If it was the last behaviour item -> exit task
                    livingTasks = livingTasks - 1;
                    exitTask(previousHardDiskTask);

                } else if (desc->current->type == CPU) {

                    // If the next item is CPU burst -> trigger IRQ
                    hardDiskTask = NULL;
                    ioHardDiskIRQ(previousHardDiskTask);

                } else if (desc->current->type == IO_KEYBOARD) {

                    // If the next item is a keyboard burst -> block
                    keyboardTask = NULL;
                    yieldKeyboard(previousHardDiskTask);

                } // else -> current->type == IO_HARD_DISK -> nothing

            }

        }

        // Launch IO Keboard interrupt
        
        if (previousKeyboardTask != NULL) {

            desc = (TaskDescriptor_t *)previousKeyboardTask;
            desc->current->remainingTime = desc->current->remainingTime - 1;

            if (desc->current->remainingTime == 0) {

                desc->current = desc->current->next;

                if (desc->current == NULL) {

                    keyboardTask = NULL;

                    // If it was the last behaviour item -> exit task
                    livingTasks = livingTasks - 1;
                    exitTask(previousKeyboardTask);

                } else if (desc->current->type == CPU) {

                    // If the next item is a CPU burst -> trigger IRQ
                    keyboardTask = NULL;
                    ioKeyboardIRQ(previousKeyboardTask);

                } else if (desc->current->type == IO_HARD_DISK) {

                    // If the next item is a hard disk burst -> block
                    keyboardTask = NULL;
                    yieldHardDisk(previousKeyboardTask);

                } // else -> current->type == IO_KEYBOARD -> nothing

            }

        }

        // Check if a new task starts
        
        desc = list->first;

        // Start all tasks that start at boot time
        while (desc != NULL) {

            if (desc->startTime == clock) {

                pcb = (PCB_t *)desc;

                pcb->PID = nextPID;
                nextPID = nextPID + 1;
                
                startTask(pcb);

            }

            desc = desc->next;

        }

        printStatus();

        iterations = iterations - 1;

    }

}

void dispatch(PCB_t * pcb) {

    runningTask = pcb;

}

void programHardDisk(PCB_t * pcb) {

    hardDiskTask = pcb;

}

void programKeyboard(PCB_t * pcb) {

    keyboardTask = pcb;

}

PCB_t * getRunningTask() {

    return runningTask;

}

PCB_t * getHardDiskWaitingTask() {

    return hardDiskTask;

}

PCB_t * getKeyboardWaitingTask() {

    return keyboardTask;

}
