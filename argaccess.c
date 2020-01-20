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

/* This program shows a cool way to access the command line arguments
 * from functions outside of main(). It opens /proc/<pid>/cmdline,
 * which holds all the command line arguments for the calling process
 * in the form of a stream of NULL-terminated strings. It counts the
 * arguments and assigned the count to my_argc. It then creates an
 * array of char pointers to the beginning of each argument,
 * essentially duplicating the functionality of the char *argv[]
 * array.
 * */

void function() {
    char readbuf[MAXBUFFERSIZE]; 
    pid_t pid = getpid();
     
    char pid_str[10];
    snprintf(pid_str, 10, "%d", (int)getpid());
    
    char pathname[40] = "/proc/";
    strcat(pathname, pid_str);
    strcat(pathname, "/cmdline");

    int fptr = open(pathname, O_RDONLY);
    if (fptr == -1) {
        errExit("open");
    }

    int bytes_read = read(fptr, readbuf, MAXBUFFERSIZE);
    if (bytes_read == -1) {
        close(fptr);
        errExit("read");
    }
 
    if (close(fptr) == -1) {
        errExit("close");
    }    
    
    int i; 
    int my_argc = 0; 
    for (i=0; i<bytes_read; i++) {
        if (readbuf[i] == '\0') {
            my_argc++;
        }
    }

    char *my_argv[my_argc];
  
    my_argv[0] = &readbuf[0]; 
    int j = 1;

    for (i=0; i<bytes_read - 1; i++) {
        if (readbuf[i] == '\0') {
            my_argv[j] = &readbuf[i+1];
            j++;
        }
    }

    printf("my_argc = %d\n", my_argc);

    printf("Testing my_argv string array:\n");
    
    for (i=0; i<my_argc; i++) {
        printf("my_argv[%d] = %s\n", i, my_argv[i]);
    }
}


int main(int argc, char *argv[]) {
  
    function();

    return EXIT_SUCCESS;
}
