#define _LARGEFILE64_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#define FILE_SIZE 10000000000

/* This program creates a 9.4 GB file full of random numbers called
 * bigfile.txt
 * I wouldn't recommend compiling and running it if you're low on
 * disk space. */


int main(int argc, char *argv[]) {
   
    errno = 0; 
    int fd_rand = open64("/dev/urandom", O_RDONLY);
    if (fd_rand == -1) {
        perror("open64 /dev/random");
        exit(EXIT_FAILURE);
    }
    
    char filename[] = "bigfile.txt";

    errno = 0; 
    int fd_new_file = open64(filename, O_WRONLY | O_CREAT,
                           S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    if (fd_new_file == -1) {
        perror("open new file");
        exit(EXIT_FAILURE);
    }

    char *randbuf = malloc(FILE_SIZE);   
    size_t bytes_read;
    size_t bytes_written;
    size_t total_bytes_read = 0;
    size_t total_bytes_written = 0; 

    errno = 0;
    while (total_bytes_read < FILE_SIZE) {
        
        printf("total_bytes_read = %lu. About to call read...\n", total_bytes_read);
        bytes_read = read(fd_rand, &randbuf[total_bytes_read], FILE_SIZE - total_bytes_read);
        printf("Called read. bytes_read = %lu. total_bytes_read = %lu\n", bytes_read, total_bytes_read);
        if (bytes_read == -1) {
            perror("read fd_rand");
            close(fd_rand);
            exit(EXIT_FAILURE);
        }
        
        bytes_written = write(fd_new_file, &randbuf[total_bytes_read], bytes_read);
        if (bytes_written != bytes_read) {
            perror("write to fd_new_file");
            close(fd_new_file);
            close(fd_rand);
            exit(EXIT_FAILURE);
        }

        total_bytes_written += bytes_written; 
        total_bytes_read += bytes_read;
    }
    
    if (total_bytes_read != FILE_SIZE) {
        perror("read fd_rand");
        close(fd_rand);
        close(fd_new_file); 
        exit(EXIT_FAILURE);
    }

    if (total_bytes_written != FILE_SIZE) {
        perror("total_bytes_written");
        close(fd_rand);
        close(fd_new_file);
        exit(EXIT_FAILURE);
    }

    errno = 0;
    if (close(fd_rand) == -1) {
        perror("close fd_rand");
        close(fd_new_file);
        exit(EXIT_FAILURE);
    }

    errno = 0;
    if (close(fd_new_file) == -1) {
        perror("close new file");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
