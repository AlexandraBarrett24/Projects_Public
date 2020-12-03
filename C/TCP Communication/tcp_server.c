
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define MYPORT 53490    /* the port users will be connecting to */

#define BACKLOG 5     /* how many pending connections queue will hold */

#define MSGSIZE 128

/**
   Accepts a string on the command line, sends that string to any
   client that connects.
*/
int main (int argc, char *argv[])
{
  char *message = "Hello. I'm a happy server!";
  int sockfd, client_fd;  /* listen on sock_fd, client connection on
			     client_fd */
  struct sockaddr_in my_addr; /* my address */
  struct sockaddr_in client_addr; /* client's address */
  int addr_size;
  int enable = 1;

  /* --- Create the socket --- */
  if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
    perror("Can't create socket");
    exit(1);
  }

  /* --- Bind the socket, making its address reusable --- */
  my_addr.sin_family = AF_INET;
  my_addr.sin_port = htons(MYPORT);
  my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  /* Note the use of INADDR_ANY to get the OS to fill in the local
     host address. You may not want to do this on a multi-home
     host. */

  /* Set the "re-use" option so we can shut down and restart the
     server without waiting for the standard timeout. */
  if (setsockopt(sockfd,SOL_SOCKET, SO_REUSEADDR, 
      &enable, sizeof(int)) == -1) {
    perror("Can't set socket option");
    exit(1);
  }

  if (bind(sockfd, (struct sockaddr *)&my_addr,
	           sizeof(my_addr)) == -1) {
    perror("Could not bind");
    exit(1);
  }

  /* --- Set up the socket to listen for incoming connection requests --- */
  if (listen(sockfd, BACKLOG) == -1) {
    perror("Could not listen");
    exit(1);
  }

  /* --- Accept connections and serve our message --- */
  while (1) {
    addr_size = sizeof(client_addr);
    
    if ((client_fd = accept(sockfd,
			 (struct sockaddr *)&client_addr,
			 &addr_size)) == -1) {
      perror("Failed to accept connection");
      continue;
    }
    fprintf(stderr, "server: client connection from %s\n",
	   inet_ntoa(client_addr.sin_addr));

    char msg[MSGSIZE];
    int readsz = 0;

    if ((readsz = read(client_fd, msg, MSGSIZE-1)) >= 0) {
      msg[readsz] = '\0';
      fprintf(stderr, "Client sez %s", msg);
    }
    else perror ("server read problem");

    /* Next, we write our message and close the socket. */
    if (write(client_fd, message, strlen(message)) == -1) {
      perror("Failed to write to client");
    }
  }

  close(client_fd);
}



