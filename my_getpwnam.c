#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>

#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)
#define MAXBUFFERSIZE 4096

/* My implementation of syscall getpwnam()
 * */

struct passwd *my_getpwnam(const char *name) {
    struct passwd *curr = getpwent();
    while (strcmp(curr->pw_name, name) != 0) { 
        curr = getpwent();
        if (curr == NULL) {
           return NULL;
        } else if (!strcmp(curr->pw_name, name)) {
            return curr;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char search_name[MAXBUFFERSIZE] = "\0";
    strncpy(search_name, argv[1], strlen(argv[1]));

    struct passwd *found_name_ptr = my_getpwnam(search_name);
    if (found_name_ptr == NULL) {
        printf("Name not found.\n");
        exit(EXIT_FAILURE);
    }

    printf("\nFound the following entry:\n");
    printf("\nName: %s\n", found_name_ptr->pw_name);
    printf("Password Hash: %s\n", found_name_ptr->pw_passwd);
    printf("uid: %u\n", found_name_ptr->pw_uid);
    printf("gid: %u\n", found_name_ptr->pw_gid);
    printf("User info: %s\n", found_name_ptr->pw_gecos);
    printf("Home directory: %s\n", found_name_ptr->pw_dir);
    printf("Shell: %s\n\n", found_name_ptr->pw_shell);


    return EXIT_SUCCESS;
}
