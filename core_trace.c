/*
 * core_trace.c
 * Jonathan D. Stott <jonathan.stott@gmail.com>
 * Created: Tuesday, October 07, 2008 @ 10:04
 * Modified: Monday, October 13, 2008 @ 12:00
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <zlib.h>

#include "helper.h"
#include "tip_trace.h"

#define NUM_TIPS (20)

void read_binary_float_sheet(const char *filename, int nx, int ny, float ** E);


int main (int argc, char ** argv) {

    // dimensions of the sheet.
    int nx, ny;
    // total number of files to look at
    int total;
    int i, j, ntips;

    float dt;
    float time;

    // the current file we're looking at.
    int index;

    // arrays to hold the sheets to be set
    float ** E;
    float ** GH;

    point_t tips[NUM_TIPS];

    float level = -30;

    FILE *filelist;

    //filenames
    char ** filenames;

    // setup some variables until we can split this out into a proper function
    nx = 375;
    ny = 375;
    total = 5000;
    dt = 1;

    filenames = malloc(total*sizeof(char*));

    open(filelist, "r", "list.txt");
    for (index = 0; index < total; ++index) {
        filenames[index] = malloc(32);
        fscanf(filelist, "%s", filenames[index]);
    }


    // allocate our arrays
    F_ARRAY_2D(E, ny, nx);
    F_ARRAY_2D(GH, ny, nx);


    // zero the arrays involved
    for (j = 0; j < ny; ++j) {
        for (i = 0; i < nx; ++i) {
            E[j][i] = 0.0;
        }
    }

    // loop over all the files
    for (index = 0; index < total; ++index) {

        // copy E into GH
        for (j = 0; j < ny; ++j) {
            for (i = 0; i < nx; ++i) {
                GH[j][i] = E[j][i];
            }
        }

        // read in file.
        read_binary_float_sheet(filenames[index], nx, ny, E);

        // calculate tip traces
        ntips = find_tips(nx, ny, E, level, GH, level, NUM_TIPS, tips);
        time = index * dt;
        if (ntips > -1) {
            // if we have tips, output them!
            for (i = 0; i < ntips; ++i) {
                printf("%f %f %f\n", time, tips[i].x, tips[i].y);
            }
        } else {
            fprintf(stderr, "Too many tips in file %s (%d)\n", filenames[index], ntips);
        }


    }

    return 0;
} /* end of main() */


void read_binary_float_sheet(const char *filename, int nx, int ny, float ** E) {
    gzFile sheet_file;
    int rw;
    
    sheet_file = gzopen(filename, "r");
    
    if (!sheet_file) {
        perror(filename);
        exit(EXIT_FAILURE);
    }

    rw = gzread(sheet_file, E[0], sizeof(float)*nx*ny);
    if (rw != nx*ny*sizeof(float)) {
        fprintf(stderr, "Problem reading %s\n", filename);
        fprintf(stderr, "%d/%d floats read\n", rw, nx*ny);
        exit(EXIT_FAILURE);
    }

    gzclose(sheet_file);
    
}
