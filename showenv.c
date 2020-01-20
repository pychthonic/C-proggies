#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

extern char **environ;

#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)
#define MAXBUFFERSIZE 8192

/* This program opens /proc/<pid>/environ, reads its contents into a
 * char pointer array, and prints them to stdout. It also counts
 * how many NULL-terminated strings there are in char **environ,
 * and compares the two counts.
 * */


int main(int argc, char *argv[]) {
    char pid_str[10];
    pid_t proc_pid = getpid();
    snprintf(pid_str, 10, "%d", proc_pid); 
    
    char pathname[40] = "/proc/";
    strcat(pathname, pid_str);
    strcat(pathname, "/environ");
    
    int fptr = open(pathname, O_RDONLY);
    if (fptr == -1) {
        errExit("open");
    }

    char readbuf[MAXBUFFERSIZE];
   
    int bytes_read = read(fptr, readbuf, MAXBUFFERSIZE);
    if (bytes_read == -1 ) {
        close(fptr);
        errExit("read");
    }

    int envcount = 0;    
    int i; 
    for (i=0; i<bytes_read; i++) {
        if (readbuf[i] == '\0') {
           envcount++;
        }
    }
    printf("  environment variable count from %s: %d\n", pathname, envcount);

    int environ_count = 0;
    char **ep;
    for (ep=environ; *ep != NULL; ep++) {
        environ_count++;
    }
    printf("environment variable count from extern char **environ: %d\n\n", environ_count); 
    
    printf("Printing out environment variables:\n\n");

    char *env_ptr[envcount];
    
    env_ptr[0] = &readbuf[0];

    int j = 1; 
    for (i=0; i<bytes_read; i++) {
        if (readbuf[i] == '\0') {
            env_ptr[j] = &readbuf[i+1];
            j++;
        }
    }

    for (i=0; i<envcount; i++) {
        printf("%s\n", env_ptr[i]);
    }

    if (close(fptr) == -1) {
        errExit("close");
    } 
    
    return EXIT_SUCCESS;
}
