#ifndef __OS_H__
#define __OS_H__

#include <tasks.h>

/**
 * @brief Dispatches a task.
 *
 * This function dispatches a task to execution.
 *
 * @param pcb Pointer to the PCB to dispatch.
 */
void dispatch(PCB_t * pcb);

/**
 * @brief Programs a hard disk operation on behalf of a given task.
 *
 * This function simulates the programming of a hard disk operation on behalf
 * of a given task.
 *
 * @param pcb Pointer to the PCB of the task.
 */
void programHardDisk(PCB_t * pcb);

/**
 * @brief Programs a keyboard operation on behalf of a given task.
 *
 * This function simulates the programming of a keyboard waiting operation on behalf
 * of a given task.
 *
 * @param pcb Pointer to the PCB of the task.
 */
void programKeyboard(PCB_t * pcb);


/**
 * @brief Returns the current running task
 *
 * @return Pointer to the PCB of the running task.
 */
PCB_t * getRunningTask();

/**
 * @brief Returns the current hard disk waiting task
 *
 * @return Pointer to the PCB of the waiting task.
 */
PCB_t * getHardDiskWaitingTask();

/**
 * @brief Returns the current keyboard waiting task
 *
 * @return Pointer to the PCB of the waiting task.
 */
PCB_t * getKeyboardWaitingTask();

#endif // __OS_H__
