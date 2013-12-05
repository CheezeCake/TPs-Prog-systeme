#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include "utils.h"

void write_pid(const char*, pid_t);

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

void write_pid(const char *path, pid_t pid)
{
	pid_t child;
	int fd;
	int ret;

	if((child = fork()) == 0)
	{
		fd = open(path, O_WRONLY);
		if(fd == -1)
		{
			fprintf(stderr, "erreur ecriture du pid: %d dans %s\n", pid, path);
			exit(1);
		}

		/* attendre tant que le fichier est locké */
		while((ret = lockf(fd, F_TEST, sizeof(pid_t))) == -1 && errno == EAGAIN)
			sleep(1);
		
		/* lock, write, unlock */
		lockf(fd, F_LOCK, sizeof(pid_t));
		write(fd, &pid, sizeof(pid_t));
		lseek(fd, 0, SEEK_SET);
		lockf(fd, F_ULOCK, sizeof(pid_t));

		exit(0);
	}
}

