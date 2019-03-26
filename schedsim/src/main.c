#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <descriptors.h>
#include <parser.h>
#include <os.h>

void runOS(TaskDescriptorList_t * list);

int main(int argc, char * argv[]) {

    int fd = 0;

    TaskDescriptorList_t list;

    if (argc != 2) {

        fprintf(stderr, "Usage: schedsim task_descriptors\n");
        exit(-1);

    }

    fd = open(argv[1], O_RDONLY);
    
    if (fd < 0) {

        perror("Error opening descriptors file:");
        exit(-1);

    }

    initTaskDescriptorList(&list);

    parseDescriptors(&list, fd);

    runOS(&list);

    freeDescriptors(&list);

    close(fd);

}
