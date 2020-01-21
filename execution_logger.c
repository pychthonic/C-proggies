#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)
#define MAXBUFFERSIZE 4096

/* This program creates a log file called execution_log.txt which
 * leaves a log entry every time the binary is executed, with the
 * exact time the program was run.
 * */


int main(int argc, char *argv[]) {
    
    struct tm curr_time, *tm_ptr;
    char hr_str[3], min_str[3], sec_str[3], day_str[3], mon_str[3], yr_str[5]; 
    
    tm_ptr = &curr_time;

    int fptr = open("execution_log.txt", O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
    if (fptr == -1) {
        errExit("open log file");
    }

    long int epoch_time = time(0); 
    
    localtime_r(&epoch_time, tm_ptr);

    snprintf(hr_str, 3, "%02d", tm_ptr->tm_hour);
    snprintf(min_str, 3, "%02d", tm_ptr->tm_min);
    snprintf(sec_str, 3, "%02d", tm_ptr->tm_sec);
    snprintf(day_str, 3, "%02d", tm_ptr->tm_mday);
    snprintf(mon_str, 3, "%02d", tm_ptr->tm_mon + 1);
    snprintf(yr_str, 5, "%d", tm_ptr->tm_year + 1900);
   
    char log_entry[100] = "Executed on ";
    strcat(log_entry, mon_str);
    strcat(log_entry, "/");
    strcat(log_entry, day_str);
    strcat(log_entry, "/");
    strcat(log_entry, yr_str);
    strcat(log_entry, " at ");
    strcat(log_entry, hr_str);
    strcat(log_entry, ":");
    strcat(log_entry, min_str);
    strcat(log_entry, ":");
    strcat(log_entry, sec_str);
    strcat(log_entry, "\n");

    if (write(fptr, log_entry, strlen(log_entry)) == -1) {
        close(fptr);
        errExit("write");
    }

    if (close(fptr) == -1) {
        errExit("close");
    }

    return EXIT_SUCCESS;
}
