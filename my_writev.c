#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)

/* My implemention of writev() function.
 * */

ssize_t my_writev(int fd, const struct iovec *iov, int iovcnt) {
    int i;
    int bytes_written;
    for (i=0; i<iovcnt; i++) {
        errno = 0; 
        bytes_written = write(fd, iov[i].iov_base, iov[i].iov_len);
        if ((bytes_written == -1) || (bytes_written != iov[i].iov_len)) {
            return -1;
        }
    }
}

int main(int argc, char *argv[]) {
  
    char str1[] = "First line of the file\n";
    char str2[] = "Second line of the file\n";
    int num1 = 77;
    float num2 = 7.7; 

    struct iovec iovector[4];

    iovector[0].iov_base = str1;
    iovector[0].iov_len = strlen(str1);
    iovector[1].iov_base = str2;
    iovector[1].iov_len = strlen(str2);
    iovector[2].iov_base = &num1;
    iovector[2].iov_len = sizeof(int);
    iovector[3].iov_base = &num2;
    iovector[3].iov_len = sizeof(float);

    errno = 0;
    int fptr = open("/dev/fd/1", O_WRONLY);
    if (fptr == -1) {
        errExit("open(stdout, O_WRONLY)");
    }
    
    my_writev(fptr, &iovector[0], 4);     

    errno = 0;
    if (close(fptr) == -1) {
        errExit("close(fptr)");
    } 
    
    return EXIT_SUCCESS;
}
