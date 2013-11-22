#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
	int n;
	int file;
	int v = 0;

	if(argc != 2)
	{
		fprintf(stderr, "mauvais nombre d'arguments\n");
		return 1;
	}

	n = atoi(argv[1]);
	if(n > 0 && n <= 4)
	{
		n--;
		file = open("base.dat", O_RDWR);

		if(file == -1)
		{
			perror("open base.dat");
			return 2;
		}

		//pread(file, &v, sizeof(int), sizeof(int)*n);

		lseek(file, sizeof(int)*n, SEEK_SET);
		lockf(file, F_LOCK, sizeof(int));
		read(file, &v, sizeof(int));
		v--;
		
		lseek(file, sizeof(int)*n, SEEK_SET);
		write(file, &v, sizeof(int));
		lockf(file, F_ULOCK, sizeof(int));
		close(file);
	}

	return 0;
}
