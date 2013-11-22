#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

void handler(int sig)
{
	char buffer[256];

	printf("\nEntrez le mot de passe: ");
	scanf("%s", buffer);

	if(strcmp(buffer, "pass") == 0)
		exit(0);

	printf("Vous n'êtes pas autorisé à arreter ce processus\n");
}

int main(void)
{
	signal(SIGINT, handler);

	while(1)
		pause();

	return 0;
}
