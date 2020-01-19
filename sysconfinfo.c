#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)

/* This program prints out a bunch of information about the system
 * it's running on.
 * */



int main(int argc, char *argv[]) {

    int max_args = sysconf(_SC_ARG_MAX);
    printf("\nMaximum # of bytes that can be passed to exec() ");
    printf("functions via argv and environ arguments  = %d\n\n", max_args);

    int max_child_processes = sysconf(_SC_CHILD_MAX);
    printf("Maximum # of simulataneous processes per user id: %d\n\n", max_child_processes);

    int max_login_len = sysconf(_SC_LOGIN_NAME_MAX);
    printf("Maximum # of characters in login name, including ");
    printf("terminating null byte: %d\n\n", max_login_len);

    int max_files = sysconf(_SC_OPEN_MAX);
    printf("Maximum # of files that a process can have open ");
    printf("at any given time: %d\n\n", max_files);
    
    int page_size = sysconf(_SC_PAGE_SIZE);
    printf("Size of a page in bytes: %d\n\n", page_size);

    int max_streams = sysconf(_SC_STREAM_MAX);
    printf("Max simultaneous streams open by a process: %d\n\n", max_streams);


    return EXIT_SUCCESS;
}
