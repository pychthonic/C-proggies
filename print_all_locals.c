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

/* This program started out with me wanting to print out the content
 * byte-by-byte of all local variables in main in the order of their
 * addresses in virtual memory. It was tough to come up with a way
 * to do that without creating more variables, which would
 * recursively mess up the order of main's local variables. So what
 * I did was, I wrote a function called print_locals, put the
 * information I'd need to pass about main's local variables in
 * those variables themselves, passed them to print_locals, and had
 * print_locals algorithmically sort the array and then print out
 * the contents of the virtual memory byte by byte. Since main calls
 * print_locals, print_locals can access all of main's variables
 * via the pointers passed to print_locals from main.
 * */

void print_locals(unsigned char *addy_array[7], size_t size_array[7]) {
    char *char_ptr_holder; 
    int int_holder; 
    for (int i=0; i<7; i++) {
        for (int j=i+1; j<7; j++) {
            if (addy_array[i] > addy_array[j]) {
                char_ptr_holder = addy_array[i];
                addy_array[i] = addy_array[j];
                addy_array[j] = char_ptr_holder;
                int_holder = size_array[i];
                size_array[i] = size_array[j];
                size_array[j] = int_holder;
            }
        }
    }
    printf("\n");
    char_ptr_holder = addy_array[0];
    printf("    address        content (hex)\n");
    for (int i=0; i<7; i++) {
        for (int j=0; j<size_array[i]; j++){
            printf("0x%p       0x%02x\n", char_ptr_holder, *char_ptr_holder & 0xff);
            char_ptr_holder++;
        }
        printf("\n");
    } 
}
 
int main() {
    short i = 0x1234;
    char x = 1;
    long sn1 = 666420;
    long sn2 = 420666;
    unsigned int total_mem = sizeof(i) + sizeof(x) + sizeof(sn1) + sizeof(sn2);
    unsigned char *y[7] = {(unsigned char *) &i, (unsigned char *) &x, (unsigned char *) &sn1, (unsigned char *) &sn2, (unsigned char *) &total_mem, (unsigned char *) &y, (unsigned char *) &y};
    size_t size_array[7] = {sizeof(i), sizeof(x), sizeof(sn1), sizeof(sn2), sizeof(total_mem), sizeof(y), sizeof(size_t) * 7};
    
    y[6] = (unsigned char *) &size_array;    
    total_mem += (sizeof(y) + sizeof(size_array)); 

    printf("\nsizeof(short i)........%02zu....addy: %p...hex: 0x%x.....dec: %d\n", sizeof(i), &i, i, i); 
    printf("sizeof(char x).........%02zu....addy: %p...hex: 0x%2x........dec: %d\n", sizeof(x), &x, x, x); 
    printf("sizeof(long sn1).......%02zu....addy: %p...hex: 0x%lx....dec: %ld\n", sizeof(sn1), &sn1, sn1, sn1);
    printf("sizeof(long sn2).......%02zu....addy: %p...hex: 0x%lx....dec: %ld\n", sizeof(sn2), &sn2, sn2, sn2); 
    printf("sizeof(int total_mem)..%02zu....addy: %p...hex: 0x%x........dec: %d\n", sizeof(total_mem), &total_mem, total_mem, total_mem);    
    printf("sizeof(char array)...%02zu....addy: %p\n", sizeof(y), &y);
    printf("sizeof(size_t array).%02zu....addy: %p\n\n", sizeof(size_array), &size_array);
    
    print_locals(y, size_array);

    return EXIT_SUCCESS;
}
