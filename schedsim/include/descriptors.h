#ifndef __DESCRIPTORS_H__
#define __DESCRIPTORS_H__

#include <tasks.h>

typedef enum {

    CPU = 0,
    IO_HARD_DISK = 1,
    IO_KEYBOARD = 2

} TaskBehaviourType_t;

typedef struct task_behaviour {

    TaskBehaviourType_t type;
    unsigned int remainingTime;

    struct task_behaviour * next;
    struct task_behaviour * prev;

} TaskBehaviour_t;

typedef struct {

    unsigned int size;
    TaskBehaviour_t * first;
    TaskBehaviour_t * last;

} TaskBehaviourList_t;

typedef struct task_descriptor {

    PCB_t pcb;
    unsigned int startTime;
    unsigned int items;

    TaskBehaviour_t * current;

    TaskBehaviourList_t behaviours;

    struct task_descriptor * next;
    struct task_descriptor * prev;

} TaskDescriptor_t;

typedef struct {

    unsigned int size;
    TaskDescriptor_t * first;
    TaskDescriptor_t * last;

} TaskDescriptorList_t;

/**
 * @brief Initializes a behaviour descriptor for a given task
 *
 * This function initializes a behaviour descriptor of a given task.
 *
 * @param desc The descriptor to be initialized.
 *
 */
void initTaskBehaviour(TaskBehaviour_t * behaviour);

/**
 * @brief Initializes a descriptor for a given task
 *
 * This function initializes the descriptor of a given task.
 *
 * @param desc The descriptor to be initialized.
 *
 */
void initTaskDescriptor(TaskDescriptor_t * desc);

/**
 * @brief Appends a behaviour item to an existing list
 *
 * This function appends a new behaviour to an existing list.
 *
 * @param list The list of behaviours. 
 * @param behaviour The behaviour item to be appended.
 *
 */
void appendBehaviour(TaskBehaviourList_t * list, TaskBehaviour_t * behaviour);

/**
 * @brief Initializes a task descriptor list
 *
 * This function initializes a task descriptor list.
 *
 * @param list Pointer to the list to be initialized.
 *
 */
void initTaskDescriptorList(TaskDescriptorList_t * list);

/**
 * @brief Appends a task descriptor at the end of a list.
 *
 * This function appends a task descriptor at the end of the given list.
 *
 * @param list Pointer to the list.
 * @param desc Pointer to the descriptor to append.
 *
 */
void appendDescriptor(TaskDescriptorList_t * list, TaskDescriptor_t * desc);

#endif // __DESCRIPTORS_H__
