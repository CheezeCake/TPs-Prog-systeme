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
		file = open("base.dat", O_RDWR, 0);

		if(file == -1)
		{
			perror("open base.dat");
			return 2;
		}

		//pread(file, &v, sizeof(int), sizeof(int)*n);

		lseek(file, sizeof(int)*n, SEEK_SET);
		read(file, &v, sizeof(int));
		lseek(file, sizeof(int)*n, SEEK_SET);
		
		if(v != 0)
		{
			v--;
			write(file, &v, sizeof(int));
		}
		close(file);
	}

	return 0;
}
