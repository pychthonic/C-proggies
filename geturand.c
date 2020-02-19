#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <inttypes.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)
#define MAXBUFFERSIZE 4096

/* This file opens /dev/urandom, which is a lot slower than
 * /dev/random, and prints a loop of the number it generates
 * until it hits 68.
 * */


int main(int argc, char *argv[]) {
  
    int fd = open("/dev/urandom", O_RDONLY);
    int8_t rand_byte = 0;
    while (rand_byte != 68) { 
        if (read(fd, &rand_byte, 1) == -1) {
            close(fd);
            errExit("read");
        }
        printf("rand_byte: %" PRIi8 "\n", rand_byte);
    }
    if (close(fd) == -1) {
        errExit("close");
    }
    return EXIT_SUCCESS;
}
