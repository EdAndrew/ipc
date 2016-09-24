
#include <stdio.h>
#include <string.h>
#include <mqueue.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int c, flags;
	mqd_t mqd;
	ssize_t n;
	unsigned prio;
	void *buff;
	struct mq_attr attr;

	flags = O_RDONLY;
	while ((c = getopt(argc, argv, "n")) != -1)
	{
		switch (c)
		{
		case 'n':
			flags |= O_NONBLOCK;
			break;
		}
	}	
	if (optind != argc-1)
	{
		fprintf(stderr, "usage: mqreceive [-n] <name>");
		return -1;
	}	
	
	mqd = mq_open(argv[optind], flags);
	mq_getattr(mqd, &attr);
	buff = malloc(attr.mq_msgsize);
	n = mq_receive(mqd, buff, attr.mq_msgsize, &prio);
	printf("read %ld bytes, prio = %u\n", (long)n, prio);
	return 0;
}

