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

/* This process prints out the real user ID, the effective user ID,
 * the real group ID, and the effective group ID of the calling
 * process.
 * */


int main(int argc, char *argv[]) {
  
    uid_t real_user_id = getuid();
    uid_t effective_user_id = geteuid();
    gid_t real_group_id = getgid();
    gid_t effective_group_id = getegid();

    printf("\nreal user ID: %d\n", real_user_id);
    printf("effective user ID: %d\n", effective_user_id);
    printf("real group ID: %d\n", real_group_id);
    printf("effective group id: %d\n", effective_group_id);

    printf("\ncalling setuid(0)...\n");

    int ret = setuid(0);
    if (ret == 0) {
        printf("\nsetuid(0) returned 0, so uid should be 0 aka root now..");
    } else if (ret == -1) {
        printf("\nsetuid(0) returned -1, so uid should be the same as it was before..\n");
    }
    
    printf("\nLet's check.\n\n"); 
    printf("real user ID: %d\n", real_user_id);
    printf("effective user ID: %d\n", effective_user_id);
    printf("real group ID: %d\n", real_group_id);
    printf("effective group id: %d\n\n", effective_group_id);

   

    return EXIT_SUCCESS;
}
