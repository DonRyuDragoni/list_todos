/*
 * This is supposed to read a file and search in it 
 * for lines containing "TODO:" and list them in
 * order, printing something like:
 *
 *     File dummy_file.txt:
 *
 *     On line 15:
 *         TODO: list files used untill now
 *     On line 122:
 *        cout << "ERR22, abort"; // TODO: fix 
 * 
 * Originally, only lines starting with the TODO
 * string were supposed to be printed, but this could
 * be considered a feature too, so lei it be like this.
 *
 * For now, the program only works for one file at a time;
 * for me, this is fine and ideal, but it may change.
 *
 */

// Required libraries
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"parser.h"

// Constants
#define TODO "TODO:" // string to be searched

// scans the whole file for inconsistent lines
// (inconsisted lines = scanned buffer with no '\n' and not in the enf of file)
int check_buffer_size(FILE *fp, int buffer_size)
{
    char buffer[buffer_size];
    while (fgets(buffer, sizeof buffer, fp) != NULL) {
        if (strstr(buffer, "\n") == NULL && fgets(buffer, sizeof buffer, fp) != NULL)
            return 1;
    }
    rewind(fp);
    return 0;
}

// Main program goes here
int main(int argc, char **argv)
{
    struct arguments arguments;
    
    // argument defaults
    arguments.outfile = DEFAULT_PRINT_OUTFILE;
    arguments.fullpath = DEFAULT_SHOW_FULLPATH;
    arguments.line_size = DEFAULT_LINE_SIZE; 
    arguments.ignore_warnings = DEFAULT_IGNORE_WARNINGS;

    // let the magic begin
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    // change arguments.args[0] into the full name of the file
    char *full_filename = realpath(arguments.args[0], NULL);
    // if asked, print the full path of the file, instead of just its name
    char *filename;
    if (arguments.fullpath)
        filename = full_filename;
    else
        filename = arguments.args[0];

    // send the output to outfile, if asked
    FILE *outstream;
    if (arguments.outfile)
        outstream = fopen(arguments.outfile, "w");
    else
        outstream = stdout;

    // Open the file and check existence
    FILE *fp = fopen(full_filename, "r");
    if (fp == NULL) {
        // Print error to stderr
        fprintf(stderr, "Failed to open file %s\n", filename);
        return EXIT_FAILURE;
    }

    // if ignore-warnings is off, check the buffer size against
    // the file; if some warning is found, close the file, print
    // error message and exit
    char buffer[arguments.line_size];
    if (!arguments.ignore_warnings && check_buffer_size(fp, arguments.line_size)) {
        fprintf(outstream, "Warning: buffer size may be too small.\nConsider changing it with the -s flag (default size is %d)\n", DEFAULT_LINE_SIZE);
        fclose(fp);
        return EXIT_FAILURE;
    }

    // Prints the filename
    fprintf(outstream, "File %s:\n\n", filename);

    // Search for the string TODO in current file, line by line
    // if found, print the whole line
    int line_num = 1;
    while (fgets(buffer, sizeof buffer, fp) != NULL) {
        if (strstr(buffer, TODO) != NULL) {
            fprintf(outstream, "On line %d:\n", line_num);
            fprintf(outstream, "\t%s", buffer);
        }
        ++line_num;
    }

    fclose(fp);
    return EXIT_SUCCESS;
}
