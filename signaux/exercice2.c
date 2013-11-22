#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void message(int sig)
{
	static int c = 0;

	printf("bonjour\n");

	if(++c == 5)
	{
		printf("au revoir\n");
		c = 0;
	}

	alarm(1);
}

int main(void)
{
	signal(SIGALRM, message);
	raise(SIGALRM);

	while(1)
		pause();

	return 0;
}
