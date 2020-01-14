#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


/* This file first creates a file and adds some data to it.
 * It then closes it, reopens it with the O_APPEND flag,
 * seeks to the beginning of the file before writing some
 * more data. Then we can check where the data ends up.
 * */


int main(int argc, char *argv[]) {
    char filename[] = "newfile.txt";
    
    errno = 0;
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC,
                  S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    
    char line_content[] = "Here's some content.\n"; 
    size_t bytes_written;
    int i; 
    for (i=0; i<10; i++) {
        errno = 0; 
        bytes_written = write(fd, line_content, sizeof(line_content) - 1);
        if (bytes_written != sizeof(line_content) - 1) {
            perror("write");
            close(fd);
            exit(EXIT_FAILURE);
        }
    }

    errno = 0;
    if (close(fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    errno = 0;
    int new_fd = open(filename, O_RDWR | O_APPEND);
    if (new_fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    errno = 0;
    if (lseek(new_fd, 0, SEEK_SET) == -1) {
        perror("lseek");
        close(new_fd);
        exit(EXIT_FAILURE);
    }
    
    errno = 0; 
    char more_content[] = "Did this data overwrite anything?\n";
    bytes_written = write(new_fd, more_content, sizeof(more_content) - 1);
    if (bytes_written != sizeof(more_content) - 1) {
        perror("write");
        close(new_fd);
        exit(EXIT_FAILURE);
    }

    errno = 0;
    if (close(new_fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }
    
    return EXIT_SUCCESS;
}
