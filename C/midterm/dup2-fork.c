/*
* Recitation Section Number:
* Breakout Number:
* Member Name
* Member Name
* Member Name
* Member Name
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[]) {

	// create/open a file named write.txt
	int fd = open("write.txt", O_RDWR| O_TRUNC | O_CREAT, 0777);

	// write hello world to write.txt
	int w1 = write(fd, "hello world\n", 12);

	// spawn a child process and use this child process to reroute stdout to write.txt
    int stdoutCopy = dup(STDOUT_FILENO);
	pid_t child = fork();
	if(child == 0){
		dup2(fd, STDOUT_FILENO);
	}

	// print pid
	printf("Middle: Process %d\n", getpid());


	// use the child process spawned previously to read the contents from write.txt, restore output to stdout
	// and then prints the contents of write.txt to stdout.
	if(child == 0){
		// read from write.txt
		fflush(stdout);
		close(fd);
		int fd_2 = open("write.txt", O_RDONLY);
		char read_text[40];
		int r1 = read(fd_2, read_text, 40);
		if (r1 < 40)
			read_text[r1]= '\0';
		close(fd_2);

		// restore stdout
		dup2(stdoutCopy, STDOUT_FILENO);
		printf("Process %d Read text from file: \n%s", getpid(), read_text);
	}
	else
		wait(NULL);

	// print pid
	printf("End: Process %d\n", getpid());

	// close the file write.txt
	close(fd);

	return 0;
}
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <errno.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include <string.h>
// int main(int argc, char *argv[]) {
//     // create/open a file named write.txt
//     int file;
//     file = open("write.txt", O_WRONLY| O_TRUNC | O_CREAT);
//     int filecopy = dup(1);
//     // write hello world to write.txt
//     write(file, "hello world\n", 13);
//     close(file);
//     // spawn a child process
//     pid_t child = fork();
//     if (child == 0) {
//         dup2(file, 1);
//     // reroute stdout to write.txt
//     }
//     // print pid
//     printf("Middle: Process %d\n", getpid());
//     // use the child process spawned previously
//     if (child == 0) {
//         // read the contents from write.txt
//                 char buffer[50];
//                 file = open("write.txt", O_RDONLY);
//                 read(file, buffer, 50);
//         // restore output to stdout// restore output to stdout
//         dup2(filecopy, 1);
//         // prints the contents of write.txt to stdout
//         printf("Process %d Read text from file: \n", getpid());
//         printf("%s", buffer);
//     } else if (child > 0) {
//         wait(NULL);
//     }
//     // print pid
//     printf("End: Process %d\n", getpid());
//     // close the file write.txt
//     close(file);
// return 0;
}
