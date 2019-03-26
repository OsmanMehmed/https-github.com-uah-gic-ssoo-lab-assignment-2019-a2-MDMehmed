#include <stdio.h>

#include <descriptors.h>


void initTaskBehaviour(TaskBehaviour_t * behaviour) {

    behaviour->type = 0;
    behaviour->remainingTime = 0;

    behaviour->next = NULL;
    behaviour->prev = NULL;

}

static void initTaskBehaviourList(TaskBehaviourList_t * list) {

    list->size = 0;

    list->first = NULL;
    list->last = NULL;

}

void initTaskDescriptor(TaskDescriptor_t * desc) {

    desc->startTime = 0;
    desc->items = 0;
    desc->current = NULL;

    initTaskBehaviourList(&(desc->behaviours));

    desc->next = NULL;
    desc->prev = NULL;

}

void initTaskDescriptorList(TaskDescriptorList_t * list) {

    list->size = 0;

    list->first = NULL;
    list->last = NULL; 

}

void appendDescriptor(TaskDescriptorList_t * list, TaskDescriptor_t * desc) {

    if (list->size == 0) {

        list->first = list->last = desc;
        list->size = 1;

    } else {

        // Previous last element of the list
        TaskDescriptor_t * prevLast = list->last;

        // 1. Make the new descriptor point to NULL
        desc->next = NULL;

        // 2. Make the previous last element point to the new element.
        prevLast->next = desc;

        // 3. Make the new descriptor point to the previous element, i.e.
        // the previous last element.
        desc->prev = prevLast;

        // 4. Update the list's last element to the new element
        list->last = desc;

        // 5. Update the number of elements of the list
        list->size = list->size + 1;

    }

}

void appendBehaviour(TaskBehaviourList_t * list, TaskBehaviour_t * behaviour) {

    if (list->size == 0) {

        list->first = list->last = behaviour;
        list->size = 1;

    } else {

        // Previous last element of the list
        TaskBehaviour_t * prevLast = list->last;

        // 1. Make the new behaviour point to NULL
        behaviour->next = NULL;

        // 2. Make the previous last element point to the new element.
        prevLast->next = behaviour;

        // 3. Make the new behaviour point to the previous element, i.e.
        // the previous last element.
        behaviour->prev = prevLast;

        // 4. Update the list's last element to the new element
        list->last = behaviour;

        // 5. Update the number of elements of the list
        list->size = list->size + 1;

    }

}
