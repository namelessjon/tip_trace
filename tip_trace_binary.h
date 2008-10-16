/*
 * tip_trace_binary.h
 * Jonathan D. Stott <jonathan.stott@gmail.com>
 *
 * Functions used as part of the tip trace binary (as compared to the library)
 */

#ifndef TIP_TRACE_BINARY_H
#define TIP_TRACE_BINARY_H
#include <stdio.h>
#include "utils/string_list.h"

typedef enum file_type {
    BINARY_FLOAT,
    BINARY_DOUBLE,
    TEXT
} file_type_t;

void process_file_list(int x, int y, float dt, float isoline, string_list_t *list,
        file_type_t file_type, FILE *output);
// given the dimensions of the tissue and a list of filenames, process each one
// in turn, printing the tip trace to the give output file.
//
// arguments:
//  x:          x dimension of the sheet
//  y:          y dimension of the sheet
//  dt:         interval between files
//  isoline:    isoline to search on
//  list:       list of filenames to process
//  file_type:  Type of files in the list
//  output:     file pointer to output too.


int read_file(file_type_t file_type, int x, int y, float **sheet, const char *filename);
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

#endif // TIP_TRACE_BINARY_H
