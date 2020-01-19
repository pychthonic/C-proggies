#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define MAXBUFFERSIZE 81
#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)

/* My implemention of readv() function. It reads in 3 lines from stdin,
 * stores them in 3 iovector structs, then uses the standard library's
 * writev() function to print them directly to stdout to prove it
 * worked.
 * */

ssize_t my_readv(int fd, struct iovec *iov, int iovcnt) {
    int i;
    int j = 0;
    char byte_read;

    for (i=0; i<iovcnt; i++) {
        while (j < MAXBUFFERSIZE - 1) {
            byte_read = read(fd, iov[i].iov_base + j, 1);
            if ((byte_read == -1) || (byte_read != 1)) {
                return -1;
            } 
            
            if (*(char *) (iov[i].iov_base + j) == '\n') {
                break; 
            }

            j++;
        }

        *(char *)(iov[i].iov_base + j) = '\0'; 
        iov[i].iov_len = j;
        j = 0;
    }
}

int main(int argc, char *argv[]) {
  
    char str1[MAXBUFFERSIZE];
    char str2[MAXBUFFERSIZE];
    char str3[MAXBUFFERSIZE]; 
    
    struct iovec iovector[3];

    iovector[0].iov_base = str1;
    iovector[0].iov_len = strlen(str1);
    iovector[1].iov_base = str2;
    iovector[1].iov_len = strlen(str2);
    iovector[2].iov_base = str3;
    iovector[2].iov_len = strlen(str2);

    int fptr = open("/dev/fd/0", O_RDONLY);
    if (fptr == -1) {
        errExit("open(stdin, O_RDONLY)");
    }
    
    size_t bytes_read = my_readv(fptr, &iovector[0], 3);     
    if (bytes_read == -1) {
        close(fptr);
        errExit("my_readv");
    }
    
    if (close(fptr) == -1) {
        errExit("close(fptr)");
    } 
    
    size_t bytes_written = writev(STDOUT_FILENO, &iovector[0], 3);
    if (bytes_written == -1) {
        errExit("writev");
    }

    return EXIT_SUCCESS;
}
