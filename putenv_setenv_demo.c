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

/* This program shows why you shouldn't use putenv() on an automatic
 * variable (that is, a variable inside a stack frame that could be
 * returned). putenv() doesn't make a copy of the string buffer in
 * the heap, so if you try to access the environment variable after
 * its function returns, you'll get unpredictable behavior.
 * This program first prints out all the environment variables for
 * the process. Then it calls foo(), which uses both putenv() and
 * setenv() to create two new environment variables. Then it calls
 * its own setenv() and putenv(), so 4 total new environment
 * variables have been  created. It prints out all the environment
 * variables, and this is where unpredictable behavior happens -
 * the PUTENV_AUTO_TESTNAME variable points to a stack frame that
 * was torn down, so on my machine, it printed out garbage.
 * */

int foo() {
    char newautovar[80] = "PUTENV_AUTO_TESTNAME=PUTENV_AUTO_TESTVALUE"; 
    putenv(newautovar);
    setenv("SETENV_AUTO_TESTNAME", "SETENV_AUTO_TESTVALUE", 1);
}

int main(int argc, char *argv[]) {
    char **ep;
    for (ep = environ; *ep != NULL; ep++)
        puts(*ep);
    
    foo();
    
    setenv("SETENV_TESTNAME", "TESTVALUE", 1);
    putenv("PUTENV_TESTNAME=PUTENV_TESTVALUE");
    
    printf("\n\n\n######################\n\n\n");
    
    for (ep = environ; *ep != NULL; ep++)
        puts(*ep);
    
    return EXIT_SUCCESS;
}
