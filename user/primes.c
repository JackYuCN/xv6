#include "kernel/types.h"
#include "user/user.h"

/**
 * @brief prime number sieve, using a pair of pipes to communicate between adjacent sieve.
 * 
 * @param plast pipe from last sleve 
 */
void sieve(int plast[2]) {
	close(plast[1]);

	int first = 0;	
	if (read(plast[0], &first, 4) == 0) {
		close(plast[0]);
	}
	else {
		printf("prime %d\n", first);

		int pnew[2];
		if (pipe(pnew) < 0) {
			fprintf(2, "pipe");
			exit(1);
		}

		if (fork() == 0) {
			close(plast[0]);
			sieve(pnew);
		}
		else {
			close(pnew[0]);
			
			int i = 0;
			while (read(plast[0], &i, 4) != 0) {
				if (i % first != 0) {
					write(pnew[1], &i, 4);
				}
			}
			close(plast[0]);
			close(pnew[1]);
			wait(0); 
		}
	}
} 

int 
main(int argc, char *argv[])
{
	int p[2];
	if (pipe(p) < 0) {
		fprintf(2, "pipe");
		exit(1);
	}	

	if (fork() == 0) {
		close(p[1]);
		sieve(p);
	}
	else {
		close(p[0]);
		for (int i = 2; i <= 35; i++) {
			write(p[1], &i, 4);
		}
		close(p[1]);
		wait(0);	// use wait here to wait for child, grandchild, grandgrandchild ... to complete.
	}

	exit(0);
}