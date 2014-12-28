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
#define TODO "TODO:"      // string to be searched
#define LINE_MAX_SIZE 700 // max bytes per line

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
    char *args[1]; // ARG1
    int fullpath;  // the -f flag
    char *outfile; // argument for -o
};

/*
 * Options, field 1 in ARGP
 *     fields: {NAME, KEY, ARG, FLAGS, DOC}
 */
static struct argp_option options[] = {
    {"fullpath", 'f', 0, 0, "Prints the full path of the file"},
    {"output", 'o', "OUTFILE", 0, "Output to OUTFILE instead of stdin"},
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
            arguments->fullpath = 1;
            break;
        case 'o':
            arguments->outfile = arg;
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

// Main program goes here
int main(int argc, char **argv)
{
    struct arguments arguments;
    
    // argument defaults
    arguments.outfile = NULL;
    arguments.fullpath = 0;

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

    // Dummy file for testing
    FILE *fp = fopen(full_filename, "r");
    if (fp == NULL) {
        // Print error to stderr
        fprintf(stderr, "Failed to open file %s\n", filename);
        return EXIT_FAILURE;
    }

    // Prints the filename
    fprintf(outstream, "File %s:\n\n", filename);

    // Search for the string TODO in current file, line by line
    // if found, print the whole line
    char buffer[LINE_MAX_SIZE];
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
