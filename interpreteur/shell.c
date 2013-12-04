#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include "shell.h"

/*
 * TEST:
 * executer le 1er membre seulement
 */

void exec_command(Command *cmd)
{
	pid_t child;

	if((child = fork()) == -1)
	{
		perror("Erreur fork()");
		exit(1);
	}

	if(child == 0)
		execvp(cmd->args[0][0], cmd->args[0]);
	else
		wait(NULL);
}

