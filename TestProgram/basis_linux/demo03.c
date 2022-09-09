#include <stdio.h>
#include<stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
void err_sys(const char * str) 
{ 
	fprintf(stderr,"%s\n",str); 
	exit(1); 
}
/*老师自定义的读取一行的函数,效率低的*/
ssize_t readline01(int fd, void* sbuf, size_t sbufsize)
{
	ssize_t n, rc;
	char c, *ptr;
	ptr = sbuf;
	for (n = 1; n < sbufsize; n++)
	{
		if ((rc = read(fd, &c, 1)) == 1)
		{
			*ptr++ = c;
			if (c == '\n')
			{
				n++;
				break;
			}
		}		
		else if (rc == 0)
		{		
			*ptr = 0;
			return n - 1;
		}
		else
			return -1;
	}	
	*ptr = 0;
	return n - 1;
}
#define BUFFSIZE 4096
static int read_cnt;
static char* read_ptr;
static char read_buf[BUFFSIZE];
static ssize_t my_read(int fd, char* ptr)
{
	if (read_cnt <= 0)
	{
		if ((read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0)
			return -1;
		else if (read_cnt == 0)
			return 0;
		read_ptr = read_buf;
	}	
	read_cnt--;
	*ptr = *read_ptr++;
	return 1;
}
ssize_t readline(int fd, void* sbuf, size_t sbufsize)
{
	ssize_t n, rc;
	char c, * ptr;
	ptr = sbuf;
	for (n = 1; n < sbufsize; n++)
	{
		if ((rc = my_read(fd, &c)) == 1)
		{
			if (c == '\r')
			{
				n--;
				continue;
			}
			*ptr++ = c;
			if (c == '\n')
			{
				n++;
				break;
			}
		}
		else if (rc == 0)
		{
			*ptr = 0;
			return n - 1;
		}
		else
			return -1;
	}
	*ptr = 0;
	return n - 1;
}
ssize_t readlinebuf(void** ppbuf)
{
	if (read_cnt)
	*ppbuf = read_ptr;
	return read_cnt;
}

int main(void)
{
	int fd = open("test.txt", O_RDONLY);
	if (fd == -1)
		err_sys("open");
	ssize_t n;
	char str[6];
	while ((n = readline(fd, str, sizeof(str))) > 0)
		printf("%ld : %s\n", n, str);
	if (n == -1)
 		err_sys("readline");
	close(fd);
	return 0;
}

