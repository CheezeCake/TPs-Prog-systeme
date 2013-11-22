#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

pid_t pid;

void killer(int sig)
{
	/*
	printf("Envoi de SIGKILL au processus %d\n", pid);
	*/
	kill(pid, SIGKILL);
	exit(0);
}

int main(int argc, char **argv)
{
	int t;

	if(argc < 3)
	{
		fprintf(stderr, "Pas assez d'arguments\n");
		return 1;
	}

	signal(SIGALRM, killer);

	t = atoi(argv[1]);
	pid = atoi(argv[2]);

	alarm(t);

	while(1)
		pause();

	return 0;
}
