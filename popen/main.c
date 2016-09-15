#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#define MAXLINE 1024


int main()
{
	size_t n;
	char buf[MAXLINE], command[MAXLINE];
	FILE *fp;

	fgets(buf, MAXLINE, stdin);
	n = strlen(buf);
	if (buf[n-1] == '\n')
		n--;
	snprintf(command, MAXLINE, "cat %s", buf);
	fp = popen(command, "r");
	while (fgets(buf, MAXLINE, fp) != NULL)
		fputs(buf, stdout);
	pclose(fp);
	return 0;
}
