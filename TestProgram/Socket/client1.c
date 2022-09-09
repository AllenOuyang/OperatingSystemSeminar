#include <sys/types.h>
      #include <sys/socket.h>
      #include <stdio.h>
      #include <sys/un.h>
      #include <unistd.h>
      #include <stdlib.h>
int main() {
          int sockfd;
          int len;
          struct sockaddr_un address;
          int result;
          char ch = 'A';
/* Создание socket а для клиента */
sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
      /*  имя сокета согласовано с именем сервера  */
address.sun_family = AF_UNIX; strcpy(address.sun_path, "server_socket"); len = sizeof(address);
/* соединение сокета клиента с сокетом сервера. */
result = connect(sockfd, (struct sockaddr *)&address, len);
          if(result == -1) {
              perror("oops: client1");
              exit(1);
}
/* теперь можно совершать операции read/write через sockfd. */
          write(sockfd, &ch, 1);
          read(sockfd, &ch, 1);
          printf("char from server = %c\n", ch);
          close(sockfd);
          exit(0);
}
