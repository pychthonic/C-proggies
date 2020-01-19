#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)

/* Exercise 5-6. This was easy. 
 * 
 * The created file goes from
 * 
 * Hello,
 * 
 * to
 * 
 * Hello,world
 * 
 * to
 * 
 * HELLO,world
 * 
 * to
 * 
 * Giddayworld
 *
 * Real interesting stuff.
 *
 * */


int main(int argc, char *argv[]) {
    int fd1 = open("/tmp/file.txt", O_RDWR | O_CREAT | O_TRUNC,
                   S_IRUSR | S_IWUSR);
    if (fd1 == -1) {
        errExit("open /tmp/file.txt");
    }
    
    int fd2 = dup(fd1);
    if (fd2 == -1) {
        errExit("dup(fd1)");
        close(fd1);
    } 
     
    int fd3 = open("/tmp/file.txt", O_RDWR | O_CREAT | O_TRUNC,
                   S_IRUSR | S_IWUSR);
    if (fd2 == -1) {
        close(fd1);
        close(fd2); 
        errExit("open /tmp/file.txt");
    }
    
    int bytes_written = write(fd1, "Hello,", 6);
    if (bytes_written == -1) {
        close(fd1);
        close(fd2);
        close(fd3);
        errExit("write");
    }
     
    bytes_written = write(fd2, "world", 6);
    if (bytes_written == -1) {
        close(fd1);
        close(fd2);
        close(fd3);
        errExit("write");
    }

    if (lseek(fd2, 0, SEEK_SET) == -1) {
        close(fd1);
        close(fd2);
        close(fd3);
        errExit("lseek");
    }

    bytes_written = write(fd1, "HELLO,", 6);
    if (bytes_written == -1) {
        close(fd1);
        close(fd2);
        close(fd3);
        errExit("write");
    }
 
    bytes_written = write(fd3, "Gidday", 6);
    if (bytes_written == -1) {
        close(fd1);
        close(fd2);
        close(fd3);
        errExit("write");
    }

    return EXIT_SUCCESS;
}
