#include <stdio.h>
#include<stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
void error(char * str, char*filename)
{ 
	printf(str,filename);
	exit(-1);
}
int main(void)
{	
	int f1,f2,n;
	char buf, file1[]="in.txt",file2[]="out.txt";
	char *pbuf=&buf;
	if ((f1 = open(file1, O_RDONLY)) == -1)
		error("Error open file %s", file1);
	if ((f2 = open(file2, O_CREAT|O_WRONLY|O_TRUNC,0644)) == -1)
		error("Error open file %s", file2);
	while ((n = read(f1,pbuf, 1)) > 0)
 		if (write(f2,pbuf,n ) != n)
 			error("error write in file %s",file2);
 	if (n<0) 
		error("error read from file %s",file1);
 	close(f1);
	close(f2);
	return 0;
}

