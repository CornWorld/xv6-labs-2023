#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

const int si=sizeof(int);

int l, r;

void work() {
	int n, x;
	read(l, &n, si);
	fprintf(1, "prime %d\n", n);
	while(read(l, &x, si)==si) {
		if(x%n) write(r, &x, si);
	}
	close(l), close(r);
	exit(0);
}

int main() {
	int p[2];
	pipe(p);
	r=p[1];
	
	int i;
	for(i=2; i<=35; i++) write(r, &i, si);
	close(r);

	for(i=1; i<=11; i++) {
		l=p[0];
		pipe(p);
		r=p[1];
		
		//fprintf(1, "r:%d w:%d\n", l, r);

		if(fork()!=0) {
			wait((int *)0);
			close(l), close(r);
		} else {
			work();
		}
	}

	exit(0);
}
