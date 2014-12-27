/*
 * An attempt to build my first usefull program in C
 *
 * This is supposed to read a file and search in it
 * for lines beggining with "TODO" and list them in
 * order, printing something like:
 *
 *     On line 15:
 *         list files used untill now
 *     On line 122:
 *         Rewrite the function, using recursion
 * 
 * (Note that the notes are printed ignoring the "TODO"
 * strings. After all, this program is already about
 * printing TODOs, so no need to remaind it in every
 * f*cking line.)
 *
 */

// Constants
#define TODO "TODO:"
#define LINE_MAX_SIZE 512

// Required libraries
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main(int argc, char* argv[])
{
    if (argc < 2)
        printf("Filename not specified.");

    // Dummy file for testing
    FILE* fp = fopen(argv[1], "r");
    if (fp == NULL) {
        // Print error to stderr
        fprintf(stderr, "Failed to open file %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    // Search for the string TODO in current file, line by line
    // if found, print the whole line
    char buffer[LINE_MAX_SIZE];
    int line_num = 1;
    while (fgets(buffer, sizeof buffer, fp) != NULL) {
        if (strstr(buffer, TODO) != NULL) {
            printf("On line %d:\n", line_num);
            printf("\t%s", buffer);
        }
        ++line_num;
    }

    fclose(fp);
    return EXIT_SUCCESS;
}
