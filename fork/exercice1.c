#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
	pid_t fils;

	fils = fork();
	if(fils == -1)
	{
		perror("fork");
		return 1;
	}

	if(fils == 0)
		printf("Dans le fils pid = %d, ppid = %d\n", getpid(), getppid());
	else
	{
		printf("Dans le pere pid = %d, pid fils = %d\n", getpid(), fils);
		wait(NULL);
	}

	return 0;
}

/*
 * si le père se termine avant le fils, le processus fils sera
 * adopté par le processus de pid 1 (init)
 */
