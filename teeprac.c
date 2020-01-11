#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_READ_SIZE 4096

/* This program uses a mix of system calls and library functions to take multiline 
 * input from stdin (use CTRL-D to signal end of input), asks user for name of new
 * file, then if the file does not exist, creates it, and writes the data entered
 * by the user into the new file. It then uses popen() to give details about the
 * newly created file. I'm making my way through "The Linux Programming Interface"
 * by Michael Kerrisk and this code is me implementing what I'm learning in the
 * book. */

int main(int argc, char *argv[]) {
    int fd_input, newfilefd;
    ssize_t bytecount_read, bytecount_written;
    char read_buffer[MAX_READ_SIZE];
    int total_bytes_read = 0;
    char message[256];
    
    errno = 0; 
    fd_input = open("/dev/tty", O_RDWR); 
    if (fd_input == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    
    strcpy(message, "Enter text to write to a file: \n"); 
    errno = 0;
    bytecount_written = write(fd_input, message, strlen(message));
    if (bytecount_written != strlen(message)) {
        perror("write");
        close(fd_input);
        exit(EXIT_FAILURE);
    }

    errno = 0;
    bytecount_read = read(fd_input, read_buffer, MAX_READ_SIZE);
    if (bytecount_read == -1) {
        perror("read");
        close(fd_input);
        exit(EXIT_FAILURE);
    }

    while (bytecount_read != 0) {
        total_bytes_read += bytecount_read; 
        errno = 0; 
        bytecount_read = read(fd_input, &read_buffer[total_bytes_read], MAX_READ_SIZE - total_bytes_read);
        if (bytecount_read == -1) {
            perror("read");
            close(fd_input); 
            exit(EXIT_FAILURE);
        }
    }
 
    char filename[256];
   
    strcpy(message, "Enter new filename: "); 
    errno = 0;
    bytecount_written = write(fd_input, message, strlen(message));
    if (bytecount_written != strlen(message)) {
         perror("write");
         exit(EXIT_FAILURE);
    }

    errno= 0;
    bytecount_read = read(fd_input, filename, 256);
    if (bytecount_read == -1) {
         perror("read");
         exit(EXIT_FAILURE);
    }
    filename[bytecount_read - 1] = '\0'; 

    errno = 0;
    newfilefd = open(filename, O_WRONLY | O_CREAT | O_TRUNC | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    if (newfilefd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    
    errno = 0;
    bytecount_written = write(newfilefd, read_buffer, total_bytes_read);
    if (bytecount_written != total_bytes_read) {
         perror("write");
         close(newfilefd);
         close(fd_input);
         exit(EXIT_FAILURE);
    }

    FILE *command_fptr;
    char command[256] = "ls -la";
    strcat(command, " "); 
    strcat(command, filename);

    errno = 0;
    command_fptr = popen(command, "r");
    if (command_fptr == NULL) {
        perror("popen");
        exit(EXIT_FAILURE);
    }

    errno = 0;
    if (fgets(read_buffer, 4096, command_fptr) == NULL) {
        perror("fgets");
        fclose(command_fptr);
        exit(EXIT_FAILURE);
    }
    
    errno = 0;
    bytecount_written = write(fd_input, read_buffer, strlen(read_buffer));
    if (bytecount_written != strlen(read_buffer)) {
        perror("write");
        fclose(command_fptr);
        close(fd_input);
        close(newfilefd);
        exit(EXIT_FAILURE);
    }

    errno = 0;
    if (close(newfilefd) == -1) {
        perror("close");
        close(fd_input);
        fclose(command_fptr);
        exit(EXIT_FAILURE);
    }

    errno = 0;
    if (close(fd_input) == -1) {
        perror("close"); 
        fclose(command_fptr); 
        exit(EXIT_FAILURE);
    }

    errno = 0;
    if (fclose(command_fptr) == -1) {
       perror("close");
       exit(EXIT_FAILURE);
    } 
    
    return EXIT_SUCCESS;
}    
