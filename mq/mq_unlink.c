#include <stdio.h>
#include <mqueue.h>

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "usage: mqunlink <name>\n");
		return -1;
	}
	
	mq_unlink(argv[1]);
	return 0;
}
