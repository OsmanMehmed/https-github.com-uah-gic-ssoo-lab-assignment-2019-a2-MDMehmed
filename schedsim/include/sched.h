#ifndef __SCHED_H__
#define __SCHED_H__

#include <tasks.h>

/**
 * @brief Scheduling function
 *
 * This function must select the next task to be executed. The selected task
 * must be extracted from the ready queue.
 *
 * @return The PCB of the next task to be executed.
 *
 */
PCB_t * schedule();

/**
 * @brief Start Task function
 *
 * This function is executed every time a new task enters in the system. The
 * function must incorporate the new task into the scheduling system.
 *
 * @param pcb Pointer to the PCB corresponding to the new task.
 *
 */
void startTask(PCB_t * pcb);

/**
 * @brief Exit Task function
 *
 * This function is exectued every time a task is terminated. The function
 * must eliminate the task from the scheduling system.
 *
 * @param pcb Pointer to the PCB corresponding to the terminated task.
 *
 */
void exitTask(PCB_t * pcb);

/**
 * @brief Clock Tick function
 *
 * This function is executed with every clock tick. The function receives
 * a pointer to the PCB of the task that is currently being executed on the
 * CPU. If no task is currently being executed, or if the task that was being
 * executed ended its burst before the clock tick, then the function shall
 * receive a NULL pointer.
 *
 * @param pcb Pointer to the PCB corresponding to the task that is currently
 * being executed on the CPU or NULL if no task is currently in execution.
 *
 */
void clockTick(PCB_t * pcb);

/**
 * @brief Yield for Hard Disk function
 *
 * This function is called when a task wants to access the Hard Disk.
 * If there was a previous task using the HD, then the new task should
 * be appended to the Hard Disk waiting queue until the current operation
 * is finished.
 *
 * @param pcb Pointer to the PCB that wants to the access the HD.
 *
 */
void yieldHardDisk(PCB_t * pcb);

/**
 * @brief Hard Disk Interrupt function
 *
 * This function is called whenever a Hard Disk interrupt occurs. The function
 * shall check if there was any other task waiting to access the HD and then
 * include the new task for scheduling.
 *
 */ 
void ioHardDiskIRQ(PCB_t * pcb);

/**
 * @brief Yield for Keyboard function
 
 * This function is called when a task wants to wait for a keypress on the
 * keyboard.  If there was a previous task waiting for the keyboard, then the
 * new task should be appended to the Keyboard waiting queue until the current
 * operation is finished.
 *
 * @param pcb Pointer to the PCB that wants to the access the keyboard.
 *
 */
void yieldKeyboard(PCB_t * pcb);

/**
 * @brief Keyboard Interrupt function
 *
 * This function is called whenever a Keyboard interrupt occurs. The function
 * shall check if there was any other task waiting for the keyboard and then
 * include the new task for scheduling.
 *
 */ 
void ioKeyboardIRQ(PCB_t * pcb);


#endif // __SCHED_H__
