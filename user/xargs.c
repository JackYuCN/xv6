#include "kernel/types.h"
#include "kernel/param.h"
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
	int i, j;
	char buf[512];
	char *new_argv[MAXARG];

	memset(buf, 0, sizeof(buf));
	for (i = 1; i < argc; i++) 
		new_argv[i - 1] = argv[i];

	j = 0;
	while (read(0, buf + j, 1) != 0) {
		if (buf[j] == '\n') {
			if (fork() == 0) {
				buf[j] = '\0';
				new_argv[i - 1] = buf;
				exec(new_argv[0], new_argv);
			}
			else {
				wait(0);
				memset(buf, 0, sizeof(buf));
				j = -1;
			}
		}
		j++;
	}

	exit(0);
	return 0;
}