#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char **argv)
{	
	int status, pid, n;
	int buffer[1];	
	if (argc != 4){ printf("Uso: anillo <n> <c> <s> \n"); exit(0);}

    printf("Se crearán %i procesos, se enviará el caracter %i desde proceso %i \n", n, buffer[0], status);
	int N = atoi(argv[1]);
	int mensaje[1]; mensaje[1] = atoi(argv[2]);
	int inicial = atoi(argv[3]);
    if (atoi(argv[1]) < 3) exit(-1); 
	//creamos un array de pipes
	int **pipes = malloc(N*sizeof(int*));
	for	(int i = 0; i < N; i++) {
		pipes[i] = malloc(2*sizeof(int));
		int q = pipe(pipes[i]);
		if (q == -1) {
			perror("pipe"); 
			exit(-1);
		}
	}

	//creamos un pipe para conectar al pipe padre con el inicial 
	int pipe_inicial_padre = pipe(pipes[2]);
	if (pipe_inicial_padre == -1) {
		perror("pipe");
		exit(-1);
	}
	
	//el padre le escribe al inicial el mensaje 
	if (write(pipe_inicial_padre, &mensaje,sizeof(int) != sizeof(int))) {
		perror("write");
		exit(-1);
	}

	int secret_number; 
	int *pids = malloc(N*sizeof(int));

	for(int i = inicial; i < N + inicial % N; (i+1) % N) {
		pid_t pid = fork(); 
		
		if (pid == 0) {
			//si es la primera vez, el hijo manda un mensaje y si es el inicial, genera un secret_number
			
			if (i == inicial) {
				secret_number = random() % 50; 
				int mensaje; 
				if (read(pipe_inicial_padre,&mensaje, sizeof(int) != sizeof(int))) {
					perror("pipe"); 
					exit(-1);
				}

				if (write(pipes[i][1], &mensaje, sizeof(int)) != sizeof(int)) {
					perror("write");
					exit(-1);
				}
			}

			while(1) {
				//leo el número actual 
				int numero_actual;
				if (read(pipes[i-1%N][0], &numero_actual, sizeof(int)) != sizeof(int)) {
					perror("read");
					exit(-1);
				}

				if (i == inicial && numero_actual >= secret_number) break;

				//le envío a través del pipe al siguiente hijo  
				numero_actual ++;
				if (write(pipes[i][1], &numero_actual, sizeof(int)) != sizeof(int)) {
					perror("write");
					exit(-1);
				}

			}
		}
	}

	for(int i = 0; i < 2*N; i++) {
		close(pipes[i][0]);
		close(pipes[i][1]);
		wait(NULL);
	}    
}
