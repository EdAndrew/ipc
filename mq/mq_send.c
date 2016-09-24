#include <stdio.h>
#include <string.h>
#include <mqueue.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	mqd_t mqd;
	void *ptr;
	size_t len;
	unsigned prio;

	if (argc != 4)
	{
		fprintf(stderr, "usage: mqsend <name> <#bytes> <priority>");
		return -1;
	}
	
	len = atoi(argv[2]);
	prio = atoi(argv[3]);
	mqd = mq_open(argv[1], O_WRONLY);
	ptr = calloc(len, sizeof(char));
	mq_send(mqd, ptr, len, prio);
	return 0;
}	
