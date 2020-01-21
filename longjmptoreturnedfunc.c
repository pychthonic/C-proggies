#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <setjmp.h>

#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)
#define MAXBUFFERSIZE 4096

/* This program shows what happens when we try to longjmp()
 * back into a function that's already returned. It will give a
 * seg fault, since it's telling the program to access a stack
 * frame that's been torn down.
 * */

static jmp_buf env;

static void func1() {
    setjmp(env);
}

int main(int argc, char *argv[]) {
    func1();

    longjmp(env, 1);    // whoops!

    return EXIT_SUCCESS;
}
