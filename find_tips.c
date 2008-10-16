/*
 * find_tips.c
 * Jonathan D. Stott <jonathan.stott@gmail.com>
 *
 */

#include "tip_trace.h"

int find_tips(int x, int y, float ** sheet_1, float isoline_1, float ** sheet_2,
        float isoline_2, int ntips, point_t * tips) {
// This method calculates if there are any spiral wave tips on the sheets, given
// the desired isolines.  To find true tips, the two sheets should either
// represent the same variables at different time steps, or two different
// variables with an approximately 90 degree phase relationship.
//
// arguments:
//  x:              x-dimension of the sheet
//  y:              y-dimension of the sheet
//  sheet_1[y][x]:  2D array of floats, the first sheet. sheet_1
//  isoline_1:      the level of the isoline on the first sheet
//  sheet_2[y][x]:  2D array of floats, the second sheet
//  isoline_2:      the level of the isoline on the second sheet
//  ntips:          The number of tips allocated in the tips array
//  intercepts:     Array of points to store tip co-ordinates in
//
// returns:
//  -n:             Number of tips found was greater than number allocated. n is
//                  the actual number found.
//  0:              No tips found
//  >0:             Number of tips found.
    int i, j;
    int nintercepts_1, nintercepts_2;
    point_t line_1[2], line_2[2], tip;
    int istip, tip_count;
    tip_count = 0;

    // loop over the whole sheet, looking for crossing isolines
    for (j = 1; j < y-1; ++j) {
        for (i = 1; i < x-1; ++i) {
            // find isolines in first frame
            nintercepts_1 = find_isoline(isoline_1, sheet_1, i, j, line_1);

            // find isoline in second frame
            nintercepts_2 = find_isoline(isoline_2, sheet_2, i, j, line_2);

            // if we have two isolines in this square ...
            if ((2 == nintercepts_1)&&(2 == nintercepts_2)) {
                // calculate the tip co-ordinates
                istip = calculate_tip_coordinates(line_1, line_2, &tip);

                // if it's actually a tip in the cell.
                if (istip) {
                    // increment the tip counter
                    tip_count++;

                    // make sure we have space
                    if (tip_count <= ntips) {
                        // store it.
                        tips[tip_count-1].x = tip.x + i;
                        tips[tip_count-1].y = tip.y + j;
                    } // if (tip_count <= ntips)
                } // if (is_tip)
            } // if ((2 == nintercepts_1)&&(2 == nintercepts_2))
        } // x loop
    } // y loop

    // check to see if we ran out of tip space
    if (tip_count > ntips) {
        return -tip_count;
    } else {
        return tip_count;
    }
}
