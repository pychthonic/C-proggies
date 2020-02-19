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

/* This program opens /dev/urandom, and prints 2-byte hexies
 * until it gets the decimal number 26985.
 * */


int main(int argc, char *argv[]) {
  
    int fd = open("/dev/urandom", O_RDONLY);
    uint16_t rand_2bytes = 0;
    if (read(fd, &rand_2bytes, 2) == -1) {
        close(fd);
        errExit("read");
    }
    
    while (rand_2bytes != 26985) { 
        if (read(fd, &rand_2bytes, 2) == -1) {
            close(fd);
            errExit("read");
        }
        printf("rand_2bytes: %04x\n", rand_2bytes);
    }
    if (close(fd) == -1) {
        errExit("close");
    }
    return EXIT_SUCCESS;
}
