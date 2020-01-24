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

/* My implementation of unsetenv(). This was a lot less
 * complicated than my_setenv().
 * 
 * Return Values of my_unsetenv():
 * 
 * variable existed in environment, but no longer: 0
 * variable did not exist in the environment: 1 
 *
 * */


int my_unsetenv(const char *name) {
    char *old_env_ptr = getenv(name);
    if (old_env_ptr == NULL) {
        return 1;
    }
    
    char **environ_backup_local = environ;
   
    char search_string[MAXBUFFERSIZE] = "\0";
    strncpy(search_string, name, strlen(name));
    strcat(search_string, "=");

    while (strstr(*environ, search_string) != *environ)
        environ++;

    for (; *environ != NULL; environ++) {
        *environ = *(environ + 1);
    }

    environ = environ_backup_local;
    
    return 0;
}

int main(int argc, char *argv[]) {

    printf("Original environment variables:\n\n");

    char **ep;
    for (ep = environ; *ep != NULL; ep++)
        puts(*ep);
    
    my_unsetenv("HOME");
     
    printf("\n\n\n######################\n\n\n");
    printf("POOF! Find a 'HOME' variable. I dare you:\n\n"); 

    for (ep = environ; *ep != NULL; ep++)
        puts(*ep);
    printf("\n");
    
    return EXIT_SUCCESS;
}
