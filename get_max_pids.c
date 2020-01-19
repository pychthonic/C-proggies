#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)
#define MAXBUFFERSIZE 256

/* This program reads the number from /proc/sys/kernel/pid_max and
 * prints the maximum number of process id's that the kernel will
 * allow. 
 * */


int main(int argc, char *argv[]) {
    int fd = open("/proc/sys/kernel/pid_max", O_RDONLY);
    if (fd == -1) {
        errExit("open /proc/sys/kernel/pid_max");
    }
    
    char pid_max[MAXBUFFERSIZE];
    if (read(fd, pid_max, MAXBUFFERSIZE) == -1) {
        errExit("read");
    }
    
    printf("max pids allowed by kernel: %s", pid_max); 

    if (close(fd) == -1) {
        errExit("close");
    }

    return EXIT_SUCCESS;
}
