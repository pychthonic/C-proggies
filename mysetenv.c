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

/* My implementations of setenv() and unsetenv(). It uses the heap for
 * the new environment variable. I decided as an exercise to try to
 * make sure the memory I malloc'ed was freed at the end of the
 * program which was tricky. To do this, I had to move the edited
 * environment variable (whether new or edited) to the last one in
 * the char **environ array. Then, depending on the Return Value of
 * my_setenv, I would know whether I had to free that last char *.
 * */


int my_setenv(const char *name, const char *value, int overwrite) {
    char *old_env_ptr = getenv(name);
    if (old_env_ptr != NULL) {
        if (!overwrite) {
            return 0;
        } else {
            char new_env_ptr[MAXBUFFERSIZE];
            strncpy(new_env_ptr, name, strlen(name));
            strcat(new_env_ptr, "=");
            strcat(new_env_ptr, value);
            if (strcmp(old_env_ptr, new_env_ptr) == 0) {
                return 0;
            } else {
                char **environ_orig_position = environ;
                char search_string[MAXBUFFERSIZE] = "\0";
                strncpy(search_string, name, strlen(name));
                strcat(search_string, "=");
                while (strstr(*environ, search_string) != *environ)
                    environ++;
                printf("After while loop, environ = %p\n", *environ);
                printf("After while loop environ points to %s\n", *environ);
                printf("current environ - environ backup = %ld\n", environ - environ_orig_position);
                char *new_heap_env_ptr = (char *) malloc(sizeof(new_env_ptr));
                strncpy(new_heap_env_ptr, new_env_ptr, strlen(new_env_ptr));
                printf("new_heap_env_ptr = %s\n", new_heap_env_ptr); 
                
                *environ = new_heap_env_ptr;

                environ = environ_orig_position;
                printf("After resetting, environ = %s\n", *environ);
                return 2;
            }
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
  
    char *old_ep = getenv("HOME");
    printf("old_ep = %p\n", old_ep);
    printf("old_ep points to %s\n", old_ep); 


    int ret = my_setenv("HOME", "WHOOPS", 1); 
    if (ret == 2) {
        printf("\nChanged env variable from 'HOME' to 'NEW HOME'\n");
    }
     
    printf("\n\n\n######################\n\n\n");
    
    for (ep = environ; *ep != NULL; ep++)
        puts(*ep);
  
    /* 
    ep--;
    free(*ep);
    *ep = NULL;
   */ 
    printf("\n\n\n######################\n\n\n");
    
    for (ep = environ; *ep != NULL; ep++)
        puts(*ep);
 
    return EXIT_SUCCESS;
}
