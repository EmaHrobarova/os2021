#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


#define READ 0
#define WRITE 1

void redirectInOut(int i, int arr[]) {     //redirect to pipe
	close(i);
	dup(arr[i]);
	close(arr[READ]);
	close(arr[WRITE]);
}

void filter(int p) {
	int n;
	while(read(READ, &n, sizeof(n))) {
		if(n % p != 0) {
			write(WRITE, &n, sizeof(n));
		}
	}
	close(WRITE);
	wait(0);    //so the primes in main wont return after second exit
	exit(0);
}

void numberWait() {
	int pDirect[2];
	int pr;

	if(read(READ, &pr, sizeof(pr))) {
		printf("prime %d\n",pr);
		pipe(pDirect);
		if(fork() == 0) {   //child
			redirectInOut(READ,pDirect);
			numberWait();
		}
		else {		    //parent
			redirectInOut(WRITE,pDirect);
			filter(pr);
		}
	}
	exit(0);	
}	


int
main(int argc, char *argv[])
{
	int p[2];
	pipe(p);

	if(fork() == 0) {      //child
		redirectInOut(READ,p);
		numberWait();
	}
	else {                 //parent
		redirectInOut(WRITE,p);
		for(int i=2; i<36; i++) {
			write(WRITE, &i, sizeof(i));
		}
		close(WRITE);     //READ side returns zero
		wait(0);          //waiting for all others primes exits
		exit(0);
	}
	return 0;
}
