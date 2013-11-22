#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include "utils.h"

int main()
{
	pid_t ordo;
	pid_t pid;
	char c;

	ordo = read_pid("ordo.dat");
	printf("pid ordo = %d\n", ordo);

	while(1)
	{
		c = '\0';
		pid = -1;

		do
		{
			printf("ajouter (a), supprimer (s): ");
			scanf("%c", &c);
		} while(c != 'a' && c != 's');

		do
		{
			printf("pid: ");
			scanf("%d", &pid);
		} while(pid < 0);

		write_pid("pid.dat", pid);
		if(c == 'a')
		{
			printf("Envoi à l'ordo de %d\n", pid);
			kill(ordo, SIGUSR1);
		}
		else
		{
			printf("Fin de %d, envoi de la notification à l'ordo\n", pid);
			kill(ordo, SIGUSR2);
		}

		getchar();
	}

	return 0;
}
