#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "command.h"
#include "shell.h"

void setup_prompt(char*, const char*, const char*);
int cmd_exit(const char*, int*);
void delete_ending_newline(char*);
void sigchld_handler(int);

int main()
{
	char input[CMD_MAX_LENGTH];
	Command cmd;
	int exit_code = 0;
	char prompt[1024];
	char hostname[256];
	char cwd[256];
	char *fgetsret = NULL;

	signal(SIGCHLD, sigchld_handler);

	gethostname(hostname, 256);
	getcwd(cwd, 256);
	setup_prompt(prompt, hostname, cwd);

	while(1)
	{
		printf("%s", prompt);
		fgetsret = fgets(input, CMD_MAX_LENGTH, stdin);
		if(fgetsret == NULL)
			return 0;

		delete_ending_newline(input);

		if(strlen(input) != 0)
		{
			if(cmd_exit(input, &exit_code))
				return exit_code;

			init_command(&cmd);
			if(parse_members(input, &cmd) == 0 && parse_args(&cmd) == 0)
			{
				/*
				   aff_members(&cmd);
				   aff_args(&cmd);
				   aff_redirect(&cmd);

				   printf("----------\n");
				   */

				exec_command(&cmd);
			}

			destroy_command(&cmd);
		}

		*input = '\0';
	}

	return 0;
}

void setup_prompt(char *prompt, const char *hostname, const char *cwd)
{
	struct passwd *infos = NULL;

	infos = getpwuid(getuid());
	sprintf(prompt, "%s@%s %s$ ", infos->pw_name, hostname, cwd);
}

int cmd_exit(const char *str, int *exit_code)
{
	char *str2 = strdup(str);
	char *tok = NULL;

	tok = strtok(str2, " ");

	if(tok == NULL || strcmp(tok, "exit") != 0)
		return 0;

	*exit_code = 0;
	tok = strtok(NULL, " ");
	if(tok)
		*exit_code = atoi(tok);

	return 1;
}

void delete_ending_newline(char *str)
{
	size_t len;

	if(str != NULL)
	{
		len = strlen(str);
		if(str[len-1] == '\n')
			str[len-1] = '\0';
	}
}

void sigchld_handler(int sig)
{
	wait(NULL);
}
