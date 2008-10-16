/*
 * read_file.c
 * Jonathan D. Stott <jonathan.stott@gmail.com>
 *
 */
#include <zlib.h>
#include "helper.h"

#include "tip_trace_binary.h"

static int read_binary_float_sheet(int x, int y, float ** E, const char *filename);
static int read_binary_double_sheet(int x, int y, float ** E, const char *filename);
static int read_text_sheet(int x, int y, float ** E, const char *filename);

int read_file(file_type_t file_type, int x, int y, float **sheet, const char *filename) {
// reads in the given file, assigning the values to sheet.
//
// file_type:   Type of files in the list
// x:           x dimension of the sheet
// y:           y dimension of the sheet
// sheet[y][x]: 2D sheet of floats
// filename:    filename to open
//
// returns:
//  0:  success
//  <0: error
    switch(file_type) {
        case BINARY_FLOAT:
              return read_binary_float_sheet(x, y, sheet, filename);
        case BINARY_DOUBLE:
              return read_binary_double_sheet(x, y, sheet, filename);
        case TEXT:
              return read_text_sheet(x, y, sheet, filename);
        default:
              fprintf(stderr, "Unknown sheet type\n");
              return -1;
    }
}


static int read_binary_float_sheet(int x, int y, float ** E, const char * filename) {
    gzFile sheet_file;
    int rw;

    sheet_file = gzopen(filename, "r");

    if (!sheet_file) {
        perror(filename);
        return -1;
    }

    rw = gzread(sheet_file, E[0], sizeof(float)*x*y);
    if (rw != x*y*sizeof(float)) {
        fprintf(stderr, "Problem reading %s\n", filename);
        fprintf(stderr, "%d/%d floats read\n", rw, x*y);
        gzclose(sheet_file);
        return -1;
    }

    gzclose(sheet_file);
    return 0;
}

static int read_binary_double_sheet(int x, int y, float ** E, const char * filename) {
    gzFile sheet_file;
    int rw, j, i;

    double ** sheet;

    sheet_file = gzopen(filename, "r");

    if (!sheet_file) {
        perror(filename);
        return -1;
    }

    D_ARRAY_2D(sheet, y, x);


    rw = gzread(sheet_file, sheet[0], sizeof(double)*x*y);
    if (rw != x*y*sizeof(double)) {
        fprintf(stderr, "Problem reading %s\n", filename);
        fprintf(stderr, "%d/%d doubles read\n", rw, x*y);
        free(sheet[0]);
        free(sheet);
        gzclose(sheet_file);
        return -1;
    }

    for (j = 0; j < y; ++j) {
        for (i = 0; i < x; ++i) {
            E[j][i] = (float) sheet[j][i];
        }
    }

    free(sheet[0]);
    free(sheet);

    gzclose(sheet_file);
    return 0;
}

static int read_text_sheet(int x, int y, float ** E, const char * filename) {
    fprintf(stderr, "Not implemented yet!\n");
    exit(EXIT_FAILURE);
}
