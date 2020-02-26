#include <stdio.h>
#include <stdlib.h>

/* This program changes values that appear in the heap. On
 * the system it was tested on, it was only able to change
 * values up to 2 ints worth of memory space above the
 * malloc'ed memory space. On the other hand, descending
 * memory addresses were able to be modified and printed
 * up to 168 ints worth of data below the malloc'ed addy.
*/

int main() {
    int *i = malloc(sizeof(int) * 3);
    *(i) = 50;
    *(i + 1) = 51;
    *(i + 2) = 52;
    *(i + 3) = 53; 
    *(i + 4) = 54; 
    *(i + 5) = 55;
    
    int count = 1;    
    
    for (int g=0; g<7; g++) {
        if (*(i + g) == 0) {
            *(i + g) = count;
            count += 1;
        }
    }

    printf("\n");
    count = 1;

    for (int j=0; j<20; j++) {
        printf("%p || i + %03d || dec: %.10d || hex: 0x%08x\n", i+j, j, *(i+j), *(i+j));
    }

    printf("\nGoing downwards...\n\n");
    
    count = 10;
    for (int g=0; g>-169; g--) {
        *(i+g) = count;
        count += 1;
    }
    
    for (int j=0; j>-169; j--) {
        printf("%p || i - %03d || dec: %.10d || hex: 0x%08x\n", i-j, -j, *(i+j), *(i+j));
    }
    
    printf("\n");
    
    return 0;
}
