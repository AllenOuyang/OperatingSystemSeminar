#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
/* Запуск дочернего процесса в виде новой программы
PROGRAM имя вызываемой программы , поиск в каталогах
определяемой переменной PATH.
   ARG_LIST - список строковых переменных оканчивающихся на NULL
   Функция возвращает идентификатор порожденного процесса.  */
int spawn (char* program, char** arg_list)
{
  pid_t child_pid;
  /* Вилка - копия процесса.  */
  child_pid = fork ();
  if (child_pid != 0)
    /* Родительский  процесс.  */
    return child_pid;
  else {
/* Выполнение указанной программы, поиск в переменной path. */
    execvp (program, arg_list);
    /* execvp - возвращает значение, если происходит ошибка.  */
    fprintf (stderr, "an error occurred in execvp\n");
    abort ();
} }
int main () {
  /* Список аргументов передаваемых execvp "ls"   */
  char* arg_list[] = {
    "ls",     /* argv[0], имя программы.  */
"-l",
    "/",
    NULL      /* список заканчивается на NULL.  */
};
  /* порождаем дочерний процесс "ls" command.   */
  spawn ("ls", arg_list);
  printf ("done with main program\n");
return 0; }

