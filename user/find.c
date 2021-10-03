#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *directory, char *file) {
		
	char buffer[512], *pointer;
	int fd;
	struct dirent dirEntries;    		//returns info about directory entries
	struct stat fileSt;		        //returns info about a file	
	
	if((fd = open(directory, 0)) < 0) {
		fprintf(2, "find: cannot open %s\n", directory);
		return;
	}
	if(fstat(fd, &fileSt) < 0) {
		fprintf(2, "find: cannot stat %s\n", directory);
		close(fd);
		return;
	}
	if(fileSt.type != T_DIR) {
		fprintf(2, "find: %s is not a directory\n", directory);
		close(fd);														return;
	}
	if(strlen(directory) + 1 + DIRSIZ + 1 > sizeof buffer) {
	fprintf(2, "find: directory too long\n");
	close(fd);
	return;
	}

	strcpy(buffer, directory);
	pointer  = buffer + strlen(buffer);
	*pointer++ = '/';

	while(read(fd, &dirEntries, sizeof(dirEntries)) == sizeof(dirEntries)) {	
		if(dirEntries.inum == 0) {
			continue;	
		}
		if(!strcmp(dirEntries.name,".") || !strcmp(dirEntries.name,"..")) {
			continue;

		}			
		memmove(pointer, dirEntries.name, DIRSIZ);
		pointer[DIRSIZ] = 0;
		if(stat(buffer, &fileSt) < 0) {
			fprintf(2, "find: cannot stat %s\n", buffer);
			continue;
		}
		if(fileSt.type == T_DIR) {
			find(buffer, file);
		}
		else if(fileSt.type == T_FILE && !strcmp(dirEntries.name, file)) {
			printf("%s\n", buffer);												}
	}
}	


int
main(int argc, char *argv[])
{
	if(argc != 3) {
		fprintf(2, "usage: find dirName fileName\n");
		exit(1);	      
	}
	find(argv[1], argv[2]);
	exit(0);
}

