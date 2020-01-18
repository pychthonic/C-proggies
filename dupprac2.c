#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


/* This program tests dup(). It opens a text file called /tmp/file.txt,
 * duplicates its fd, writes to it using both file descriptors, and
 * prints the file descriptor offsets as it goes along to show that 
 * the descriptors point to the same entries in the kernel's file
 * table, as per the dup man page.
 * */


int check_offsets(int fd1, int fd2) {
    off_t off1 = lseek(fd1, 0, SEEK_CUR);
    off_t off2 = lseek(fd2, 0, SEEK_CUR);
    
    printf("offset1: %ld, offset2: %ld\n", off1, off2); 
    
    if (off1 != off2) {
        printf("File offsets for fd1 and fd2 don't match.\n");
        return -1;
    } else {
        printf("File offsets for fd1 and fd2 are the same\n");
        return 1;
    } 
}

int main(int argc, char *argv[]) {

    errno = 0;
    int newfd1 = open("/tmp/file.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (newfd1 == -1) {
        perror("open newfd1");
        exit(EXIT_FAILURE);
    }    

    printf("newfd1 = %d\n", newfd1);
    int newfd2 = dup(newfd1);
    printf("newfd2, which points to same open file description as newfd1: %d\n", newfd2);

    if (check_offsets(newfd1, newfd2) == -1) {
        close(newfd1);
        close(newfd2);
        fprintf(stderr, "File offsets don't match. Exiting.\n");
        exit(EXIT_FAILURE);
    }

    int bytes_written = write(newfd1, "aaaaa", 5); 
    printf("Checking offsets after writing 5 characters to newfd1\n");
    
    if (check_offsets(newfd1, newfd2) == -1) {
        close(newfd1);
        close(newfd2);
        fprintf(stderr, "File offsets don't match. Exiting.\n");
        exit(EXIT_FAILURE);
    }

    bytes_written = write(newfd2, "bbbbb", 7);     
    printf("Checking offsets after writing 7 characters to newfd2\n");

    if (check_offsets(newfd1, newfd2) == -1) {
        close(newfd1);
        close(newfd2);
        fprintf(stderr, "File offsets don't match. Exiting.\n");
        exit(EXIT_FAILURE);
    }

    errno = 0;
    if (close(newfd2) == -1) {
        perror("close newfd2");
        close(newfd1); 
        exit(EXIT_FAILURE);
    }

    errno = 0; 
    if (close(newfd1) == -1) {
        perror("close newfd1");
        exit(EXIT_FAILURE);
    }

    printf("We're golden.\n");

    return EXIT_SUCCESS;
}
