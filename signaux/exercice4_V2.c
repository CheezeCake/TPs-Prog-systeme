#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

pid_t p1;
pid_t p2;

void message(int sig)
{
	pid_t p = getpid();
	printf("Ceci est un bon exercice pour comprendre le mécanisme des signaux (%d)\n", p);
	kill(getppid(), (p == p1) ? SIGUSR1 : SIGUSR2);
}	

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

void run()
{
	pid_t p = getpid();
	if(p == p1)
	{
		signal(SIGUSR1, message);
		raise(SIGUSR1);
	}
	else
		signal(SIGUSR2, message);
	

	while(1)
		pause();
}

int main(int argc, char **argv)
{
	signal(SIGUSR1, msg);
	signal(SIGUSR2, msg);
	signal(SIGINT, quit);
	signal(SIGALRM, quit);

	switch((p1 = fork()))
	{
		case -1:
			perror("fork");
			exit(1);
			break;
		case 0:
			p1 = getpid();
			run();
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
			p2 = getpid();
			run();
			break;
	}

	while(1)
		pause();

	return 0;
}
