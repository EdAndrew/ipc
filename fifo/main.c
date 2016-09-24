#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define  FILE_MODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)
#define FIFO1 "/tmp/fifo.1"
#define FIFO2 "/tmp/fifo.2"

void client(int readfd, int writefd);
void server(int readfd, int writefd);

int main()
{
	int readfd, writefd;
	pid_t childpid;

	if ((mkfifo(FIFO1, FILE_MODE) < 0) && (errno != EEXIST))
	{
		fprintf(stderr, "cannot create fifo1.\n");
		exit(0);
	}	
	if ((mkfifo(FIFO2, FILE_MODE) < 0) && (errno != EEXIST))
	{
		fprintf(stderr, "cannot create fifo2.\n");	
		unlink(FIFO1);
		exit(0);	
	}
	if ((childpid = fork()) == 0)
	{
		writefd = open(FIFO2, O_WRONLY, 0);	//there is dead lock problem!
		readfd = open(FIFO1, O_RDONLY, 0);
		server(readfd, writefd);
		close(writefd);
		close(readfd);
		exit(0);
	}
	readfd = open(FIFO2, O_RDONLY, 0);
	writefd = open(FIFO1, O_WRONLY, 0);
	client(readfd, writefd);
	waitpid(childpid, NULL, 0);
	close(writefd);
	close(readfd);
	unlink(FIFO1);
	unlink(FIFO2);
	exit(0);
}

void client(int readfd, int writefd)
{
	size_t len;
	ssize_t n;
	char buf[1024];
	
	//printf("clinet");
	fgets(buf, 1024, stdin);
	len = strlen(buf);
	if (buf[len-1] == '\n')
	{
		len--;
	}
	
	write(writefd, buf, len);
	while ((n = read(readfd, buf, 1024)) > 0)
		write(STDOUT_FILENO, buf, n);
}

void server(int readfd, int writefd)
{
	int fd;
	ssize_t n;
	char buf[1024+1];
	
	//printf("server");
	if ((n = read(readfd, buf, 1024)) == 0)
	{
		fprintf(stderr, "empty file name.\n"); 
		exit(0);	
	}
	buf[n] = '\0';
	
	if ((fd = open(buf, O_RDONLY)) < 0)
	{
		snprintf(buf+n, sizeof(buf)-n, " : cannot open, error = %s\n", strerror(errno));
		n = strlen(buf);
		write(writefd, buf, n);	
		exit(0);
	}
	while ((n = read(fd, buf, 1024)) > 0)
		write(writefd, buf, n);
	close(fd);
	exit(0);
}
