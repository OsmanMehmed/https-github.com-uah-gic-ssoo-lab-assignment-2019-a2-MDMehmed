#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

#define TARGET_URL  "http://212.128.69.216/lolo"
#define REMOTE_TARGET_SIZE_IN_BYTES 1047491658L
#define CHUNK_FILENAME_PREFIX "download"

void download_fragment(char* url, long from, long to, char* outfile);
int are_arguments_correct(int argc, char* argv[]);

/**
 ** Main function **
 */
int main(int argc, char* argv[]) {
    int chunk_size;
    int i;
    int from, to;
    int pid;

    if (!are_arguments_correct(argc, argv)) {
        return -1;
    }

    char download_mode=argv[2][0];
    int num_processes=atoi(argv[1]);

    /**
     * Calculate the chunk sizes and inform the user
     */
    chunk_size = REMOTE_TARGET_SIZE_IN_BYTES/num_processes;
    printf ("Using %d processes for download \n", num_processes);
    if ( REMOTE_TARGET_SIZE_IN_BYTES % num_processes == 0) {
        printf ("%d chunks of %d bytes\n", num_processes, chunk_size);
    } else {
        printf ("%d chunks of %d bytes\n", num_processes-1, chunk_size);
        printf ("%d chunks of %ld bytes\n", 1, REMOTE_TARGET_SIZE_IN_BYTES - (chunk_size*num_processes) + chunk_size);
    }
    printf ("Total %ld bytes to download \n", REMOTE_TARGET_SIZE_IN_BYTES);

    for (i=1; i <= num_processes; i++) {
        /**
         * For each chunk, compute the range where to start and where to stop
         */
        if (num_processes == 1) {
            from = 0;
            to = REMOTE_TARGET_SIZE_IN_BYTES;
        } else if (i < num_processes) {
            from = chunk_size*(i-1);
            to = chunk_size*i-1;
        } else {
            /**
             * The last one gets the remainder chunk up to the end
             */
            from = to + 1;
            to = REMOTE_TARGET_SIZE_IN_BYTES;
        }

        /**
         * TODO: Create a child process that will:
         *   - Print a message showing what part it will download (mostly for
         *   debugging purposes (i.e. printf("\t chunk #%d: Range %d-%d \n", i, from, to))
         *   - Generate the name of the file for the current chunk, following
         *   the pattern download-1, download-2, download-i. You can do this
         *   with something like:
         *       sprintf(outfile, "%s-%d", CHUNK_FILENAME_PREFIX, i);
         *   - Call download_fragment(TARGET_URL, from, to, outfile); 
         *   - exit(0);
         */


        if (download_mode == 'S') {
            /**
             * TODO: the father must wait until the child has finished
             * downloading the current chunk if the download mode is S
             * (sequential)
             */
        }
    }

    if (download_mode == 'P') {
        /**
         * TODO: wait until all the downloads have finished if the download mode
         * is P (parallel)
         */
    }
    printf ("-- End downloader --\n");
}

/**
 * Example curl call:
 * curl -s -H "Range: bytes=2-3" https://localhost/testfile.txt -o filename
 */
void download_fragment(char* url, long from, long to, char* outfile){
    char range[200];
    sprintf(range, "Range: bytes=%ld-%ld", from, to);
    printf("Testing %s\n",range);
    execlp("curl", "curl", "-s", "-H", range, url, "-o", outfile, NULL);
    perror("Error");
}

int are_arguments_correct(int argc, char* argv[]) {
    /**
     * First, we have to check the number of arguments, and also, if the 
     * download_mode arguments is a P (parallel) or a S (sequential)
     */
    if (argc != 3 ) {

        printf("error: invalid number of arguments\n"
               "usage: %s processes {P/S} \n"
               "\tprocesses: number of download processes to fork \n"
               "\tdownload mode: (P) Parallel download (S) Sequential download\n", argv[0]);
        return 0;

    }

    char download_mode=argv[2][0];
    if (download_mode != 'P' && download_mode != 'S') {
        printf("error: invalid download mode. It has to be P or S\n");
        return 0;
    }

    int num_processes=atoi(argv[1]);
    if (num_processes <=0) {
        printf("error: the number of processes has to be greater than 0\n");
        return 0;
    }

    return 1;
}
