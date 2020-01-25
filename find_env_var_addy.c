#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)
#define MAXBUFFERSIZE 4096

extern char **environ;

/* As I mentioned in my_setenv.c comments, writing my own
 * implementations of those syscalls made me wonder where
 * setenv() stores the actual new strings pointed to by
 * char **environ. You can't make them local to the function,
 * since the stack frame is torn down when it returns. Maybe
 * it puts them in the same place as the ones it already
 * points to, i.e. above the stack? I should be able to figure
 * this out with a gdb session.
 *
 * This program is going to mess around with setenv() and see
 * if I can find out where the char pointer poitners are
 * pointing to.
 *
 * */

int main(int argc, char *argv[]) {

    printf("Original environment variables:\n\n");

    char **ep;
    for (ep = environ; *ep != NULL; ep++) {
        printf("Virtual memory address: %p\n", *ep);
        puts(*ep);
        printf("strlen(env var) = %ld.", strlen(*ep));
        printf("Adding 1 for null character makes %ld.\n", strlen(*ep) + 1);
        printf("Next char pointer should be located at %p\n", *ep + strlen(*ep) + 1);
        printf("\n");
    }        
  
    return EXIT_SUCCESS;
}
