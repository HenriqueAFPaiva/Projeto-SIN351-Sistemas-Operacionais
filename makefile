meuShell: 	meuShell.o
			gcc meuShell.o -o meuShell

meuShell.o: meuShell.c
			gcc -c -g -Wall meuShell.c

clean:      
			rm meuShell.o meuShell
