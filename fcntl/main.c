#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>

#define SEQFILE "seqno"
#define MAXLINE 1024
#define FILE_MODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

void my_lock(int);
void my_unlock(int);

int main(int argc, char *argv[])
{
	int fd;
	long i, seqno;
	pid_t pid;
	ssize_t n;
	char line[MAXLINE + 1];
	
	pid = getpid();
	fd = open(SEQFILE, O_RDWR, FILE_MODE);
	for (i = 0; i < 20; ++i)
	{
		my_lock(fd);
		lseek(fd, 0L, SEEK_SET);
		n = read(fd, line, MAXLINE);
		line[n] = '\0';
		n = sscanf(line, "%ld\n", &seqno);
		printf("%s: pid = %ld, seq# = %ld\n", argv[0], (long)pid, seqno);
		++seqno;
		snprintf(line, sizeof(line), "%ld\n", seqno);
		lseek(fd, 0L, SEEK_SET);
		write(fd, line, strlen(line));
	}
}
