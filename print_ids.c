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
 * process. It then calls setuid(0), which asks the kernel to
 * escalate its privileges by setting its user id to 0. If the
 * binary compiled by this c code does not have its Set User ID bit
 * set (rwSr-xr-x) and the binary is not run as sudo, then the
 * setuid(0) call will fail and return 0. If you run the binary
 * as sudo ("sudo ./print_ids"), it will change all 4 user IDs to
 * 0 (on my machine it did, at least). You can change the owner of
 * the binary to root and set the Set User ID bit with these commands:
 *
 * sudo chown root print_ids
 * sudo chmod u=rws print_ids
 *
 * Then check that the bit was activated with:
 *
 * ls -lah | grep print_ids
 *
 * Then run the file without sudo:
 *
 * ./print_ids
 *
 * and it will change the effective user ID to 0, while keeping
 * the real user ID, real group ID, and effective group id the
 * same.
 *
 * If you follow these steps, I would recommend deleting the
 * binary from your system when you're done testing it out,
 * as keeping binaries owned by root with the set user ID bit
 * set is a security vulnerability - if someone manages to
 * log into your system as an unprivileged user they can
 * search for these files pretty easily and exploit them to
 * escalate their privileges on your system.
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
