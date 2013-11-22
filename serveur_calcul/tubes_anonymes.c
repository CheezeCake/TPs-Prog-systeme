#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define TEMPS_MAX 3*60

void demarrer_srv();
void attendre_op();
int operation(char, int , int);
void quit_client(int);
void nettoyer();

int p1[2];
int p2[2];
pid_t fils = 0;

int main()
{
	char o;
	int op1, op2;

	signal(SIGALRM, nettoyer);
	signal(SIGINT, quit_client);

	pipe(p1);
	pipe(p2);

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
		close(p1[1]);
		close(p2[0]);

		while(1)
			attendre_op();
	}

	printf("Serveur demarré (%d)\n", fils);

	/* initialisation de l'alarme */
	alarm(TEMPS_MAX);
	close(p1[0]);
	close(p2[1]);
}

void attendre_op()
{
	char o;
	int op1, op2;
	int res = 0;

	read(p1[0], &o, sizeof(char));
	read(p1[0], &op1, sizeof(int));
	read(p1[0], &op2, sizeof(int));

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
	write(p2[1], &res, sizeof(int));
}

int operation(char o, int op1, int op2)
{
	int ret = 0;

	/* re-initialisation de l'alarme */
	alarm(TEMPS_MAX);

	printf("CLIENT (%d): demande de %d %c %d\n", getpid(), op1, o, op2);
	write(p1[1], &o, sizeof(char));
	write(p1[1], &op1, sizeof(int));
	write(p1[1], &op2, sizeof(int));

	read(p2[0], &ret, sizeof(int));

	printf("CLIENT (%d): resultat reçu = %d\n", getpid(), ret);

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
	close(p1[1]);
	close(p2[0]);

	exit(0);
}
