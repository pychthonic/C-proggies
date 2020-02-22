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

/* 
 * */

int func1(int a, int b) {
    int c = a + b;
    return c;
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
    
    char read_file_buffer_backup[MAXBUFFERSIZE];
    strncpy(read_file_buffer_backup, read_file_buffer, MAXBUFFERSIZE);

    
      
    char delim[] = " ";     
    char *charptr; 
    printf("%s\n", read_file_buffer);
    int count = 1;
    charptr = strtok(read_file_buffer, delim);
/*    
    for (int i=1; i<28; i++) {
        charptr = strtok(NULL, delim);
        printf("%d: charptr = %s\n", count, charptr);
        count++;
    }
 */   
    long int current_sp = strtol(charptr, NULL, 10);

    printf("stack pointer addy: %lx\n", (unsigned long) current_sp);
    
    if (close(stack_file) == -1) {
        errExit("close");
    }



    return EXIT_SUCCESS;
}
