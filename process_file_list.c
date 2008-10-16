/*
 * process_file_list.c
 * Jonathan D. Stott <jonathan.stott@gmail.com>
 *
 */

#include "tip_trace_binary.h"
#include "helper.h"

#include <math.h>
#include <zlib.h>

#include "helper.h"
#include "tip_trace.h"
#include "utils/string_list.h"

#define NUM_TIPS (20)

void process_file_list(int x, int y, float dt, float isoline, string_list_t *list,
        file_type_t file_type, FILE *output) {
// given the dimensions of the tissue and a list of filenames, process each one
// in turn, printing the tip trace to the give output file.
//
// arguments:
//  x:          x dimension of the sheet
//  y:          y dimension of the sheet
//  dt:         interval between sheets
//  isoline:    The isoline to search for
//  list:       list of filenames to process
//  file_type:  Type of files in the list
//  output:     file pointer to output too.

    int i, j, ntips;

    float time;

    // the current file we're looking at.
    int index;

    // arrays to hold the sheets to be set
    float ** E;
    float ** GH;

    point_t tips[NUM_TIPS];


    // allocate our arrays
    F_ARRAY_2D(E, y, x);
    F_ARRAY_2D(GH, y, x);


    // zero the arrays involved
    for (j = 0; j < y; ++j) {
        for (i = 0; i < x; ++i) {
            E[j][i] = 0.0;
        }
    }

    // loop over all the files
    for (index = 0; index < string_list_length(list); ++index) {

        // copy E into GH
        for (j = 0; j < y; ++j) {
            for (i = 0; i < x; ++i) {
                GH[j][i] = E[j][i];
            }
        }

        // read in file.
        if (0 == read_file(file_type, x, y, E, string_list_at(list, index))) {
            // calculate tip traces
            ntips = find_tips(x, y, E, isoline, GH, isoline, NUM_TIPS, tips);
            time = index * dt;
            if (ntips > -1) {
                // if we have tips, output them!
                for (i = 0; i < ntips; ++i) {
                    fprintf(output, "%f %f %f\n", time, tips[i].x, tips[i].y);
                }
            } else {
                fprintf(stderr, "Too many tips in file %s (%d)\n", string_list_at(list, index), ntips);
            }
        } else {
            fprintf(stderr, "Problem reading in %s\n"
                    , string_list_at(list, index));

        }

    }

    return;
}


