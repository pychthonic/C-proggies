#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>

#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)

/* This program prints hard and soft limits on the number of open
 * file descriptors for the process. On the system I'm coding on,
 * the soft limit is 1024 and the hard limit is around 1 million.
 * */


int main(int argc, char *argv[]) {
    
    struct rlimit rlim;
    
    errno = 0;
    if (getrlimit(RLIMIT_NOFILE, &rlim) == -1) {
        errExit("getrlimit");
    }
	
    printf("soft max fds: %ld\n", rlim.rlim_cur);
    printf("hard max fds: %ld\n", rlim.rlim_max);

    return EXIT_SUCCESS;
}
