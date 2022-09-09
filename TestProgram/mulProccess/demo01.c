#include <stdio.h>
#include <unistd.h>
int main () 
{
	printf ("The process id is %d\n", (int) getpid ());
	printf ("The parent process id is %d\n", (int) getppid ()); 
	sleep(5);
	return 0;
}
