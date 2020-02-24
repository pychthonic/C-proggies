#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)
#define MAXBUFFERSIZE 4096

/* This program calls a function which returns its own
 * stack frame size.
 * */

int get_stack_frame_size(int main_sp, char stack_file_string[50]) {

    int stack_file = open(stack_file_string, O_RDONLY);
    if (stack_file == -1) {
        errExit("open");
    } 
 
    char read_file_buffer[300];
    int bytes_read = read(stack_file, read_file_buffer, 300);
     
    if (close(stack_file) == -1) {
        errExit("close");
    }
     
    char delim[] = " ";     
    char *charptr; 
    
    charptr = strtok(read_file_buffer, delim);
    for (int i=0; i<7; i++) {
       charptr = strtok(NULL, delim);
    }
     
    long int current_sp = strtol(charptr, NULL, 16);
     
    return main_sp - current_sp;
}

int main(int argc, char *argv[]) {
    
    pid_t current_pid = getpid();
    char pid_str[10];
    
    snprintf(pid_str, 10, "%d", current_pid);
    
    char stack_file_string[50] = "/proc/";
    strcat(stack_file_string, pid_str);
    strcat(stack_file_string, "/syscall");
 
    int stack_file = open(stack_file_string, O_RDONLY);
    if (stack_file == -1) {
        errExit("open");
    } 
    
    char read_file_buffer[MAXBUFFERSIZE];
    int bytes_read = read(stack_file, read_file_buffer, MAXBUFFERSIZE);
      
    if (close(stack_file) == -1) {
        errExit("close");
    }
  
    char delim[] = " ";     
    char *charptr; 
    
    charptr = strtok(read_file_buffer, delim);
    
    for (int i=0; i<7; i++) {
       charptr = strtok(NULL, delim);
    }
     
    long int current_sp = strtol(charptr, NULL, 16);
 
    int stack_frame_size = get_stack_frame_size(current_sp, stack_file_string);

    printf("Stack frame took up %d bytes.\n", stack_frame_size);   

    return EXIT_SUCCESS;
}
