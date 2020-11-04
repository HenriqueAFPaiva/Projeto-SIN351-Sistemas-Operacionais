# Projeto-SIN351

**Grupo:**
  - José Eduardo - 5964 - T2
  - Henrique Alves - 5968 - T2

  
**Link repositório:** https://github.com/HenriqueAFPaiva/Projeto-SIN351

**Objetivo:**
  Desenvolver um interpretador de comandos (shell) em linguagem C para a disciplina de **Sistemas Operacionais - SIN351**
  
**Características do Shell**
  - A forma de concatenar os comandos é feita através do caractere ",";
  - Para sair do shell basta digitar o comando "quit";
  
**Erros conhecidos**
  - Quando se usa pipes(",") sem comandos entre eles, o Shell imprime o erro porém continua a executar os outros comandos. (Ex: cat file ,,,, ls -l; O Shell executa o comando ls -l após imprimir os erros);
  
**Compilar o projeto:**

```bash
make meuShell
```

**Executar:**

```bash
./meuShell
```
