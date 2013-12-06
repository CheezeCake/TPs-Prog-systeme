#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

pid_t sync_p;
pid_t p1, p2;
pid_t p;

void message(int sig)
{
	printf("ceci est un bon exercice pour comprendre le mécanisme des signaux (%d)\n", getpid());
	kill(sync_p, SIGUSR1);
}

void send(int sig)
{
	sleep(1);

	p = (p == p1) ? p2 : p1;
	kill(p, SIGUSR1);
}

void cleanup(int sig)
{
	kill(p1, SIGKILL);
	kill(p2, SIGKILL);
	exit(0);
}

int main()
{
	sync_p = getpid();

	if((p1 = fork()) == -1)
	{
		perror("fork 1");
		return 1;
	}

	if(p1 == 0)
	{
		signal(SIGUSR1, message);
		while(1)
			pause();
	}

	if((p2 = fork()) == -1)
	{
		perror("fork 2");
		kill(p1, SIGKILL);
		return 2;
	}

	if(p2 == 0)
	{
		signal(SIGUSR1, message);
		while(1)
			pause();
	}

	p = p2;
	signal(SIGINT, cleanup);
	signal(SIGUSR1, send);
	raise(SIGUSR1);

	while(1)
		pause();

	return 0;
}