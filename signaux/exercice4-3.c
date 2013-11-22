#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

pid_t p1;
pid_t p2;

void msg(int sig)
{
	sleep(1);
	if(sig == SIGUSR1)
		while(kill(p2, SIGUSR2) == -1);
	else
		while(kill(p1, SIGUSR1) == -1);
}

void quit(int sig)
{
	kill(p1, SIGKILL);
	wait(NULL);
	kill(p2, SIGKILL);
	wait(NULL);
	exit(0);
}

int main(int argc, char **argv)
{
	char pid[10];

	signal(SIGUSR1, msg);
	signal(SIGUSR2, msg);
	signal(SIGINT, quit);
	signal(SIGALRM, quit);

	if(argc != 3)
	{
		fprintf(stderr, "mauvais nombre d'arguments\n"
				"exercice4-3 prog1 prog2\n");
		return 1;
	}

	sprintf(pid, "%d", getpid());

	switch((p1 = fork()))
	{
		case -1:
			perror("fork");
			exit(1);
			break;
		case 0:
			execl(argv[1], argv[1], pid, NULL);
			fprintf(stderr, "Erreur exec %s: %s\n", argv[1], strerror(errno));
			exit(1);
			break;
	}

	switch((p2 = fork()))
	{
		case -1:
			perror("fork");
			kill(p1, SIGKILL);
			exit(1);
			break;
		case 0:
			execl(argv[2], argv[2], pid, NULL);
			fprintf(stderr, "Erreur exec %s: %s\n", argv[2], strerror(errno));
			kill(p1, SIGKILL);
			exit(1);
	}

	while(1)
		pause();

	return 0;
}
