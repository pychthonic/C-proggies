#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


/* Exercise 5-3. I spent about an hour trying to figure out why the
 * program was outputting "argv[3] is -a instead of --append or -a".
 * You can't just compare strings in C like you can in Python. Gotta
 * remember strcmp()! This program demonstrates why the atomicity
 * guaranteed by opening a file with the O_APPEND flag is necessary.
 * 
 * If you run:
 * 
 * ./atomicitydemo newfile.txt 1000 & ./atomicitydemo newfile.txt 1000 
 * 
 * The writes will overwrite each other, and you'll end up with a file
 * that's somewhere between 1000 and 2000 bytes long, depending on
 * how many times the two processes switch back and forth while
 * writing and overwriting each other.
 * 
 * If you run:
 * 
 * ./atomicitydemo newfile.txt 1000 -a & ./atomicity newfile.txt 1000 -a
 * 
 * The writes always append, and you'll get a file that's 2000 bytes
 * long. 
 * */


int main(int argc, char *argv[]) {
    if ((argc != 3) && (argc != 4)) {
        printf("argc is %d, instead of 3 or 4\n", argc);
        printf("Usage: %s filename <byte_count> [--append | -a]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    if ((argc == 4) && !strcmp(argv[3], "--append") && !strcmp(argv[3], "-a")) {
        printf("argv[3] is %s instead of --append or -a\n", argv[3]);
        printf("Usage: %s filename <byte_count> [--append | -a]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    errno = 0;
    int byte_count = strtol(argv[2], NULL, 10);
    if ((errno == EINVAL) || (errno == ERANGE)) {
        printf("Invalid number entered for argument <byte_count>\n"); 
        printf("Usage: %s filename <byte_count> [--append | -a]\n", argv[0]);
        exit(EXIT_FAILURE);
    } 
    
    int flags = O_WRONLY | O_CREAT;
    
    if ((argc == 4) && (!strcmp(argv[3], "--append") || !strcmp(argv[3], "-a"))) {
        flags |= O_APPEND;
    }
    
    int fptr = open(argv[1], flags, S_IRUSR | S_IWUSR);
    if (fptr == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    
    int i, byte_written;
     
    for (i=0; i<byte_count; i++) {
        if (write(fptr, "x", 1) == -1) {
             perror("write");
             close(fptr);
             exit(EXIT_FAILURE);
        }
        //printf("i = %d\n", i);  // Uncomment the printf if you want
                                  // to watch the two processes go back
                                  // and forth...on my machine, the first
                                  // process got about 20 writes in
                                  // before the other one interupted
                                  // and then they mostly go back and
                                  // forth for the rest of the 1000
                                  // writes.
        if (!(flags & O_APPEND)) {
            if (lseek(fptr, 0, SEEK_END) == -1) {
                perror("lseek");
                close(fptr);
                exit(EXIT_FAILURE);
            }
        }
    }

    if (close(fptr) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
