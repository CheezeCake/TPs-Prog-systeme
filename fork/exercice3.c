#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

char **prog;
int n;

void run(int sig)
{
	pid_t child;
	child = fork();

	if(child == -1)
	{
		perror("fork");
		exit(1);
	}

	if(child == 0)
		execvp(prog[0], prog);

	alarm(n);
}

int main(int argc, char **argv)
{
	signal(SIGALRM, run);

	if(argc < 3)
	{
		fprintf(stderr, "usage: timeout n prog");
		return 1;
	}

	prog = argv+2; /* prog[0] = argv[2] */
	n = atoi(argv[1]);

	raise(SIGALRM);

	while(1)
		pause();

	return 0;
}
