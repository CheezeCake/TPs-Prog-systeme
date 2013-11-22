#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "utils.h"

pid_t pidn; /* pid new process */
pid_t pidt; /* pid term process */
pid_t pidr = -1; /* pid running process */
Process_list l;

void new_process(int sig)
{
	pidn = read_pid("pid.dat");
	kill(pidn, SIGSTOP);
	add_process(&l, pidn);

	/* pas de processus en cours d'execution */
	if(pidr == -1)
		raise(SIGALRM);

	printf("Ajout de %d\n", pidn);
}

void term_process(int sig)
{
	pidt = read_pid("pid.dat");
	delete_process(&l, pidt);

	printf("suppression de %d\n", pidt);
}

void ordo(int sig)
{
	if(empty(&l))
		return;

	if(pidr != -1)
	{
		kill(pidr, SIGSTOP);
		printf("STOP %d\n", pidr);
	}

	pidr = get_process(&l);
	if(pidr != -1)
	{
		kill(pidr, SIGCONT);
		printf("CONT %d\n", pidr);
		alarm(2);
	}
}

int main()
{
	printf("pid ordo: %d\n\n", getpid());
	init_list(&l);

	signal(SIGUSR1, new_process);
	signal(SIGUSR2, term_process);
	signal(SIGALRM, ordo);

	write_pid("ordo.dat", getpid());

	while(1)
		pause();

	return 0;
}
