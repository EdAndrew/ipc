#include <stdio.h>
#include <mqueue.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define  FILE_MODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

int main(int argc, char *argv[])
{
	int c, flags;
	mqd_t mqd;

	flags = O_RDWR | O_CREAT;
	if (argc != 2)
	{
		fprintf(stderr, "usage: mqcreate <name>\n");
		exit(-1);
	}
	
	printf("%s\n", argv[1]);
	mqd = mq_open(argv[1], flags, FILE_MODE, NULL);
	if (mqd < 0)
	{
		fprintf(stderr, "create mq fail, error is %s\n", strerror(errno));
		exit(-1);
	}	
	mq_close(mqd);
	exit(0);
}
