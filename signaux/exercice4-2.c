#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

pid_t sync_process;

void message(int sig)
{
	printf("Ceci est un bon exercice pour comprendre le mécanisme des signaux (%d)\n",
			getpid());
	kill(sync_process, SIGUSR2);
}

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		fprintf(stderr, "mauvais nombre d'arguments\n");
		return 1;
	}

	signal(SIGUSR2, message);

	sync_process = atoi(argv[1]);

	while(1)
		pause();

	return 0;
}
