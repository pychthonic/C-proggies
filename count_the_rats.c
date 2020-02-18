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

/* This program isn't the best organized, in that it
 * juggles a few plates at once instead of focusing
 * on one task at a time. It reads rats.txt and while
 * counting how many R's, A's, and T's it finds, it
 * counts how many times the string "RAT" is found,
 * including when it is spread across two lines.
 * It also makes sure rats.txt is exactly 500 lines
 * long, that there are 100 characters per line,
 * and that the lines are made up only of R's, A's,
 * T's, and X's.
 * */


int main(int argc, char *argv[]) {
    int fp = open("rats.txt", O_RDONLY);
    if (fp == -1) {
        errExit("open");
    }
    
    int char_count_in_line = 0; 
    int line_count = 0;
    char c;
    int r_count = 0;
    int a_count = 0;
    int t_count = 0;
    int x_count = 0;
    int rat_count = 0;
    int rewind_count;
    
    while (read(fp, &c, 1) != 0) {
        char_count_in_line++;
        if (c == '\n') {
            if (char_count_in_line != 101)
                errExit("character count in line != 100");
            char_count_in_line = 0;
            line_count++;
            continue;
        }
        rewind_count = 0;     
        if (c == 'R') {
            r_count++;
            rewind_count++;
            if (read(fp, &c, 1) == 0) {
                if (lseek(fp, -rewind_count + 1, SEEK_END) == -1)
                    errExit("lseek"); 
                continue;
            }
            if (c == '\n') { 
                rewind_count++;
                if (read(fp, &c, 1) == 0) {
                    if (lseek(fp, -rewind_count + 1, SEEK_END) == -1)
                        errExit("lseek");
                    continue;
                }
            }
            if (c == 'A') {
                rewind_count++;
                if (read(fp, &c, 1) == 0) {
                    if (lseek(fp, -rewind_count + 1, SEEK_END) == -1)
                        errExit("lseek");
                    continue;
                }
                if (c == '\n') {
                    rewind_count++; 
                    if (read(fp, &c, 1) == 0) {
                        if (lseek(fp, -rewind_count + 1, SEEK_END) == -1)
                            errExit("lseek");
                        continue;
                    }
                }
                if (c == 'T') {
                    rat_count += 1;
                }
            }
            if (lseek(fp, -rewind_count, SEEK_CUR) == -1)
                errExit("lseek");
        }
        else if (c == 'A')
            a_count++;
        else if (c == 'T')
            t_count++;
        else if (c == 'X')
            x_count++;
        else {
            fprintf(stderr, "%c != 'R', 'A', 'T', or 'X'", c);
            return -1;
        }
    }
    
    if (line_count != 500) {
        close(fp); 
        errExit("line count != 500");
    }

    if (close(fp) == -1) {
        errExit("close fp");
    }

    printf("Found %d R's.\n", r_count);
    printf("Found %d A's.\n", a_count);
    printf("Found %d T's.\n", t_count);
    printf("Found %d X's.\n", x_count);
    
    printf("\nFound %d rats.\n", rat_count);

    return EXIT_SUCCESS;
}
