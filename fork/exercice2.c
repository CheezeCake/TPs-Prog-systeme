#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

pid_t child;

void kill_child(int sig)
{
	kill(child, SIGKILL);
}

int main(int argc, char **argv)
{
	int n;

	signal(SIGALRM, kill_child);

	if(argc < 3)
	{
		fprintf(stderr, "usage: timeout n prog");
		return 1;
	}

	n = atoi(argv[1]);

	child = fork();
	if(child == -1)
	{
		perror("fork");
		return 2;
	}

	if(child == 0)
		execvp(argv[2], argv+2);
	else
	{
		alarm(n);
		pause();
	}

	return 0;
}
