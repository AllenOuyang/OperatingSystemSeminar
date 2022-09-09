#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
void err_sys(const char* info)
{
       	perror(info);
	exit(1); 
}
int main(int argc, char* argv[])
{
       	if (argc != 5)
       	{
		printf("Format: commandname prog1 prog2 prog3 logfile\n");
		return 1; 
	}
	pid_t pid; // используется для хранения идентификаторов дочерних процессов
       	int status; // используется в функции waitpid
       	int fd[2], fdout; // дескрипторы для неименованного канала и результирующего файла
      	// создание неименованного канала
	pipe(fd);
     	// открытие (или создание) результирующего файла argv[4]
	fdout = open(argv[4], O_WRONLY |O_CREAT | O_APPEND, 0644);
        // создание первого дочернего процесса
	if((pid = fork()) < 0)
		err_sys("fork error");
	else if (pid == 0) // код первого дочернего процесса
	{
         	close(fd[0]);
         	dup2(fd[1], STDOUT_FILENO);
         	close(fd[1]);
                   // запуск команды argv[1]
		execlp(argv[1], argv[1], (char*)0);
         	printf("cannot execute: %s\n", argv[1]);
		return 2; 
	}
       // ожидание завершения первого дочернего процесса
       	if(waitpid(pid, &status, 0) < 0)
		err_sys("waitpid error");
       // создание второго дочернего процесса
	if((pid = fork()) < 0)
       		err_sys("fork error");	
	else if(pid == 0)
       	{
		close(fd[0]);
		// настройка файлового дескриптора с помощью функции dup2 close(fd[1]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
    		 // запуск команды argv[2] без параметров
		execlp(argv[2], argv[2], (char*)0);
         	printf("cannot execute: %s\n", argv[2]);
         	return 2;
	}
       	// ожидание завершения второго дочернего процесса
	if(waitpid(pid, &status, 0) < 0)
		err_sys("waitpid error");
       	close(fd[1]);
       	// создание третьего дочернего процесса
	if((pid = fork()) < 0)
		err_sys("fork error");
	else if (pid == 0)
       	{
		// настройка файловых дескрипторов с помощью функций dup2 close(fdout);
		dup2(fd[0], STDIN_FILENO);
		dup2(fdout, STDOUT_FILENO);
		close(fdout);
		close(fd[0]);
     		// запуск команды argv[3] без параметров
		execlp(argv[3], argv[3], (char*)0);
        	printf("cannot execute: %s\n", argv[3]);
        	return 2;
       	}
       	close(fd[0]);
       	close(fdout);
       	// ожидание завершения третьего дочернего процесса
	if(waitpid(pid, &status, 0) < 0)
		err_sys("fork error");
       	printf("\nOK\n");
       	return 0;
}
