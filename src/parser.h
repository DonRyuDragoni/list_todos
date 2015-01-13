/*
 * parser.h
 *
 * To be imported by the main file, this uses the GNU argp.h for command-line
 * arguments parsing, so it behaves more naturally (e.g. --help command, or
 * printing the usage when called without arguments).
 *
 * Maybe defining everything in a header is not the best practice, but this
 * is how i got it to work.
 *
 */

#ifndef PARSER_H_8380A159_48C3_4BE8_9E90_3C7D7B0F1F28
#define PARSER_H_8380A159_48C3_4BE8_9E90_3C7D7B0F1F28

// Required libraries
#include <argp.h>

// Default behavior of the program
#define DEFAULT_LINE_SIZE 700      // buffer to read line by line
#define DEFAULT_SHOW_FULLPATH 0    // show fullpath or only filename
#define DEFAULT_PRINT_OUTFILE NULL // print to outfile or to stdout
#define DEFAULT_FILECOUNT 1        // display or not filecount before each filename
#define DEFAULT_IGNORE_WARNINGS 0  // ignore warnings and run the program anyway

// program version
const char *argp_program_version = "list_todos 1.2";
// email for bug fixes
const char *argp_program_bug_address = "<dragoni.ryu@gmail.com>";

// struct to communicate with parse_opt
struct arguments {
    int fullpath;  // the -f flag, show or not the full path
    int line_size; // argument for -s, change default size of lines
    char *outfile; // argument for -o, file to be printed in
    int ignore_warnings;  // the -i flag, ignore or not warnings
    int filecount; // the -c flag, display or not count of files
    int filenum; // number of files listed as arguments
    char *args[]; // name of the files to be opened
};

/*
 * Options, field 1 in ARGP
 *     fields: {NAME, KEY, ARG, FLAGS, DOC}
 */
static struct argp_option options[] = {
    {"filecount",       'c', 0,          0, "Do not display the file count before every file name"},
    {"fullpath",        'f', 0,          0, "Prints the full path of the file"},
    {"ignore-warnings", 'i', 0,          0, "Ignore buffer size checking and run anyway; please note this leads to unknown behavior"},
    {"output",          'o', "OUTFILE",  0, "Output to OUTFILE instead of stdout"},
    {"line-size",       's', "LINESIZE", 0, "Change line (buffer) size, in characters"},
    {0}
};

/*
 * Parser, field 2 in ARGP
 *     fields: {KEY, ARG, STATE}
 */
static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;

    switch(key) {
        case 'c':
            // swap the behavior of the filecount flag
            if (DEFAULT_FILECOUNT == 1)
                arguments->filecount = 0;
            else
                arguments->filecount = 1;
            break;
        case 'f':
            // swap the behavior of the fullpath flag
            if (DEFAULT_SHOW_FULLPATH == 1)
                arguments->fullpath = 0;
            else
                arguments->fullpath = 1;
            break;
        case 'i':
            // swap the behavior of the warnings flag
            if (DEFAULT_IGNORE_WARNINGS == 1)
               arguments->ignore_warnings = 0;
            else
               arguments->ignore_warnings = 1; 
            break;
        case 'o':
            arguments->outfile = arg;
            break;
        case 's':
            arguments->line_size = atoi(arg);
            break;
        
        case ARGP_KEY_ARG:
            arguments->filenum = state->arg_num + 1;
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
static char args_doc[] = "FILENAME(S)";

/*
 * Program documentation, field 4 in ARGP
 *     Program documentation.
 */
static char doc[] = "List TODO notes in file(s).";

// The ARGP structure
static struct argp argp = {options, parse_opt, args_doc, doc};

#endif // PARSER_H_8380A159_48C3_4BE8_9E90_3C7D7B0F1F28