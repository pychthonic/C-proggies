#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)
#define MAXBUFFERSIZE 256

/* This file gets the pid of the shell two ways and prints them to show
 * they match. 1/ It creates a child process with popen() which calls
 * "ps | grep bash" on the underlying shell, then parses the output for
 * the first number and converts it to an int - this is the pid of the
 * shell. 2/ It uses getppid() to get the pid of the parent process,
 * which is the shell. */


int main(int argc, char *argv[]) {
    int shellpid1 = getppid();
   
    char popen_readbuf[MAXBUFFERSIZE]; 

    char command[] = "ps | grep bash";

    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        errExit("popen");
    } else {
        if (fgets(popen_readbuf, MAXBUFFERSIZE, fp) == NULL) {
            pclose(fp);
            errExit("fgets");
        }
    }

    char *endptr;
    int shellpid2 = strtol(popen_readbuf, &endptr, 10);

    printf("     shellpid from getppid(): %d\n", shellpid1);
    printf("shellpid from ps | grep bash: %d\n", shellpid2);

    if (pclose(fp)) {
        errExit("fclose");
    }

    return EXIT_SUCCESS;
}
