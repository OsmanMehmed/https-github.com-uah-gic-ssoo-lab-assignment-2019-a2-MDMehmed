#ifndef __PARSER_H__
#define __PARSER_H__

#include <descriptors.h>

/**
 * @brief Parse a descriptor file
 *
 * This function parses a JSON file that contains a list of task descriptors.
 *
 * @param list Pointer to the list that will store the parsed descriptors.
 * @param fd File descriptor of the opened JSON file.
 *
 */
void parseDescriptors(TaskDescriptorList_t * list, int fd);


/**
 * @brief Frees the dynamic memory allocated during the parsing
 *
 * This function frees all the memory that was dynamically allocated when
 * the parsing process was performed.
 *
 * @param list Pointer to the list of descriptors.
 *
 */
void freeDescriptors(TaskDescriptorList_t * list);

#endif // __PARSER_H__
