#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "shell.h"

void exec_command(Command *cmd)
{
	pid_t child;
	int **p = NULL;
	int i, j;
	int ret;
	int fd;

	/* création des pipes */
	p = calloc(sizeof(int*), cmd->nb_members-1);
	for(i = 0; i < cmd->nb_members-1; i++)
	{
		p[i] = calloc(sizeof(int), 2);
		pipe(p[i]);
	}

	for(i = 0; i < cmd->nb_members; i++)
	{
		if((child = fork()) == -1)
		{
			perror("erreur fork()");
			exit(1);
		}

		if(child == 0)
		{
			/* fermer pipes inutiles */
			for(j = 0; j < cmd->nb_members-1; j++)
			{
				if(i != j && j != i-1)
				{
					close(p[j][0]);
					close(p[j][1]);
				}
			}

			/* rediriger stdin vers pipe[0] de la commande précédente */
			if(i > 0)
			{
				close(STDIN_FILENO);
				dup(p[i-1][0]);

				close(p[i-1][0]);
				close(p[i-1][1]);
			}

			/* rediriger stdout vers pipe[1] actuel */
			if(i < cmd->nb_members-1)
			{
				close(STDOUT_FILENO);
				dup(p[i][1]);

				close(p[i][0]);
				close(p[i][1]);
			}

			/*
			 * redirection depuis/vers fichiers
			 */

			/* STDIN */
			if(cmd->redirect[i][STDIN] != NULL)
			{
				fd = open(cmd->redirect[i][STDIN], O_RDONLY);
				if(fd == -1)
				{
					fprintf(stderr, "impossible d'ouvrir %s: %s\n",
							cmd->redirect[i][STDIN], strerror(errno));
					exit(1);
				}

				dup2(fd, STDIN_FILENO);
				close(fd);
			}

			/* STDOUT */
			if(cmd->redirect[i][STDOUT] != NULL)
			{
				if(cmd->type_redirect[i][STDOUT] == APPEND)
					fd = open(cmd->redirect[i][STDOUT],
							O_CREAT | O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR);
				else
					fd = open(cmd->redirect[i][STDOUT],
							O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);

				if(fd == -1)
				{
					fprintf(stderr, "impossible d'ouvrir %s: %s\n",
							cmd->redirect[i][STDOUT], strerror(errno));
					exit(1);
				}

				dup2(fd, STDOUT_FILENO);
				close(fd);
			}

			/* STDERR */
			if(cmd->redirect[i][STDERR] != NULL)
			{
				if(cmd->type_redirect[i][STDERR] == APPEND)
					fd = open(cmd->redirect[i][STDERR],
							O_CREAT | O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR);
				else
					fd = open(cmd->redirect[i][STDERR],
							O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);

				if(fd == -1)
				{
					fprintf(stderr, "impossible d'ouvrir %s: %s\n",
							cmd->redirect[i][STDERR], strerror(errno));
					exit(1);
				}

				dup2(fd, STDERR_FILENO);
				close(fd);
			}

			/*
			 * executer la commande
			 */
			ret = execvp(cmd->args[i][0], cmd->args[i]);

			if(ret == -1)
				fprintf(stderr, "Erreur execution: %s\n%s", cmd->args[i][0],
						strerror(errno));
			exit(errno);
		}
	}

	/* fermer tous les tubes */
	for(i = 0; i < cmd->nb_members-1; i++)
	{
		close(p[i][0]);
		close(p[i][1]);
		free(p[i]);
	}
	free(p);

	/* wait */
	for(i = 0; i < cmd->nb_members; i++)
		wait(NULL);
}

