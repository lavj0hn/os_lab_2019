#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  const size_t kSize = sizeof(struct sockaddr_in);

  int lfd, cfd;
  int nread;
  struct sockaddr_in servaddr;
  struct sockaddr_in cliaddr;

  if (argc != 3) {
    printf("Usage: %s <port> <bufsize>\n", argv[0]);
    exit(1);
  }

  int serv_port = atoi(argv[1]);
  int bufsize = atoi(argv[2]);

  if ((lfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(1);
  }

  memset(&servaddr, 0, kSize);
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(serv_port);

  if (bind(lfd, (struct sockaddr *)&servaddr, kSize) < 0) {
    perror("bind");
    exit(1);
  }

  if (listen(lfd, 5) < 0) {
    perror("listen");
    exit(1);
  }

  while (1) {
    unsigned int clilen = kSize;

    if ((cfd = accept(lfd, (struct sockaddr *)&cliaddr, &clilen)) < 0) {
      perror("accept");
      exit(1);
    }
    printf("connection established\n");

    char buffer[bufsize]; 

    while ((nread = read(cfd, buffer, bufsize)) > 0) {
      write(1, buffer, nread); 
    }

    if (nread == -1) {
      perror("read");
      exit(1);
    }
    close(cfd);
  }
}