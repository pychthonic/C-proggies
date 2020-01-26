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
 * And it turns out, setenv() uses the heap for the strings
 * it makes char **environ point to. This makes sense, since
 * if the space above the stack was going to be used, it
 * would have to be allocated for the largest possible
 * strings that can be held as environment variables, which
 * isn't a good use of virtual memory space. However, my 
 * next question is, how is that memory free'd at the end
 * of the program?
 *
 * */

int main(int argc, char *argv[]) {
    

    printf("\nsbrk(0) = %p\n", sbrk(0));

    printf("Original environment variables:\n\n");


    printf("\nsbrk(0) = %p\n", sbrk(0));

    char **ep;
    for (ep = environ; *ep != NULL; ep++) {
        printf("Virtual memory address: %p\n", *ep);
        puts(*ep);
        printf("Next char pointer should be located at %p\n", *ep + strlen(*ep) + 1);
        printf("\n");
    }        
    
    printf("\nsbrk(0) = %p\n", sbrk(0));
    printf("\n\n#####################################################\n\n");
    printf("\n\n### Changing 'HOME' environment variable... ###\n\n");
    
    setenv("HOME", "NEW_HOME", 1);
    
    printf("\nsbrk(0) = %p\n", sbrk(0));
    for (ep = environ; *ep != NULL; ep++) {
        printf("Virtual memory address: %p\n", *ep);
        puts(*ep);
        printf("Next char pointer should be located at %p\n", *ep + strlen(*ep) + 1);
        printf("\n");
    }        

    printf("\nsbrk(0) = %p\n", sbrk(0));

    return EXIT_SUCCESS;
}
