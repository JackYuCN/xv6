#include "kernel/types.h"
#include "user/user.h"

int
fork1(void)
{
  int pid;

  pid = fork();
  if(pid == -1) {
		fprintf(2, "fork");
		exit(1);
	}
  return pid;
}

int
main(int argc, char *argv[])
{
	int p1[2], p2[2];
	char buf[10];

  if(pipe(p1) < 0 || pipe(p2) < 0) {
		fprintf(2, "pipe");
		exit(1);
	}

	if (fork1() == 0) {
		close(p1[1]);
		read(p1[0], buf, 5);
		close(p1[0]);
		printf("%d: received %s\n", getpid(), buf);

		write(p2[1], "ping", 5);		 
		close(p2[1]);	
		close(p2[0]);

	}
	else {
		close(p1[0]);
		write(p1[1], "pong", 5);
		close(p1[1]);

		close(p2[1]);
		read(p2[0], buf, 5);
		close(p2[1]);
		printf("%d: received %s\n", getpid(), buf);
	}

	exit(0);
}