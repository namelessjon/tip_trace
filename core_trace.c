/*
 * core_trace.c
 * Jonathan D. Stott <jonathan.stott@gmail.com>
 * Created: Tuesday, October 07, 2008 @ 10:04
 * Modified: Monday, October 13, 2008 @ 12:00
 *
 */

#include <stdio.h>

#include "tip_trace_binary.h"
#include "utils/string_list.h"
#include "helper.h"

int main (int argc, char ** argv) {

    // dimensions of the sheet.
    int nx, ny;
    int total;

    float dt;

    // the current file we're looking at.
    int index;


    float level = -30;

    FILE *filelist;

    //filenames
    string_list_t *filenames;
    char filename[64];

    // setup some variables until we can split this out into a proper function
    nx = 375;
    ny = 375;
    total = 6000;
    dt = 1;

    filenames = new_string_list();

    open(filelist, "r", "list.txt");
    for (index = 0; index < total; ++index) {
        fscanf(filelist, "%s", filename);
        string_list_push(filenames, filename);
    }

    process_file_list(nx, ny, dt, level, filenames, BINARY_FLOAT, stdout);

    return 0;
} /* end of main() */

