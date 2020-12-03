
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 53490 /* the server's port number */

#define MAXMSG 128 /* maximum message size */

/**
   Takes the hostname of the server as the only argument, emits the
   message from the server to stdout.
*/
int main (int argc, char *argv[])
{
  int sockfd, msgsize;
  int s;
  struct addrinfo *result;
  char buf[MAXMSG];
  char *hostname;
  struct sockaddr_in server;
  struct hostent *hp;

  if (argc != 2) {
    fprintf(stderr, "Usage: demo_client hostname\n");
    exit(1);
  }
  hostname = argv[1];
  if ((hp = gethostbyname (hostname)) == 0) {
    perror("Can't resolve name");
    exit(1);
  }

  // highly system dependent!
  memcpy (&server.sin_addr, hp->h_addr, hp->h_length);
  server.sin_port = htons (PORT);
  server.sin_family = AF_INET;


  /* Create the socket. We won't bind it, so when we try to connect
     the OS binds it to a random port. */
  if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
    perror("Can't create socket");
    exit(1);
  }

  if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
    perror("Failed to connect");
    exit(1);
  }

  /* OK, we have a connection. Now solicit input from the user and
     send the result to the server. */
  printf("Server request: ");
  if (fgets(buf, MAXMSG, stdin) != NULL) 
    write(sockfd, buf, strlen(buf));
  

  /* Read from the server and display the output */
  if ((msgsize = read(sockfd, buf, MAXMSG - 1)) == -1) {
    perror("Socket read failure");
    exit(1);
  }
  buf[msgsize] = '\0';
  printf("Server sez %s\n",buf);

  close(sockfd);
  return 0;
}
