#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void err_sys(const char *info)
{
      	perror(info);
	exit(1);
}
int main(int argc, char **argv)
{
      	if (argc != 2)
      	{
        	printf("Usage: commandname <filename>\n");

		return 1;
      	}
      	int f1 = open(argv[1], O_RDONLY);// открытие исходного файла
      	if (f1 == -1)
        	err_sys(argv[1]);
      	struct stat statbuf;
      	if (stat(argv[1], &statbuf) != 0)
        	err_sys(argv[1]);
      	char *tmpfilename = "tmpfile.tmp";
      	int f2 = open(tmpfilename, O_WRONLY | O_CREAT | O_TRUNC, statbuf.st_mode);// создание вспомогательного файла,
         // для задания режима доступа используется
         // структура statbuf
      	if (f2 == -1)
       		err_sys(tmpfilename);
	char c; // переменная для хранения прочитанных символов
	int k; // переменная для хранения количества прочитанных символов
       	// обработка исходного файла (посимвольное чтение
	// данных из файла f1 и запись требуемых символов во вспомогательный файл f2)
       	while((k = read(f1, &c, 1)) > 0) // пока число прочитанных символов k > 0
       	{
		if( '0' <= c && c <= '9')
			continue;
		write(f2, &c, 1);
	}
       	if (k < 0)
		err_sys("read");
       // закрытие файлов
       	close(f1);
	close(f2);
       // переименование вспомогательного файла
       	rename(tmpfilename, argv[1]);
       	printf("\nOK\n");
	return 0; 
}

