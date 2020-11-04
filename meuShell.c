#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

// Array para armazenar os dados do comando
static char* argumentos[512];
pid_t pid;

#define LER  0
#define ESCREVER 1

// separa os comandos por pipe
static int separa_comando(int entrada, int primeiro, int ultimo){
	int pipes[2];

	// Chama o pipe
	pipe( pipes );
	pid = fork();

	if (pid == 0) {
		if (primeiro == 1 && ultimo == 0 && entrada == 0) {
			// primeiro comando
			dup2( pipes[ESCREVER], STDOUT_FILENO );
		} else if (primeiro == 0 && ultimo == 0 && entrada != 0) {
			// segundo comando
			dup2(entrada, STDIN_FILENO);
			dup2(pipes[ESCREVER], STDOUT_FILENO);
		} else {
			// ultimo comando
			dup2( entrada, STDIN_FILENO );
		}

		if (execvp( argumentos[0], argumentos) == -1)
			_exit(EXIT_FAILURE);
	}

	if (entrada != 0)
		close(entrada);

	close(pipes[ESCREVER]);

	// se é o ultimo comando, nada mais precisa ser lido
	if (ultimo == 1)
		close(pipes[LER]);

	return pipes[LER];
}

static void limpar(int n){
	int i;
	for (i = 0; i < n; ++i)
		wait(NULL);
}

static char linha[1024];
static int n = 0; // numero de chamadas de comandos

//pula os espaços encontrados
static char* pular_espaco(char* s){
	while (isspace(*s)) ++s;
	return s;
}

//separa os comandos
static void separar(char* comando){
	comando = pular_espaco(comando);
	char* proximo = strchr(comando, ' ');
	int i = 0;

	while(proximo != NULL) {
		proximo[0] = '\0';
		argumentos[i] = comando;
		++i;
		comando = pular_espaco(proximo + 1);
		proximo = strchr(comando, ' ');
	}

	if (comando[0] != '\0') {
		argumentos[i] = comando;
		proximo = strchr(comando, '\n');
		proximo[0] = '\0';
		++i;
	}

	argumentos[i] = NULL;
}

static int executar(char* comando, int entrada, int primeiro, int ultimo){
	separar(comando);
	if (argumentos[0] != NULL) {
		if (strcmp(argumentos[0], "quit") == 0){
      exit(0);
    }

		n += 1;
		return separa_comando(entrada, primeiro, ultimo);
	} else {
    printf("meu-shell > ERRO: comando NULL encontrado\n");
  }
	return 0;
}

int main(){
	while (1) {
		// Imprime o meu-shell> na cor verde
    printf("\033[1;32m");
		printf("meu-shell > ");
    printf("\033[0m");
		fflush(NULL);

		// Ler o comando
		if (!fgets(linha, 1024, stdin))
			return 0;

		int entrada = 0;
		int primeiro = 1;

		char* comando = linha;
		char* proximo = strchr(comando, ','); // Acha a primeira ',' (pipe)

		while (proximo != NULL) {
			// 'proximo' aponta para a ',' (pipe)
			*proximo = '\0';
      // executa o comando após a ',' (pipe)
			entrada = executar(comando, entrada, primeiro, 0);

			comando = proximo + 1;
			proximo = strchr(comando, ','); // Acha a proxima ',' (pipe)
			primeiro = 0;
		}
    // executa o comando após a ',' (pipe)
		entrada = executar(comando, entrada, primeiro, 1);
		limpar(n);
		n = 0;
	}

	return 0;
}
