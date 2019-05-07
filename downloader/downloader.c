#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

#define TARGET_URL "http://212.128.69.216/lolo"
#define REMOTE_TARGET_SIZE_IN_BYTES 1047491658L
#define CHUNK_FILENAME_PREFIX "download"

void download_fragment(char* url, long from, long to, char* outfile);
int are_arguments_correct(int argc, char* argv[]);

/**
 ** Main function **
 */

int main(int argc, char* argv[]) {
    	int chunkTamano;
    	int i;
    	int from, to;
    	int pid;
    	char outfile[1];

    	if (!are_arguments_correct(argc, argv)) {
        	return -1;
    	}
    

    	int nProcesos = atoi(argv[1]);
    	char tipoDescarga = argv[2][0];
    	/**
     	* Calculate the chunk sizes and inform the user
     	*/

    	chunkTamano = REMOTE_TARGET_SIZE_IN_BYTES/nProcesos;
    	printf ("### Usando %d procesos para la descarga ###\n", nProcesos);

    	if ( REMOTE_TARGET_SIZE_IN_BYTES % nProcesos == 0) {
        	printf ("%d chunks de %d bytes\n", nProcesos, chunkTamano);
    	} else {
        	printf ("%d chunks de %d bytes\n", nProcesos-1, chunkTamano);
        	printf ("%d chunk de %ld bytes\n", 1, REMOTE_TARGET_SIZE_IN_BYTES-(chunkTamano*nProcesos)+chunkTamano);
    	}

    	printf ("Total de %ld bytes para descargar \n\n", REMOTE_TARGET_SIZE_IN_BYTES);

    	for (i=1; i <= nProcesos; i++) {
        	/**
        	 * For each chunk, compute the range where to start and where to stop
        	 */
        	if (nProcesos == 1) {
            		from = 0;
            		to = REMOTE_TARGET_SIZE_IN_BYTES;

        	} else if (i < nProcesos) {
        		from = chunkTamano*(i-1);
            		to = chunkTamano*i-1;

        	} else {
           		/**
             		* The last one gets the remainder chunk up to the end
             		*/
            		from = to + 1;
            		to = REMOTE_TARGET_SIZE_IN_BYTES;
        	}

		pid_t pid;
		pid = fork(); // Proceso hijo

     		if (pid == 0) {
                	printf("Número de downloader: %d: \n", i);
			printf("\t chunk %d: Rango de bytes %d-%d", i, from, to);
			sprintf(outfile, "%s-%d", CHUNK_FILENAME_PREFIX, i); //  Generate the name of the file for the current chunk, following the pattern download-1, download-2, download-i.
			download_fragment(TARGET_URL, from, to, outfile); // Llamada a la funcion download_fragment()
			exit(0);

 		}
    		if (tipoDescarga == 'S') {
			printf("* Esperando al hijo: %d para terminar *\n", i);
                	wait(NULL); // Esperando al/a los proceso/s hijo iniciado en fork()
            		
     		}

	} // TERMINA EL BUCLE PARA HACER LA DESCARGA SECUENCIAL

    	if (tipoDescarga == 'P') { // Descarga paralela
		for (i=0; i <= nProcesos; i++) {
			if (i>0){
				printf("Hijo: %d terminado\n", i);
			}
       			wait(NULL); // Esperando al/a los proceso/s hijo iniciado en fork()
      			

    		} // TERMINA EL BUCLE PARA HACER LA DESCARGA PARALELA
    	} 
   	printf ("\n-- Fin de la descarga --\n");

} // TERMINA FUNCION MAIN


/**
 * Example curl call:
 * curl -s -H "Range: bytes=2-3" https://localhost/testfile.txt -o filename
 */

void download_fragment (char* url, long from, long to, char* outfile){
    	char range[200];
	sprintf(range,"Range: bytes=%ld-%ld\n", from, to);
    	printf("\n	 Testing %s\n",range);
    	execlp("curl", "curl", "-s", "-H", range, url, "-o", outfile, NULL);
    	perror("Error");

}

int are_arguments_correct(int argc, char* argv[]) {

    	/**
     	* First, we have to check the number of arguments, and also, if the 
     	* download_mode arguments is a P (parallel) or a S (sequential)
     	*/

    	if (argc != 3 ) {  //En el comando tienen que haber tres entradas espaciadas

        	printf(	"error: invalid number of arguments\n"
               		"usage: %s processes {P/S} \n"
               		"\tprocesses: number of download processes to fork \n"
               		"\tdownload mode: (P) Parallel download (S) Sequential download\n", argv[0]);
        	return 0;

    	}	

    	char tipoDescarga = argv[2][0]; 
    	if (tipoDescarga != 'P' && tipoDescarga != 'S') {
        	printf("error: invalid download mode. It has to be P or S\n");
        return 0; // Si la entrada del argv[2][0] es distinto a los tipos establecidos de descarga, la función devuelve 0
    	}

    	int nProcesos = atoi(argv[1]);
    	if (nProcesos <= 0) {
        	printf("error: the number of processes has to be greater than 0\n");
        return 0; //Si el numero de procesos es inferior o igual a 0, el argumento de entrada no es válido, la función devuelve 0
    	}

    	return 1;
}
