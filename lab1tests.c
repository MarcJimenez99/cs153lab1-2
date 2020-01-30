#include "types.h"
#include "param.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"
#include "traps.h"
#include "memlayout.h"

int main(int argc, char *argv[])
{
	printf(1, "Running tests for waitpid functionality\n");
	printf(1, "Expected to reap all child processes in order\n\n");
	int pid[5];
	int i, child_status;
	for (i = 0; i < 5; i++) {
		if((pid[i] = fork()) == 0) {
			exit(100+i);
		}
	}
	for (i = 0; i < 5; i++) {
		int dumbpid = waitpid(pid[i], &child_status, 0);
		if (child_status) {
			printf(1, "Child %d terminated successfully\n", dumbpid);
		}
		else {
			printf(1, "Child %d terminated abnornamlly\n", dumbpid);
		}
	}
	exit(0);
}
