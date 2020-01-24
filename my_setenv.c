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

/* My implementations of setenv().
 *
 * It uses the heap for new and modified environment variables.
 *
 * I decided to make sure the memory I malloc'ed was freed at
 * the end of the program. This made me wonder where the
 * setenv() syscall stores the actual new strings pointed to
 * by char **environ. You can't make them local to the function,
 * since the stack frame is torn down when it returns. Maybe
 * it puts them in the same place as the ones it already
 * points to, i.e. above the stack? I should be able to figure
 * this out with a gdb session.
 *
 * The freeing of heap memory and resetting of environ in this 
 * implementation is done in main(), even though I'm aware that
 * kind of work would never be left to the user to figure out.
 *
 * Return Values of my_setenv():
 * 
 * environment variable exists but overwrite flag was 0: 0
 * environment variable existed and was overwritten: 1
 * environment variable did not exist, but now it does: 2
 * error: -1
 *
 * */


int my_setenv(const char *name, const char *value, int overwrite) {
    char *old_env_ptr = getenv(name);
    char **environ_backup_local = environ;
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
                char search_string[MAXBUFFERSIZE] = "\0";
                strncpy(search_string, name, strlen(name));
                strcat(search_string, "=");
                
                while (strstr(*environ, search_string) != *environ)
                    environ++;
                
                char *new_heap_env_ptr = (char *) malloc(sizeof(new_env_ptr));
                if (new_heap_env_ptr == NULL) {
                    environ = environ_backup_local;
                    return -1;
                }
                
                strncpy(new_heap_env_ptr, new_env_ptr, strlen(new_env_ptr));
                
                *environ = new_heap_env_ptr;

                environ = environ_backup_local;
                return 1;
            }
        }
    } else {
        int var_count = 0; 
         
        while (*environ != NULL) 
            environ++;
                 
        char *env_ptr; 

        int i;
        
        size_t name_size = strlen(name);
        size_t value_size = strlen(value);
        size_t heap_space_needed = name_size + value_size + 2;
        env_ptr = (char *) malloc(sizeof(char) * heap_space_needed);
        if (env_ptr == NULL) {
            environ = environ_backup_local;
            return -1;
        }
        
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
        environ = environ_backup_local;
        return 2;
    }            
}

int main(int argc, char *argv[]) {

    char **environ_backup_main = environ;
    
    printf("Original environment variables:\n\n");

    char **ep;
    for (ep = environ; *ep != NULL; ep++)
        puts(*ep);

    char env_key[50] = "HOME";
     
    char *old_ep = getenv(env_key);
    old_ep -= (strlen(env_key) + 1);
            // Saved for char **environ reset just before exiting main

    my_setenv("HOME", "NEW_HOME", 1); 
     
    printf("\n\n\n######################\n\n\n");
    printf("Changed 'HOME' environment variable to 'NEW_HOME':\n\n"); 

    for (ep = environ; *ep != NULL; ep++)
        puts(*ep);
  
    my_setenv("FAVE_MEAL", "TACO_BELL_BIG_BOX", 0);
 
    printf("\n\n\n######################\n\n\n");
    printf("Added environment variable FAVE_MEAL:\n\n"); 

    for (ep = environ; *ep != NULL; ep++)
        puts(*ep);

    ep--;
    free(*ep);
    *ep = NULL;

    printf("\n\n\n######################\n\n\n");
    printf("Removed FAVE_MEAL and freed malloc'd memory:\n\n"); 

    for (ep = environ; *ep != NULL; ep++)
        puts(*ep);

    char *new_ep_edit = getenv(env_key);
    new_ep_edit -= (strlen(env_key) + 1); 
    for (ep = environ; *ep != new_ep_edit; ep++) {
        environ++;
    }
   
    free(*ep);
    *ep = old_ep;
    environ = environ_backup_main;

    printf("\n\n\n######################\n\n\n");
    printf("Changed HOME environment variable back and free'd malloc'd memory:\n\n"); 

    for (ep = environ; *ep != NULL; ep++)
        puts(*ep);
    printf("\n");
    
    return EXIT_SUCCESS;
}
