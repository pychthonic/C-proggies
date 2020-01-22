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

/* My implementation of setenv()
 * */


int my_setenv(const char *name, const char *value, int overwrite) {
    if (getenv(name) != NULL) {
        if (!overwrite) {
            return 0;
        }
        else {
            return 1; /* WRITE THIS CODE TOMORROW. */ 
        }
    } else {
        int var_count = 0; 
         
        while (*environ != NULL) {
            environ++;
            var_count++;
        }
                 
        char *env_ptr; 

        int i;
        
        size_t name_size = strlen(name);
        size_t value_size = strlen(value);
        size_t heap_space_needed = name_size + value_size + 2;
        env_ptr = (char *) malloc(sizeof(char) * heap_space_needed);
                        /* REMEMBER TO FREE THIS MEM BEFORE EXIT AT MAIN */      
        *environ = env_ptr;

        for (i=0; i<name_size; i++) {
            *env_ptr = *name;
            env_ptr++;
            name++;
        }
        *env_ptr = '=';
        env_ptr++;     
        for (i=0; i<value_size; i++) {
           *env_ptr = *value;
           env_ptr++;
           value++;
        }
        *env_ptr = '\0';

        environ++;
        *environ = NULL;
        environ -= var_count;
        environ--;
        return 0;
    }            
}

int main(int argc, char *argv[]) {

    char **ep;
    for (ep = environ; *ep != NULL; ep++)
        puts(*ep);
    
    if (my_setenv("BEEP_BEEP", "BEEPVALUE", 1) == 0) {
        printf("returned 0\n");
    }
     
    printf("\n\n\n######################\n\n\n");
    
    for (ep = environ; *ep != NULL; ep++)
        puts(*ep);
   
    ep--;
    free(*ep);
    *ep = NULL;
    
    printf("\n\n\n######################\n\n\n");
    
    for (ep = environ; *ep != NULL; ep++)
        puts(*ep);
 
    return EXIT_SUCCESS;
}
