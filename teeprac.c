#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_READ_SIZE 4096

/* This program uses a mix of system calls and glibc wrapper functions to take
 * multiline input from stdin (use CTRL-D to signal end of input), and asks the
 * user to provide a name for a new file.
 * 
 * It then uses getpid() to get the process id, opens /proc/<pid>/status, and
 * parses the file to get the umask number and prints it to the screen.
 * 
 * It then clears umask for the running process, and verifies that the umask has
 * been cleared by re-opening /proc/<pid>/status and re-parsing for the umask,
 * and prints the verified umask found in the file.
 * 
 * If the filename entered by the user a few steps back does not exist, the process
 * uses a system call to create it, and writes the data entered by the user into
 * the new file. It then uses popen() to give details about the newly created file,
 * so we can verify that the expected permissions were applied to it.
 * 
 * I'm making my way through "The Linux Programming Interface" by Michael Kerrisk
 * and this code is me implementing what I'm learning in the book. */

int main(int argc, char *argv[]) {
    int fd_input, newfilefd;
    ssize_t bytecount_read, bytecount_written;
    char read_buffer[MAX_READ_SIZE];
    int total_bytes_read = 0;
    char message[256];
    
    fd_input = open("/dev/tty", O_RDWR); 
    if (fd_input == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    
    strcpy(message, "Enter text to write to a file: \n"); 
    
    bytecount_written = write(fd_input, message, strlen(message));
    if (bytecount_written != strlen(message)) {
        perror("write");
        close(fd_input);
        exit(EXIT_FAILURE);
    }

    bytecount_read = read(fd_input, read_buffer, MAX_READ_SIZE);
    if (bytecount_read == -1) {
        perror("read");
        close(fd_input);
        exit(EXIT_FAILURE);
    }

    while (bytecount_read != 0) {
        total_bytes_read += bytecount_read; 
        bytecount_read = read(fd_input, &read_buffer[total_bytes_read], MAX_READ_SIZE - total_bytes_read);
        if (bytecount_read == -1) {
            perror("read");
            close(fd_input); 
            exit(EXIT_FAILURE);
        }
    }
 
    char filename[256];
   
    strcpy(message, "Enter new filename: "); 
    bytecount_written = write(fd_input, message, strlen(message));
    if (bytecount_written != strlen(message)) {
         perror("write");
         close(fd_input);
         exit(EXIT_FAILURE);
    }

    bytecount_read = read(fd_input, filename, 256);
    if (bytecount_read == -1) {
         perror("read");
         close(fd_input);
         exit(EXIT_FAILURE);
    }
    filename[bytecount_read - 1] = '\0'; 
   
    char proc_pid_str[10]; 
    char status_path[50] = "/proc/";
   
    snprintf(proc_pid_str, 10, "%d", (int) getpid());

    strcat(status_path, proc_pid_str);
    strcat(status_path, "/status");
   
    int procfptr = open(status_path, O_RDONLY);
    if (procfptr == -1) {
         perror("open procfptr");
         exit(EXIT_FAILURE);
    }
   
    char procfile_buf[100]; 
    int i = 0;
    int k = 0;
    int umask_found = 0;
    char line_string[100]; 
    char umask_search_string[] = "Umask:";
    char umask_str[5];
    int line_index = 0;
    char *found_string;
     
    while ((read(procfptr, &procfile_buf[i], 1) == 1) && (!umask_found)) {
        line_string[line_index] = procfile_buf[i];
        if (line_string[line_index] == '\n') {
            line_string[line_index] = '\0';
            found_string = strstr(line_string, umask_search_string);
            if (found_string != NULL) {
                for (k=0; k<4; k++) {
                    umask_str[k] = *(found_string+k+7);
                }
                umask_str[4] = '\0';
                umask_found = 1;
            }
            line_index = 0;
        } else {
            line_index++;
        }
        i++;
    }
    
    printf("umask retrieved from %s before calling umask(0) = %s\n", status_path, umask_str);
                            
    if (close(procfptr) == -1) {
        perror("open");
        close(fd_input);
        exit(EXIT_FAILURE);
    }

    umask(0);
  
    procfptr = open(status_path, O_RDONLY);
    if (procfptr == -1) {
         perror("open procfptr");
         exit(EXIT_FAILURE);
    }
    
    i = 0;
    k = 0;
    umask_found = 0;
    line_index = 0;
 
    while ((read(procfptr, &procfile_buf[i], 1) == 1) && (!umask_found)) {
        line_string[line_index] = procfile_buf[i];
        if (line_string[line_index] == '\n') {
            line_string[line_index] = '\0';
            found_string = strstr(line_string, umask_search_string);
            if (found_string != NULL) {
                for (k=0; k<4; k++) {
                    umask_str[k] = *(found_string+k+7);
                }
                umask_str[4] = '\0';
                umask_found = 1;
            }
            line_index = 0;
        } else {
            line_index++;
        }
        i++;
    }
    
    printf("umask retrieved from %s after calling umask(0) =  %s\n", status_path, umask_str);
                            
    if (close(procfptr) == -1) {
        perror("open");
        close(fd_input);
        exit(EXIT_FAILURE);
    }

    newfilefd = open(filename, O_WRONLY | O_CREAT | O_TRUNC | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    if (newfilefd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    
    bytecount_written = write(newfilefd, read_buffer, total_bytes_read);
    if (bytecount_written != total_bytes_read) {
         perror("write");
         close(newfilefd);
         close(fd_input);
         exit(EXIT_FAILURE);
    }
 
    FILE *command_fptr;
    char command[256] = "ls -la ";
    strcat(command, filename);

    command_fptr = popen(command, "r");
    if (command_fptr == NULL) {
        perror("popen");
        exit(EXIT_FAILURE);
    }

    if (fgets(read_buffer, 4096, command_fptr) == NULL) {
        perror("fgets");
        fclose(command_fptr);
        exit(EXIT_FAILURE);
    }
    
    bytecount_written = write(fd_input, read_buffer, strlen(read_buffer));
    if (bytecount_written != strlen(read_buffer)) {
        perror("write");
        fclose(command_fptr);
        close(fd_input);
        close(newfilefd);
        exit(EXIT_FAILURE);
    }

    if (close(newfilefd) == -1) {
        perror("close");
        close(fd_input);
        fclose(command_fptr);
        exit(EXIT_FAILURE);
    }

    if (close(fd_input) == -1) {
        perror("close"); 
        fclose(command_fptr); 
        exit(EXIT_FAILURE);
    }

    if (fclose(command_fptr) == -1) {
       perror("close");
       exit(EXIT_FAILURE);
    } 
    
    return EXIT_SUCCESS;
}    
