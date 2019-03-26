#ifndef __TASKS_H__
#define __TASKS_H__

typedef enum {

    INIT = 0,
    READY = 1,
    RUNNING = 2,
    WAITING = 3,
    FINISHED = 4

} ProcessState_t;

typedef struct pcb {

    unsigned int PID;
    ProcessState_t state;

    char * command;

    unsigned int priority;
    unsigned int timeslice;
    
    struct pcb * next;
    struct pcb * prev;

} PCB_t;

typedef struct {

    unsigned int size;
    PCB_t * first;
    PCB_t * last;

} TaskQueue_t;

/**
 * @brief Initializes a PCB structure.
 *
 * @param pcb Pointer to the PCB to initialize.
 * @param PID PID of the process.
 * @param command Full command that launched the process
 * @param priority Initial priority of the process.
 * @param timeslice Initial timeslice of the process.
 */
void initPCB(PCB_t * pcb, unsigned int PID, char * command,
             unsigned int priority, unsigned int timeslice);

/**
 * @brief Returns the priority of a task.
 *
 * @param pcb Pointer to the PCB of the task.
 *
 * @return The priority of the task.
 *
 */
unsigned int getPriority(PCB_t * pcb);

/**
 * @brief Sets the priority of a task.
 *
 * @param pcb Pointer to the PCB of the task.
 * @param priority The new priority of the task.
 *
 */
void setPriority(PCB_t * pcb, unsigned int priority);

/**
 * @brief Returns the state of a task.
 *
 * @param pcb Pointer to the PCB of the task.
 *
 * @return The state of the task.
 *
 */
ProcessState_t getState(PCB_t * pcb);

/**
 * @brief Sets the state of a task.
 *
 * @param pcb Pointer to the PCB of the task.
 * @param priority The new state of the task.
 *
 */
void setState(PCB_t * pcb, ProcessState_t state);


/**
 * @brief Returns the value of the remaining timeslice of a task.
 *
 * @param pcb Pointer to the PCB of the task.
 *
 * @return The remaining timeslice of the task.
 *
 */
unsigned int getTimeslice(PCB_t * pcb);

/**
 * @brief Sets the value of the remaining timeslice of a task.
 *
 * @param pcb Pointer to the PCB of the task.
 * @param timeslice The new value of the timeslice of the task.
 *
 */
void setTimeslice(PCB_t * pcb, unsigned int timeslice);

/**
 * @brief Initializes a task queue.
 *
 * @param queue Pointer to the queue.
 *
 */
void initQueue(TaskQueue_t * queue);

/**
 * @brief Appends a PCB at the end of a queue.
 *
 * This function appends a PCB at the end of the given queue.
 *
 * @param queue Pointer to the queue to which the PCB will be appended.
 * @param pcb Pointer to the PCB to append.
 *
 */
void appendPCB(TaskQueue_t * queue, PCB_t * pcb);

/**
 * @brief Adds a PCB on a queue ordered by its priority.
 *
 * This function adds a given PCB at the location corresponding to its
 * priority.
 *
 * @param queue Pointer to the queue to which the PCB will be added.
 * @param pcb Pointer to the PCB to be added.
 *
 */
void addPCBByPriority(TaskQueue_t * queue, PCB_t * pcb);

/**
 * @brief Extracts the first PCB of a queue.
 *
 * This function extract the first element of a queue. If the queue was empty,
 * the function will return NULL.
 *
 * @param queue Pointer to the queue.
 *
 * @return Pointer to the first PCB of the queue or NULL if the queue was empty.
 *
 */
PCB_t * extractFirst(TaskQueue_t * queue);

/**
 * @brief Extracts the last PCB of a queue.
 *
 * This function extract the last element of a queue. If the queue was empty,
 * the function will return NULL. 
 *
 * @param queue Pointer to the queue.
 *
 * @return Pointer to the last PCB of the queue or NULL if the queue was empty.
 *
 */
PCB_t * extractLast(TaskQueue_t * queue);

/**
 * @brief Returns the current size of the queue
 *
 * @param queue Pointer to the queue.
 *
 * @return The size of the queue.
 */
unsigned int getSize(TaskQueue_t * queue);

#endif // __TASKS_H__
