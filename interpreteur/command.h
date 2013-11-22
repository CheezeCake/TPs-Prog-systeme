#ifndef COMMAND_H
#define COMMAND_H

#define CMD_MAX_LENGTH 512

typedef struct Command
{
	/*char *initial_str;*/

	char **members; /* membres (separes par '|') */
	int nb_members;

	char ***args; /* arguments de chaque membres */
	int *nb_args;

	char **redirect; /* chemin du fichier ou rediriger */
	int **type_redirect; /* type redirection (RAPPEND ou non) */
} Command;

void init_command(Command*);
void destroy_command(Command*);

int parse_members(char*, Command*);
void aff_members(Command*);
int parse_args(Command*);
void aff_args(Command*);

int countchar(const char*, char);
int countwords(const char*);

#endif
