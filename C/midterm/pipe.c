// -----------------------------------------------------------------------------
// Pipes
//
// Write a program that creates a child process and pipes the string
// "hello child" to the child to be printed. The parent should wait for the
// child to terminate
//
// Solution
//
// Expected output:
// Parent Sending: hello child
// Child Received: hello child
// -----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void) {
  // Write to fds[1] and read from fds[0]
  int fds[2];

  // Open pipe
  int pipe_result = pipe(fds);
  if (pipe_result < 0) {
    fprintf(stderr, "ERROR: Failed to open pipe\n");
    exit(1);
  }

  char string_to_send[] = "hello child";
  int bytes_to_send_recv = strlen(string_to_send) + 1;

  // Create child process
  pid_t pid = fork();
  if (pid < 0) {
    // Error
    fprintf(stderr, "ERROR: Failed to fork\n");
  }
  else if (pid > 0) {
    // Parent
    printf("Parent Sending: %s\n", string_to_send);

    // Parent doesn't need read file descriptor
    close(fds[0]);

    // Write the string to the pipe
    // NOTE: A single write() call will work if we are not worried about errors
    int total_bytes_written = 0;
    while (total_bytes_written < bytes_to_send_recv) {
      int bytes_remaining = bytes_to_send_recv - total_bytes_written;
      int bytes_written = write(fds[1], &string_to_send[total_bytes_written], bytes_remaining);
      if (bytes_written < 0) {
        fprintf(stderr, "ERROR: Failed to write\n");
      }

      total_bytes_written += bytes_written;
    }

    // Done writing
    close(fds[1]);

    // Wait for child to terminate
    wait(NULL);
  }
  else {
    // Child

    char *recv_buffer = malloc(bytes_to_send_recv);

    // Child doesn't need write file descriptor
    close(fds[1]);

    // Read the string from the pipe
    // NOTE: A single read() call will work if we are not worried about errors
    int total_bytes_read = 0;
    while (total_bytes_read < bytes_to_send_recv) {
      int bytes_remaining = bytes_to_send_recv - total_bytes_read;
      int bytes_read = read(fds[0], recv_buffer, bytes_remaining);
      if (bytes_read < 0) {
        fprintf(stderr, "ERROR: Failed to read\n");
      }

      total_bytes_read += bytes_read;
    }

    // Done reading
    close(fds[0]);

    // Print result
    printf("Child Received: %s\n", recv_buffer);
    free(recv_buffer);
  }

  return 0;
}
