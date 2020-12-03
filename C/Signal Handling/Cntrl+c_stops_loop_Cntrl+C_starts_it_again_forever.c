#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

struct sigaction act;
static volatile sig_atomic_t run = 1;

/* Stop the printing until the next interrupt. */
void handler(int signo) {

	run = !run;
}

void infinitePrint() {

	static int count = 0;
	printf("%ld: Count = %d\n", time(NULL), ++count);
	sleep(1);
}

int main() {

	/* Set up the signal handler. */
	act.sa_handler=handler;
	act.sa_flags=0;
	sigemptyset(&act.sa_mask);

	/* specify action associated with SIGNT. */
	sigaction(SIGINT, &act, NULL);

	/* Print infinitely. */
	while (1) {

		while (run) infinitePrint();
		while (!run) {}
	}
}
