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

/* This program is a demo of pointers to pointers. It creates two ints
 * i and j, then two pointers to those ints ip1 and ip2, then an int
 * pointer pointer (a pointer that points to an int pointer) ipp, and
 * assigns the address of ip1 to ipp and prints out the addresses of
 * i and j, the values of ip1 and ip2 (which are the addresses of i and
 * j, respectively), the addresses of ip1 and ip2 (where ip1 and ip2
 * are located in virtual memory), the dereferenced value of ipp, and
 * the value of ipp. You can see that the dereferenced value of ipp is
 * the same address as is found in ip1, which is the address of i in
 * virtual memory. You can also see that the value of ipp is the
 * address of ip1 (&ip1).
 *
 * Here where the unexpected thing happened, for me at least.
 *
 * I changed the dereferenced value of ipp to ip2. What I thought this
 * would do is put the address of ip2 into ipp, that is, put the
 * address of ip2 into ipp. The value of ipp didn't change though. 
 * It just changed the value of ip1 so that it pointed at the same
 * address as ip2 pointed to. ipp stayed pointing to ip1. This seems
 * counter-intuitive since the assignment "*ipp = ip2" seems like
 * it should do just that - take ip2 and assign it to ipp.
 *
 * The same behavior can be observed when I changed the dereferenced
 * value of ipp (*ipp) to the address of k (&k). Instead of changing
 * anything inside the actual ipp pointer, what it did was it changed
 * the address inside the value of ip1 to the address of k.
 *
 * This made me realize I want to do more practice with pointers so
 * I can expect this kind of thing instead of being surprised by it,
 * especially since arrays of pointers to pointers are pretty common
 * in C code and Linux system programming. 
 *
 * */


int main(int argc, char *argv[]) {
    int **ipp;
    int i = 5, j = 6, k = 7;
    int *ip1 = &i, *ip2 = &j;
    ipp = &ip1;
     
    printf("\n  &i:  %p        &j:  %p\n", &i, &j);  
    printf(" ip1:  %p       ip2:  %p\n", ip1, ip2);
    printf("&ip1:  %p      &ip2:  %p\n", &ip1, &ip2); 
    printf("*ipp:  %p       ipp:  %p\n\n", *ipp, ipp);

    *ipp = ip2;

    printf("             *ipp = ip2;\n");
     
    printf("\n  &i:  %p        &j:  %p\n", &i, &j);  
    printf(" ip1:  %p       ip2:  %p\n", ip1, ip2);
    printf("&ip1:  %p      &ip2:  %p\n", &ip1, &ip2); 
    printf("*ipp:  %p       ipp:  %p\n\n", *ipp, ipp);

    *ipp = &k;

    printf("             *ipp = ip2;\n");
     
    printf("\n  &i:  %p        &j:  %p        &k: %p\n", &i, &j, &k);  
    printf(" ip1:  %p       ip2:  %p\n", ip1, ip2);
    printf("&ip1:  %p      &ip2:  %p\n", &ip1, &ip2); 
    printf("*ipp:  %p       ipp:  %p\n\n", *ipp, ipp);

    return EXIT_SUCCESS;
}
