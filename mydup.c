#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>

#define errExit(msg) do {perror(msg); exit(EXIT_FAILURE);} while (0)

/* This program does a couple things. It tests every possible number
 * that can be an open file descriptor and dynamically allocates
 * memory for a linked list containing all the open file descriptors
 * of the process. It goes through the linked list and prints all
 * the file descriptors.
 * 
 * It also has my implementation of the dup() and dup2() functions
 * which is the solution for exercise 5-4 in The Linux Programming
 * Interface.
 * */

struct open_fd {
        int fd;
        struct open_fd *next;
};

int mydup(int fd) {
    errno = 0;
    int new_fd = fcntl(fd, F_DUPFD, 0);
    if (new_fd == -1) {
        perror("fcntl(fd, F_DUPFD, 0");
        return -1;
    } 
    return new_fd; 
}

int mydup2(int fd1, int fd2) {
    if (fcntl(fd1, F_GETFD) == -1) {
        errno = EBADF;
        perror("fcntl(fd1, F_GETFD)");
        return -1;
    }
    
    if (fd1 == fd2) {
        return fd1;
    }
    
    if (fcntl(fd2, F_GETFD) != -1) {
        if (close(fd2) == -1) {
            return -1;
        }
    }
    
    errno = 0;
    fd2 = fcntl(fd1, F_DUPFD, fd2);
    if (fd2 == -1) {
        perror("Error duplicating fd2");
        return -1;
    }
    
    return fd2;
}


struct open_fd *get_open_file_descriptors (int max_fd_count) {
    /* Creates a linked list of all open file descriptors and returns
     * a pointer to the first linked list struct. */

    int fd_count = 0;
    int i;
    for (i=0; i<=max_fd_count; i++) {
        if (fcntl(i, F_GETFD) != -1) {
            fd_count++;
        }
    }

    printf("Found %d open file descriptors.\n", fd_count);
    printf("Allocating memory for a linked list with %d entries.\n", fd_count);

    struct open_fd *fd_ll_ptr = malloc(sizeof(struct open_fd) * fd_count);

    for (i=0; i<=max_fd_count; i++) {
        if (fcntl(i, F_GETFD) != -1) {
            fd_ll_ptr->fd = i;
            fd_ll_ptr->next = fd_ll_ptr + sizeof(struct open_fd);
            fd_ll_ptr = fd_ll_ptr->next;
        }
    }

    fd_ll_ptr -= sizeof(struct open_fd) * fd_count;

    return fd_ll_ptr;
}
    

int main(int argc, char *argv[]) {
     
    struct rlimit max_fd_count;
    
    errno = 0;
    if (getrlimit(RLIMIT_NOFILE, &max_fd_count) == -1) {
        errExit("getrlimit"); 
    }

    printf("soft max fds: %ld\n", max_fd_count.rlim_cur);
    printf("hard max fds: %ld\n", max_fd_count.rlim_max);
    
    errno = 0; 
    int fd1 = open("/tmp/file1.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd1 == -1) {
        errExit("open /tmp/file1.txt");
    } 
    
    errno = 0;
    int fd2 = mydup(fd1);
    if (fd2 == -1) {
        close(fd1);
        errExit("mydup(fd1)");
    }

    errno = 0;
    int fd3 = mydup2(fd2, 55);
    if (fd3 == -1) {
        close(fd1);
        close(fd2);
        errExit("mydup2(fd2, 55)");
    }
    
    struct open_fd *fd_linked_list_ptr;   

    fd_linked_list_ptr = get_open_file_descriptors(max_fd_count.rlim_max); 
    
    int fd_count = 0; 

    while (fd_linked_list_ptr->next) {
        fd_count++;
        printf("Open fd: %d\n", fd_linked_list_ptr->fd);
        fd_linked_list_ptr = fd_linked_list_ptr->next;
    }
 
    fd_linked_list_ptr -= sizeof(struct open_fd) * fd_count; 
    
    free(fd_linked_list_ptr);

    if (close(fd1) == -1) {
        close(fd2);
        close(fd3);
        errExit("close fd1");
    }

    if (close(fd2) == -1) {
        close(fd3); 
        errExit("close fd2");
    }

    if (close(fd3) == -1) {
        errExit("close fd3");
    }

    return EXIT_SUCCESS;
}
