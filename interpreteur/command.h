#ifndef COMMAND_H
#define COMMAND_H

#define CMD_MAX_LENGTH 512

#define STDIN 0
#define STDOUT 1

#define NORMAL 0
#define RAPPEND 1

typedef struct Command
{
	/*char *initial_str;*/

	char **members; /* membres (separes par '|') */
	int nb_members;

	char ***args; /* arguments de chaque membres */
	int *nb_args;

	char ***redirect; /* chemins fichiers ou rediriger */
	int *type_redirect; /* type redirection stdout (RAPPEND ou non) */
} Command;

void init_command(Command*);
void destroy_command(Command*);

int parse_members(char*, Command*);
void aff_members(const Command*);
int parse_args(Command*);
void aff_args(const Command*);
void aff_redirect(const Command*);

int countchar(const char*, char);
int countwords(const char*);

#endif
