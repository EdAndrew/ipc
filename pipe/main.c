#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

void client(int readfd, int writefd);
void server(int readfd, int writefd);

int main()
{
	int pipe1[2], pipe2[2];
	pid_t childpid;
	
	pipe(pipe1);
	pipe(pipe2);
	
	if ((childpid = fork()) == 0)
	{
		close(pipe1[1]);
		close(pipe2[0]);
		server(pipe1[0], pipe2[1]);
		exit(0);
	}
	close(pipe1[0]);
	close(pipe2[1]);
	client(pipe2[0], pipe1[1]);
	waitpid(childpid, NULL, 0);
	exit(0);
}

void client(int readfd, int writefd)
{
	size_t len;
	ssize_t n;
	char buf[1024];

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

















