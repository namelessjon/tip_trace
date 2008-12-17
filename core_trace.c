/*
 * core_trace.c
 * Jonathan D. Stott <jonathan.stott@gmail.com>
 * Created: Tuesday, October 07, 2008 @ 10:04
 * Modified: Wednesday, December 17, 2008 @ 10:44
 *
 */

#include <stdio.h>
#include <getopt.h>
#include <string.h>

#include "tip_trace_binary.h"
#include "utils/string_list.h"
#include "helper.h"

static void print_help_text(char * progname);
// help text output

int main (int argc, char ** argv) {
    int c, file_set = 0;
 
    // dimensions of the sheet.
    int nx, ny;

    // timestep
    float dt;

    // isoline level
    float isoline;

    // output file
    FILE *output;

    // inpit file
    FILE *input;
    string_list_t *filenames;
    char filename[64];


    // filetype
    file_type_t type;

    // set some defaults
    nx = 375;
    ny = 375;
    isoline = -30;
    output = stdout;
    type = BINARY_FLOAT;
    dt = 1;
    filenames = new_string_list();

    while (1)
    {
        static struct option long_options[] =
        {
            /* These options don't set a flag.
               We distinguish them by their indices. */
            {"x-dim",       required_argument, 0, 'x'},
            {"y-dim",       required_argument, 0, 'y'},
            {"timestep",    required_argument, 0, 't'},
            {"file",        required_argument, 0, 'f'},
            {"isoline",     required_argument, 0, 'i'},
            {"output",      required_argument, 0, 'o'},
            {"type",        required_argument, 0, 'T'},
            {"help",        no_argument,       0, 'h'},
            {0, 0, 0, 0}
        };
        /* getopt_long stores the option index here. */
        int option_index = 0;

        c = getopt_long (argc, argv, "x:y:t:f:i:o:T:h",
                long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch (c)
        {

            case 'x':
                nx = atoi(optarg);
                break;

            case 'y':
                ny = atoi(optarg);
                break;

            case 't':
                dt = atof(optarg);
                break;

            case 'i':
                isoline = atof(optarg);
                break;

            case 'o':
                open(output, "w", optarg);
                break;

            case 'T':
                if (0==strcmp("float", optarg)) {
                    type = BINARY_FLOAT;
                    break;
                }
                if (0==strcmp("double", optarg)) {
                    type = BINARY_DOUBLE;
                    break;
                }
                if (0==strcmp("text", optarg)) {
                    type = TEXT;
                    break;
                }
                fprintf(stderr, "Unrecognised type.  Try float or double or text\n");
                exit(EXIT_FAILURE);
            case 'f':
                if ((optarg[0] == '-') && (optarg[1] == 0)) {
                    input = stdin;
                    break;
                }
                open(input, "r", optarg);
                file_set = 1;
                break;
            case 'h':
                print_help_text(argv[0]);
            case '?':
                print_help_text(argv[0]);
                /* getopt_long already printed an error message. */
                break;

            default:
                abort ();
        }
    }


    // if we didn't set a file, use the spare args from the commandline
    if (0 == file_set) {
        while (optind < argc) {
            string_list_push(filenames, argv[optind++]);
        }
    } else {
        // read the file
        while(EOF != fscanf(input, "%s", filename)) {
            string_list_push(filenames, filename);
        }
    }

    if (string_list_length(filenames) < 1) {
        fprintf(stderr, "No filenames found!\n");
        print_help_text(argv[0]);
        exit(EXIT_FAILURE);
    }

    // scan in all the filelist!

    process_file_list(nx, ny, dt, isoline, filenames, type, output);

    return 0;
} /* end of main() */

void print_help_text(char * progname) {
    fprintf(stderr, "Usage: %s [OPTIONS] [FILE, FILE, ...]\n", progname);
    fprintf(stderr, "Calculates the spiral tip trajectories in files...\n\n");
    fprintf(stderr, "  -x NX, --x-dim NX\n");
    fprintf(stderr, "                 The x dimension of the sheet (defaults to 375)\n");
    fprintf(stderr, "  -y NY, --y-dim NY\n");
    fprintf(stderr, "                 The y dimension of the sheet (defaults to 375)\n");
    fprintf(stderr, "  -t DT, --timestep DT\n");
    fprintf(stderr, "                 The timestep between successive frames of the sheet (defaults to 1)\n");
    fprintf(stderr, "  -i LEVEL, --isoline LEVEL\n");
    fprintf(stderr, "                 The isoline to track the tips alone (defaults to -30 mV)\n");
    fprintf(stderr, "  -o FILE, --output FILE\n");
    fprintf(stderr, "                 File to divert output to.  stdout otherwise.\n");
    fprintf(stderr, "  -f FILE, --file FILE\n");
    fprintf(stderr, "                 File to read framelist from.  - for stdin.  argv otherwise\n");
    fprintf(stderr, "  -T TYPE, --type TYPE\n");
    fprintf(stderr, "                 Type of input files.  One of float (binary floats), double (binary doubles) or text (whitespace delimited text).  Defaults to float.\n");
    fprintf(stderr, "  -h, --help\n");
    fprintf(stderr, "                 This help\n");
    exit(EXIT_FAILURE);
}
