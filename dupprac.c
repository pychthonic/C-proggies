#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


/* This program tests dup(). It duplicates stderr, closes
 * stderr, opens a new fd on a text file, prints its new fd
 * which should be 2, closes both new file descriptors and
 * exits.
 * */


int main(int argc, char *argv[]) {
    int newfd1 = dup(2);
    printf("New fd for stderr: %d\n", newfd1);
   
    errno = 0; 
    if (close(2) == -1) {
        perror("close(2)");
        exit(EXIT_FAILURE);
    }

    errno = 0;
    int newfd2 = open("file.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (newfd2 == -1) {
        perror("open newfd2");
        close(newfd1);
        exit(EXIT_FAILURE);
    }    

    printf("newfd2 = %d\n", newfd2);

    errno = 0;
    if (close(newfd2) == -1) {
        perror("close newfd2");
        exit(EXIT_FAILURE);
    }

    errno = 0; 
    if (close(newfd1) == -1) {
        perror("close newfd1");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
