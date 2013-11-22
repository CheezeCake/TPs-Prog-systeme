#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	int T[4] = {100, 100, 100, 100}; 
	int file;

	file = open("base.dat", O_WRONLY | O_CREAT | O_TRUNC,
							S_IRUSR | S_IWUSR);
	write(file, T, sizeof(int)*4);
	/*
	FILE *files = fopen("base.dat", "w");
	fwrite(T, sizeof(int), 4, files);
	*/
	close(file);

	return 0;
}
