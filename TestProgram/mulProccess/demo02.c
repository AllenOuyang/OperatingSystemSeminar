#include <stdio.h>
     #include <unistd.h>
     #include <sys/types.h>
     //для pid_t
     #include <stdlib.h>
     void err_sys(const char* info)
     {
      perror(info);
exit(1); }
      int globvar = 8; /* глобальная переменная в сегменте данных
*/
int main() {
     int var; /* переменная, размещаемая на стеке */
      pid_t pid;
      var = 88;
     if ((pid = fork()) < 0)
      err_sys("fork error");
      else if (pid == 0)
     { /* дочерний процесс */
       printf("child' process \n");
       globvar++; /* изменить переменные */
       var++;
} else
       sleep(2); /* родительский процесс */
       printf("pid = %d, ppid = %d, globvar = %d, var = %d\n",
       getpid(), getppid(), globvar, var);
return 0; }
