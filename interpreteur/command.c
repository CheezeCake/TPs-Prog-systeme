#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "command.h"

void init_command(Command *cmd)
{
	/*cmd->init_str = NULL;*/
	cmd->members = NULL;
	cmd->nb_members = 0;

	cmd->args = NULL;
	cmd->nb_args = NULL;

	cmd->redirect = NULL;
	cmd->type_redirect = NULL;

	cmd->background = 0;
}

void destroy_command(Command *cmd)
{
	int i, j;

	for(i = 0; i < cmd->nb_members; i++)
	{
		if(cmd->redirect != NULL && cmd->redirect[i] != NULL)
		{
			free(cmd->redirect[i][STDIN]);
			free(cmd->redirect[i][STDOUT]);
			free(cmd->redirect[i][STDERR]);
			free(cmd->redirect[i]);
		}

		if(cmd->type_redirect != NULL && cmd->type_redirect[i] != NULL)
			free(cmd->type_redirect[i]);

		if(cmd->members != NULL && cmd->members[i] != NULL)
			free(cmd->members[i]);

		if(cmd->args != NULL && cmd->args[i] != NULL)
		{
			for(j = 0; j < cmd->nb_args[i]; j++)
				free(cmd->args[i][j]);
			free(cmd->args[i]);
		}
	}

	/*free(cmd->initial_str);*/
	free(cmd->members);
	free(cmd->args);
	free(cmd->redirect);
	free(cmd->type_redirect);
}

int parse_members(char *initial_str, Command *cmd)
{
	int i;
	int len;
	char *token = NULL;

	/*cmd->initial_str = strdup(initial_str);*/
	cmd->nb_members = countchar(initial_str, '|')+1;
	cmd->members = calloc(cmd->nb_members, sizeof(char*));
	for(i = 0; i < cmd->nb_members; i++)
		cmd->members[i] = NULL;

	i = 0;
	for(token = strtok(initial_str, "|");
			token;
			token = strtok(NULL, "|"))
	{
		/*
		 * supprimmer les espaces
		 */
		while(*token == ' ')
			++token;

		len = strlen(token)-1;
		if(len <= 0)
			return 1;

		while(token[len] && token[len] == ' ')
			token[len--] = '\0';

		cmd->members[i++] = strdup(token);
	}

	return 0;
}

void aff_members(const Command *cmd)
{
	int i;
	for(i = 0; i < cmd->nb_members; i++)
		printf("member[%d] = '%s'\n", i, cmd->members[i]);
}

int parse_args(Command *cmd)
{
	int i, j;
	char *token = NULL;
	char *member = NULL;

	cmd->args = calloc(cmd->nb_members, sizeof(char**));
	cmd->nb_args = calloc(cmd->nb_members, sizeof(int*));
	cmd->redirect = calloc(cmd->nb_members, sizeof(char**));
	cmd->type_redirect = calloc(cmd->nb_members, sizeof(int*));
	for(j = 0; j < cmd->nb_members; j++)
	{
		cmd->args[j] = NULL;
		cmd->redirect[j] = NULL;
		cmd->type_redirect[j] = NULL;
	}

	for(i = 0; i < cmd->nb_members; i++)
	{
		member = strdup(cmd->members[i]);

		cmd->nb_args[i] = countwords(member)+1;
		cmd->args[i] = calloc(cmd->nb_args[i], sizeof(char*));
		for(j = 0; j < cmd->nb_args[i]; j++)
			cmd->args[i][j] = NULL;

		cmd->redirect[i] = calloc(3, sizeof(char*));
		cmd->redirect[i][STDIN] = NULL;
		cmd->redirect[i][STDOUT] = NULL;
		cmd->redirect[i][STDERR] = NULL;

		cmd->type_redirect[i] = calloc(3, sizeof(int));
		cmd->type_redirect[i][STDOUT] = NORMAL;
		cmd->type_redirect[i][STDERR] = NORMAL;

		j = 0;
		for(token = strtok(member, " ");
				token;
				token = strtok(NULL, " "))
		{
			if(!parse_redirect(i, token, cmd))
				cmd->args[i][j++] = strdup(token);
		}

		if(strcmp(cmd->args[i][j-1], "&") == 0)
		{
			if(cmd->nb_members > 1)
			{
				fprintf(stderr, "pas executable en background\n");
				return 1;
			}

			cmd->background = 1;
			free(cmd->args[i][j-1]);
			cmd->args[i][j-1] = NULL;
		}

		cmd->args[i][j] = NULL;
	}

	return 0;
}

void aff_args(const Command *cmd)
{
	int i, j;
	for(i = 0; i < cmd->nb_members; i++)
	{
		for(j = 0; j < cmd->nb_args[i]; j++)
			printf("args[%d][%d] = '%s' ", i, j, cmd->args[i][j]);
		printf("\n");
	}
}

int parse_redirect(int i, const char *token, Command *cmd)
{
	if(strcmp(token, ">>") == 0)
	{
		cmd->redirect[i][STDOUT] = strdup(strtok(NULL, " "));
		cmd->type_redirect[i][STDOUT] = APPEND;
	}
	else if(strcmp(token, "2>>") == 0)
	{
		cmd->redirect[i][STDERR] = strdup(strtok(NULL, " "));
		cmd->type_redirect[i][STDERR] = APPEND;
	}
	else if(strcmp(token, ">") == 0)
		cmd->redirect[i][STDOUT] = strdup(strtok(NULL, " "));
	else if(strcmp(token, "<") == 0)
		cmd->redirect[i][STDIN] = strdup(strtok(NULL, " "));
	else if(strcmp(token, "2>") == 0)
		cmd->redirect[i][STDERR] = strdup(strtok(NULL, " "));
	else
		return 0;

	return 1;
}

void aff_redirect(const Command *cmd)
{
	int i;
	for(i = 0; i < cmd->nb_members; i++)
	{
		printf("redirect[%d][STDIN] = '%s'\n", i, cmd->redirect[i][STDIN]);
		printf("redirect[%d][STDOUT] = '%s'", i, cmd->redirect[i][STDOUT]);
		if(cmd->type_redirect[i][STDOUT] == APPEND)
			printf(" (APPEND)");
		printf("\n");

		printf("redirect[%d][STDERR] = '%s'", i, cmd->redirect[i][STDERR]);
		if(cmd->type_redirect[i][STDERR] == APPEND)
			printf(" (APPEND)");
		printf("\n");
	}
}

int countchar(const char *str, char c)
{
	int ret = 0;

	for(; *str; str++)
	{
		if(*str == c)
			++ret;
	}

	return ret;
}

int countwords(const char *str)
{
	int ret = 0;

	while(*str++)
	{
		if(*str == ' ' && isalpha(*(str-1)))
			++ret;
	}

	return (ret+1);
}
