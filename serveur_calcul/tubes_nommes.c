#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define TEMPS_MAX 3*60

void demarrer_srv();
void attendre_op();
int operation(char, int, int);
void quit_client(int);
void nettoyer();

pid_t fils = 0;

int main()
{
	char o;
	int op1, op2;
	mode_t mode;

	signal(SIGALRM, nettoyer);
	signal(SIGINT, quit_client);

	mode = S_IRUSR | S_IWUSR;
	mkfifo("tube1", mode);
	mkfifo("tube2", mode);

	demarrer_srv();

	while(1)
	{
		printf("> ");
		scanf("%c%d%d", &o, &op1, &op2);
		getchar();

		if(fils == 0)
			demarrer_srv();

		printf("< %d\n", operation(o, op1, op2));
	}

	return 0;
}

void demarrer_srv()
{
	printf("Demarrage du serveur par %d\n", getpid());

	fils = fork();

	if(fils == 0)
	{
		while(1)
			attendre_op();
	}

	printf("Serveur demarré (%d)\n", fils);

	/* initilisation de l'alarme */
	alarm(TEMPS_MAX);
}

void attendre_op()
{
	char o;
	int op1, op2;
	int res = 0;
	int p1, p2;

	p1 = open("tube1", O_RDONLY);
	p2 = open("tube2", O_WRONLY);

	read(p1, &o, sizeof(char));
	read(p1, &op1, sizeof(int));
	read(p1, &op2, sizeof(int));

	switch(o)
	{
		case '+':
			res = op1+op2;
			break;
		case '-':
			res = op1-op2;
			break;
		case '*':
			res = op1*op2;
			break;
		case '/':
			res = op1/op2;
			break;
	}

	printf("SRV (%d): op realisée, %d %c %d = %d\n", getpid(), op1, o, op2, res);
	write(p2, &res, sizeof(int));

	close(p1);
	close(p2);
}

int operation(char o, int op1, int op2)
{
	int ret = 0;
	int p1, p2;

	/* re-initialisation de l'alarme */
	alarm(TEMPS_MAX);

	p1 = open("tube1", O_WRONLY);
	p2 = open("tube2", O_RDONLY);

	printf("CLIENT (%d): demande de %d %c %d\n", getpid(), op1, o, op2);
	write(p1, &o, sizeof(char));
	write(p1, &op1, sizeof(int));
	write(p1, &op2, sizeof(int));

	read(p2, &ret, sizeof(int));
	printf("CLIENT (%d): resultat reçu = %d\n", getpid(), ret);

	close(p1);
	close(p2);

	return ret;
}

void nettoyer()
{
	if(fils != 0)
	{
		printf("Kill du serveur (%d)\n", fils);
		kill(fils, SIGKILL);
		fils = 0;
	}
}

void quit_client(int sig)
{
	nettoyer();
	unlink("tube1");
	unlink("tube2");

	exit(0);
}
