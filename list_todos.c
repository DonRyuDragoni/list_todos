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

// Constants
#define TODO "TODO:" // string to be searched
// Default behavior of the program
#define DEFAULT_LINE_SIZE 700
#define DEFAULT_SHOW_FULLPATH 0
#define DEFAULT_PRINT_OUTFILE NULL
#define DEFAULT_IGNORE_WARNINGS 0

// Required libraries
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<argp.h>

// program version
const char *argp_program_version = "list_todos 1.1";
// email for bug fixes
const char *argp_program_bug_address = "<dragoni.ryu@gmail.com>";

// struct to communicate with parse_opt
struct arguments {
    char *args[1]; // ARG1, the name of the file
    int fullpath;  // the -f flag, show or not the full path
    int line_size; // argument for -s, change default size of lines
    char *outfile; // argument for -o, file to be printed in
    int ignore_warnings;  // the -i flag, ignore or not warnings
};

/*
 * Options, field 1 in ARGP
 *     fields: {NAME, KEY, ARG, FLAGS, DOC}
 */
static struct argp_option options[] = {
    {"fullpath", 'f', 0, 0, "Prints the full path of the file"},
    {"line-size", 's', "LINESIZE", 0, "Change line (buffer) size, in bytes"},
    {"output", 'o', "OUTFILE", 0, "Output to OUTFILE instead of stdin"},
    {"ignore-warnings", 'i', 0, 0, "Ignore buffer size checking and run anyway; please note this leads to unknown behavior"},
    {0}
};

/*
 * Parser, field 2 in ARGP
 *     fields: {KEY, ARG, STATE}
 */
static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;

    switch(key) {
        case 'f':
            // swap the behavior of the fullpath flag
            if (DEFAULT_SHOW_FULLPATH == 1)
                arguments->fullpath = 0;
            else
                arguments->fullpath = 1;
            break;
        case 'o':
            arguments->outfile = arg;
            break;
        case 's':
            arguments->line_size = atoi(arg);
            break;
        case 'i':
            // swap the behavior of the warnings flag
            if (DEFAULT_IGNORE_WARNINGS == 1)
               arguments->ignore_warnings = 0;
            else
               arguments->ignore_warnings = 1; 
            break;
        case ARGP_KEY_ARG:
            if(state->arg_num >= 1)
                argp_usage(state);
            arguments->args[state->arg_num] = arg;
            break;
        case ARGP_KEY_END:
            if (state->arg_num < 1)
                argp_usage(state);
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

/*
 * Documentation, field 3 in ARGP
 *     Description of the non-option command-line arguments that we accept.
 */
static char args_doc[] = "FILENAME";

/*
 * Program documentation, field 4 in ARGP
 *     Program documentation.
 */
static char doc[] = "list_todos -- A program to list all TODO notes in a file.";

// The ARGP structure
static struct argp argp = {options, parse_opt, args_doc, doc};

// scans the whole file for inconsistent lines
// (inconsisted lines = scanned buffer with no '\n')
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
