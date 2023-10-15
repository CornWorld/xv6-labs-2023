#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char buf[512];
char *arg[514];

int main(int argc, char ** argv) {
	if(argc<2) {
		fprintf(2, "xargs: need 1 more arg.\n");
		exit(-1);
	}
	int i;
	for(i=1; i<argc; i++) {
		arg[i-1]=argv[i];
	}
    arg[argc-1]=buf;
    arg[argc]=0;
	while(gets(buf, 511)[0]!='\0') {
		buf[strlen(buf)-1]='\0'; // QwQ!!!
		if(fork()!=0) {
			wait(0);
		} else {
			// for(i=0; i<argc; i++) {
				// fprintf(2, "%s ", arg[i]);
			// }
			// fprintf(2, "\n");
			
			exec(argv[1], arg);
			fprintf(2, "xargs: exec %s failed\n", argv[1]);
            exit(-1);
		}
	}
	exit(0);
}