#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include "utils.h"

pid_t ordo;
pid_t pid;

void suppr(int sig)
{
	write_pid("pid.dat", pid);
	kill(ordo, SIGUSR2);
	printf("Envoi à l'ordo de %d\n", pid);
	kill(pid, SIGKILL);
}

int main(int argc, char **argv)
{
	int ret;
	signal(SIGINT, suppr);

	ordo = read_pid("ordo.dat");
	printf("pid ordo = %d\n", ordo);

	if(argc == 1)
	{
		fprintf(stderr, "pas de programme specifié\n"
				"usage: ./lanceur prog [prog args]\n");
		exit(0);
	}

	if((pid = fork()) == -1)
	{
		perror("erreur fork");
		exit(1);
	}

	if(pid == 0)
	{
		ret = execvp(argv[1], argv+1);
		if(ret == -1)
			exit(2);
	}
	else
	{
		write_pid("pid.dat", pid);
		printf("Envoi à l'ordo de %d\n", pid);
		kill(ordo, SIGUSR1);

		wait(NULL);
		suppr(0);
	}

	return 0;
}
