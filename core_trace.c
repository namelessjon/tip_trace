/*
 * core_trace.c
 * Jonathan D. Stott <jonathan.stott@gmail.com>
 * Created: Tuesday, October 07, 2008 @ 10:04
 * Modified: Monday, October 13, 2008 @ 12:00
 *
 */

#include <stdio.h>
#include <getopt.h>
#include <string.h>

#include "tip_trace_binary.h"
#include "utils/string_list.h"
#include "helper.h"

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
            case '?':
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
        exit(EXIT_FAILURE);
    }

    // scan in all the filelist!

    process_file_list(nx, ny, dt, isoline, filenames, type, output);

    return 0;
} /* end of main() */

