#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

#include <lib/jsmn.h>

#include <parser.h>

/**
 * @brief Parse a string token from a JSON file.
 *
 * @param descriptors The string containing the contents of the JSON file.
 * @param tokens Pointer to the token identifier of the string to be parsed.
 *
 * @return The parsed string. The memory that stores the string is
 *         dynamically allocted.
 *
 */
char *parseString(char *descriptors, jsmntok_t *tokens) {

    int length = 0;
    char *tmpString = NULL;

    // Size of the memory chunk to be allocated = size of the string + '\0'
    length = tokens[0].end - tokens[0].start + 1;

    tmpString = (char *)malloc(length);

    // Copy the string from the file to the buffer
    strncpy(tmpString, descriptors + tokens[0].start, length - 1);

    // Add the EOS character
    tmpString[length - 1] = '\0';

    return tmpString;
}

/**
 * @brief Parse a decimal number from a JSON file.
 *
 * @param descriptors The string containing the contents of the JSON file.
 * @param tokens Pointer to the token identifier of the decimal number to be
 * parsed.
 *
 * @return The parsed decimal number.
 *
 */
unsigned long parseDecimal(char *descriptors, jsmntok_t *tokens) {

    int length = 0;
    char *tmpString = NULL, *endString = NULL;
    unsigned long result = 0;

    length = tokens[0].end - tokens[0].start + 1;

    tmpString = (char *)malloc(length);

    strncpy(tmpString, descriptors + tokens[0].start, length - 1);

    tmpString[length - 1] = '\0';

    result = strtoul(tmpString, &endString, 10);

    if (*endString != '\0') {

        fprintf(stderr, "Invalid decimal integer at: %d\n", tokens->start);
        exit(-1);
    }

    free(tmpString);

    return result;
}

unsigned int parseStartTime(TaskDescriptor_t *desc, char *descriptors,
                            jsmntok_t *tokens) {

    if (tokens[0].type == JSMN_PRIMITIVE && tokens[0].size == 0) {

        desc->startTime = parseDecimal(descriptors, tokens);

    } else {

        fprintf(stderr, "Invalid start time value at: %d\n", tokens->start);
        exit(-1);
    }

    return 1;
}

unsigned int parsePriority(TaskDescriptor_t *desc, char *descriptors,
                            jsmntok_t *tokens) {

    if (tokens[0].type == JSMN_PRIMITIVE && tokens[0].size == 0) {

        desc->pcb.priority = parseDecimal(descriptors, tokens);

    } else {

        fprintf(stderr, "Invalid priority value at: %d\n", tokens->start);
        exit(-1);
    }

    return 1;
}

unsigned int parseBehaviourDuration(TaskBehaviour_t *behaviour,
                                    char *descriptors, jsmntok_t *tokens) {

    if (tokens[0].type == JSMN_PRIMITIVE && tokens[0].size == 0) {

        behaviour->remainingTime = parseDecimal(descriptors, tokens);

    } else {

        fprintf(stderr, "Invalid duration time value at: %d\n", tokens->start);
        exit(-1);
    }

    return 1;
}

unsigned int parseBehaviourType(TaskBehaviour_t *behaviour, char *descriptors,
                                jsmntok_t *tokens) {

    TaskBehaviourType_t type = 0;

    if (tokens[0].type == JSMN_PRIMITIVE && tokens[0].size == 0) {

        type = parseDecimal(descriptors, tokens);

        if (type != CPU && type != IO_HARD_DISK && type != IO_KEYBOARD) {

            fprintf(
                stderr,
                "Invalid behaviour type at: %d\n, valid values are 0, 1 or 2",
                tokens->start);
            exit(-1);
        }

        behaviour->type = type;

    } else {

        fprintf(stderr,
                "Invalid behaviour type at: %d\n, valid values are 0, 1 or 2",
                tokens->start);
        exit(-1);
    }

    return 1;
}

unsigned int parseCommand(TaskDescriptor_t *desc, char *descriptors,
                          jsmntok_t *tokens) {

    if (tokens[0].type == JSMN_STRING && tokens[0].size == 0) {

        desc->pcb.command = parseString(descriptors, tokens);

    } else {

        fprintf(stderr, "Invalid command value at: %d\n", tokens->start);
        exit(-1);
    }

    return 1;
}

unsigned int parseBehaviour(TaskDescriptor_t *desc, char *descriptors,
                            jsmntok_t *tokens) {

    TaskBehaviour_t *behaviour = NULL;
    unsigned int currPtr = 0;
    int foundType = 0, foundDuration = 0;

    if (tokens->size != 2) {

        fprintf(stderr, "Malformed behaviour desciptor at character %d: it "
                        "must contain two objects: \"type\" and \"duration\"\n",
                tokens->start);
        exit(-1);
    }

    // Allocate the new behaviour descriptor
    behaviour = (TaskBehaviour_t *)malloc(sizeof(TaskBehaviour_t));

    if (behaviour == NULL) {

        perror("Not enough memory for parsing the JSON file");
        exit(-1);
    }

    initTaskBehaviour(behaviour);

    currPtr = 1;

    while (foundType == 0 || foundDuration == 0) {

        if (tokens[currPtr].type == JSMN_STRING && tokens[currPtr].size == 1 &&
            strncmp("type", descriptors + tokens[currPtr].start,
                    tokens[currPtr].end - tokens[currPtr].start) == 0 &&
            (tokens[currPtr].end - tokens[currPtr].start) == 4) {

            foundType = 1;

            currPtr = currPtr + 1;

            currPtr = currPtr + parseBehaviourType(behaviour, descriptors,
                                                   tokens + currPtr);

        } else if (tokens[currPtr].type == JSMN_STRING &&
                   tokens[currPtr].size == 1 &&
                   strncmp("duration", descriptors + tokens[currPtr].start,
                           tokens[currPtr].end - tokens[currPtr].start) == 0 &&
                   (tokens[currPtr].end - tokens[currPtr].start) == 8) {

            foundDuration = 1;

            currPtr = currPtr + 1;

            currPtr = currPtr + parseBehaviourDuration(behaviour, descriptors,
                                                       tokens + currPtr);

        } else {

            fprintf(stderr, "Unknown behaviour desciptor field at character "
                            "%d: they must only be: \"type\" or \"duration\"\n",
                    tokens[currPtr].start);
            exit(-1);
        }
    }

    appendBehaviour(&(desc->behaviours), behaviour);

    return currPtr;
}

unsigned int parseDescriptor(TaskDescriptorList_t *list, char *descriptors,
                             jsmntok_t *tokens) {

    TaskDescriptor_t *desc = NULL;
    unsigned int currPtr = 0, arrayPtr = 0;
    int foundStartTime = 0, foundBehaviour = 0;
    int foundPriority = 0, foundCommand = 0;
    int i = 0;

    if (tokens->size != 4) {

        fprintf(stderr, "Malformed task desciptor at character %d: it must "
                        "contain three objects: \"command\", \"start_time\", "
                        "\"priority\" and \"behaviour\"\n",
                tokens->start);
        exit(-1);
    }

    // Allocate the new task descriptor
    desc = (TaskDescriptor_t *)malloc(sizeof(TaskDescriptor_t));

    if (desc == NULL) {

        perror("Not enough memory for parsing the JSON file");
        exit(-1);
    }

    initTaskDescriptor(desc);

    currPtr = 1;

    while (foundStartTime == 0 || foundBehaviour == 0 || 
           foundPriority == 0 || foundCommand == 0) {

        if (tokens[currPtr].type == JSMN_STRING && tokens[currPtr].size == 1 &&
            strncmp("behaviour", descriptors + tokens[currPtr].start,
                    tokens[currPtr].end - tokens[currPtr].start) == 0 &&
            (tokens[currPtr].end - tokens[currPtr].start) == 9) {

            foundBehaviour = 1;

            currPtr = currPtr + 1;

            if (tokens[currPtr].type == JSMN_OBJECT) {

                currPtr = currPtr +
                          parseBehaviour(desc, descriptors, tokens + currPtr);

            } else if (tokens[currPtr].type == JSMN_ARRAY) {

                arrayPtr = currPtr;

                currPtr = currPtr + 1;

                for (i = 0; i < tokens[arrayPtr].size; i++) {

                    currPtr = currPtr + parseBehaviour(desc, descriptors,
                                                       tokens + currPtr);
                }

            } else {

                fprintf(
                    stderr,
                    "Invalid \"behaviour\" descriptor field at character %d\n",
                    tokens[currPtr].start);
                exit(-1);
            }

        } else if (tokens[currPtr].type == JSMN_STRING &&
                   tokens[currPtr].size == 1 &&
                   strncmp("start_time", descriptors + tokens[currPtr].start,
                           tokens[currPtr].end - tokens[currPtr].start) == 0 &&
                   (tokens[currPtr].end - tokens[currPtr].start) == 10) {

            foundStartTime = 1;

            currPtr = currPtr + 1;

            currPtr =
                currPtr + parseStartTime(desc, descriptors, tokens + currPtr);

        } else if (tokens[currPtr].type == JSMN_STRING &&
                   tokens[currPtr].size == 1 &&
                   strncmp("priority", descriptors + tokens[currPtr].start,
                           tokens[currPtr].end - tokens[currPtr].start) == 0 &&
                   (tokens[currPtr].end - tokens[currPtr].start) == 8) {

            foundPriority = 1;

            currPtr = currPtr + 1;

            currPtr =
                currPtr + parsePriority(desc, descriptors, tokens + currPtr);


        } else if (tokens[currPtr].type == JSMN_STRING &&
                   tokens[currPtr].size == 1 &&
                   strncmp("command", descriptors + tokens[currPtr].start,
                           tokens[currPtr].end - tokens[currPtr].start) == 0 &&
                   (tokens[currPtr].end - tokens[currPtr].start) == 7) {

            foundCommand = 1;

            currPtr = currPtr + 1;

            currPtr =
                currPtr + parseCommand(desc, descriptors, tokens + currPtr);

        } else {

            fprintf(stderr, "Unknown task desciptor field at character %d: "
                            "they must only be: \"command\", \"start_time\", "
                            "or \"behaviour\"\n",
                    tokens[currPtr].start);
            exit(-1);
        }
    }

    desc->current = desc->behaviours.first;

    appendDescriptor(list, desc);

    return currPtr;
}

void parseDescriptors(TaskDescriptorList_t *list, int fd) {

    char *descriptors = NULL;
    jsmntok_t *tokens = NULL;

    off_t size = 0;
    int count = 0, i = 0;
    unsigned int currPtr = 0;

    jsmn_parser parser;

    jsmn_init(&parser);

    // Now we have to find the size of the file

    size = lseek(fd, 0, SEEK_END);

    if (size < 0) {

        perror("Error accessing descriptors file:");
        exit(-1);
    }

    // Then we have to map the file into memory so that we can parse it

    descriptors = (char *)mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);

    if (descriptors == NULL) {

        perror("Error mapping descriptors file:");
        exit(-1);
    }

    // And then we can parse it. First we need to know how many tokens are
    // there on the file.

    count = jsmn_parse(&parser, descriptors, size, NULL, 0);

    if (count < 0) {

        fprintf(stderr, "Error when parsing the JSON file");

        switch (count) {
        case JSMN_ERROR_INVAL:
            fprintf(stderr, ": invalid character at position %d\n", parser.pos);
            break;
        case JSMN_ERROR_PART:
            fprintf(stderr, ": premature end of input\n");
            break;
        default:
            break;
        }
        exit(-1);
    }

    // Now we have to allocate memory to store all the tokens

    tokens = (jsmntok_t *)malloc(count * sizeof(jsmntok_t));

    if (tokens == NULL) {

        perror("Not enough memory for parsing the JSON file");
        exit(-1);
    }

    jsmn_init(&parser);

    count = jsmn_parse(&parser, descriptors, size, tokens, count);

    // Now we need to check that the JSON contains only one element called
    // "tasks"

    if (tokens[0].type != JSMN_OBJECT || tokens[0].size != 1 ||
        (tokens[0].end - tokens[0].start + 1) != size) {

        fprintf(stderr, "Malformed JSON file: it shall only contain one object "
                        "called \"tasks\"\n");
        exit(-1);
    }

    if (tokens[1].type != JSMN_STRING || tokens[1].size != 1 ||
        strncmp("tasks", descriptors + tokens[1].start,
                tokens[1].end - tokens[1].start) != 0 ||
        (tokens[1].end - tokens[1].start) != 5) {

        fprintf(stderr, "Malformed JSON file: it shall only contain one "
                        "element called \"tasks\"\n");
        exit(-1);
    }

    if (tokens[2].type == JSMN_OBJECT) {

        // Parse a single task, which does not make much sense
        parseDescriptor(list, descriptors, tokens + 3);

    } else if (tokens[2].type == JSMN_ARRAY) {

        // Parse an array of objects
        currPtr = 3;

        for (i = 0; i < tokens[2].size; i++) {

            currPtr =
                currPtr + parseDescriptor(list, descriptors, tokens + currPtr);
        }

    } else {

        fprintf(stderr, "Malformed JSON file: the description must be an "
                        "object or an array\n");
        exit(-1);
    }

    free(tokens);

    munmap(descriptors, size);
}

void freeDescriptors(TaskDescriptorList_t *list) {

    TaskDescriptor_t *currDescriptor = NULL, *freeDescriptor = NULL;
    TaskBehaviour_t *currBehaviour = NULL, *freeBehaviour = NULL;

    currDescriptor = list->first;

    while (currDescriptor != NULL) {

        currBehaviour = currDescriptor->behaviours.first;

        while (currBehaviour != NULL) {

            freeBehaviour = currBehaviour;

            currBehaviour = currBehaviour->next;

            free(freeBehaviour);
        }

        freeDescriptor = currDescriptor;

        currDescriptor = currDescriptor->next;

        free(freeDescriptor->pcb.command);

        free(freeDescriptor);
    }
}
