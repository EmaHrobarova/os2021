#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"
#define MAXN 1024

int main(int argc, char *argv[])
{
	//ivalid arguments
	if (argc <2) {
		fprintf(2, "usage: xargs command\n");
		exit(1);
	}

	char *buffer_argv[MAXARG];
	for (int i = 1; i < argc; i++)
		buffer_argv[i - 1] = argv[i];
	char buffer[MAXN];
	char c = 0;
	int status = 1;

	while (status) {
		int counter_buf = 0;
		int arg_start = 0;
		int counter_argv = argc - 1;

		//reading command line arguments
		while (1) {
			status = read(0, &c, 1);
			if (status == 0)
				exit(0);
			if (c == ' ' || c == '\n') {
				buffer[counter_buf++] = 0;
				buffer_argv[counter_argv++] = &buffer[arg_start];
				arg_start = counter_buf;
				if (c == '\n') 
					break;
			} else {
				buffer[counter_buf++] = c;
			}
		}

		//est commnands exe
		buffer_argv[counter_argv] = 0;
		if (fork() == 0) {
			exec(buffer_argv[0], buffer_argv);
		} else {
			wait(0);   //for child
		}
	}
	exit(0);
}

