#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

void infintePrint() {

	static int count = 0;
	printf("%ld: Count = %d\n", time(NULL), ++count);
	sleep(1);
}

int main() {

	sigset_t newsigset, oldsigset;

	/* Initialize a new signal set. */
	sigemptyset(&newsigset);

	/* Add SIGINT to the signal set. */
	sigaddset(&newsigset, SIGINT);

	/* Add SIGINT to the signal mask. */
	sigprocmask(SIG_BLOCK, &newsigset, &oldsigset);

	/* Print infinitely. */
	while (1) infintePrint();
}
