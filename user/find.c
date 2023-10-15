#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char* fmtname(char *path) {
	char *p=path+strlen(path);
	while (*p!='/' && path<=p) p--; // cannot guarantee that path contains a slash
	p+=1;
	int pl=strlen(p);
	
	char *buf=(char*)malloc(sizeof(char) * (DIRSIZ + 1));
	memmove(buf, p, pl);
	memset(buf+pl, ' ', DIRSIZ-pl);
	buf[DIRSIZ] = '\0';
	return buf;
}

void find(char *path, char *name) {
	// fprintf(2, "find: %s \n", path);
	int fd;
	struct stat st;
	struct dirent de;
	if((fd=open(path, 0))<0) {
		fprintf(2, "find: cannot open %s\n", path);
		return;
	}
	if(fstat(fd, &st)<0) {
		fprintf(2, "find: cannot fstat %s\n", path);
		close(fd);
		return;
	}
	char buf[512];
	char *n;
	switch(st.type) {
	case T_FILE:
		n=fmtname(path);
		// fprintf(2, "f: n %s|name %s|\n", n, name);
		if(!strcmp(name, n)) {
			fprintf(1, "%s\n", path);
		}
		free(n);
		break;
	case T_DIR:
		if(strlen(path)+DIRSIZ+1>sizeof(buf)) {
			fprintf(2, "find: path too long\n");
			break;
		}
		strcpy(buf, path);
		char* p=buf+strlen(buf)+1;
		*(p-1)='/';
		// fprintf(2, "buf: %s\n", buf);
		while(read(fd, &de, sizeof(de))==sizeof(de)) {
			if(!de.inum) continue;
			if(!strcmp(de.name, ".") || 
				!strcmp(de.name, "..")) continue;
			memmove(p, de.name, DIRSIZ);
			p[DIRSIZ]=0;
			if(stat(buf, &st)<0){
				printf("find: cannot stat %s\n", buf);
		    	continue;
		    }
			find(buf, name);
		}
		break;
	}
	close(fd);
}

void main(int argc, char** argv) {
	if(argc!=3) {
		fprintf(2, "find: need only 2 arg.\n");
		exit(-1);
	}
	char *name=fmtname(argv[2]);
	find(argv[1], name);
	free(name);
	exit(0);
}