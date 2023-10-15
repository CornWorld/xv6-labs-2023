#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int p[2];

int main() {
	pipe(p);
	if(fork()!=0) {
		write(p[1], "1", 2);
		wait((int *)0);
		if(read(p[0], (int *)0, 2)==2) {
                        fprintf(0, "%d: received pong\n", getpid());
                        exit(0);
                }
	} else {
		if(read(p[0], (int *)0, 2)==2) {
			fprintf(0, "%d: received ping\n", getpid());
			write(p[1], "2", 2);
			exit(0);
		}
	}

	exit(-1);
}
