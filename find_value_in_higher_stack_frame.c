#include <stdio.h>

/* This program shows that stack frames can access
 * local variables declared in stack frames above
 * them. It declares a variable y inside main,
 * then calls find_y_in_main, which starts
 * searching from the address of its own local
 * variable z, incrementing the address of a
 * pointer to z until it finds 77. It varifies
 * it found the correct '77' by printing both
 * the value and the address of y in both main
 * and find_y_in_main.

void find_y_in_main() {
    char z = '$';
    void *char_ptr = &z;
    
    while (*(int *) char_ptr != 77) {
        char_ptr++;
    }
    printf("Found y:         &y = %p.     *(int *)char_ptr = %d\n", char_ptr, *(int *)char_ptr);
}

int main() {
    int y = 77;
    printf("Inside main:     &y = %p.                    y = %d\n", &y, y); 
    find_y_in_main();
    return 0;
}
