#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <errno.h>

#define FILE_MODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

int main()
{
	int c, flags;
	sem_t *sem;
	unsigned int value;
	
	flags = O_RDWR | O_CREAT | O_EXCL;
	value = 1;

	sem = sem_open("/tmp/test", flags, FILE_MODE, value);
	if (sem == SEM_FAILED)
		printf("error\n");
	perror("sem");
}
