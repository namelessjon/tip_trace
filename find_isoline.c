/*
 * find_isoline.c
 * Jonathan D. Stott <jonathan.stott@gmail.com>
 *
 */

#include <math.h>
#include "point_t.h"

int find_isoline(float isoline, float ** E, int i, int j, point_t * intercepts) {
// this method finds if an isoline crosses the cell considered.  If we
// consider a cell to be:
//
//           2
// j+1, i +------+ j+1,i+i
//        |      |
//     3  |      |  1
//        |      |
//   j, i +------+ j, i+1
//           0
//
// The code first considers if the isoline crosses line number 0, from
// (j, i) to (j, i+1) and if it does, works out where it crosses in the
// local co-ordinate system, assuming a linear gradient of E along that
// line.  It then considers lines 1, 2, and 3 in order.
//
// arguments:
//  isoline:        The value of the isoline to find.
//  E:              the sheet of potentials.
//  i:              current x co-ord
//  j:              current y co-ord
//  intercepts:     the intercepts (point_t array, length >= 2)
//
// returns:
//  number of intercepts.  Should be 0 or 2.
//  -1: problem with the intercepts.
    int cdx[] = { 0, 1, 1, 0, 0 };
    int cdy[] = { 0, 0, 1, 1, 0 };
    int intercept_count = 0, n;
    float prev, next, intercept;

    // loop round the cell.
    for (n = 0; n < 4; ++n) {

        // find the previous point (E(j, i) on the first pass through this loop)
        // relative to the level
        prev = E[j + cdy[n]][i + cdx[n]] - isoline;

        // find the next point (E(j, i+1) on the first pass through this loop)
        // relative to the level
        next = E[j + cdy[n+1]][i + cdx[n+1]] - isoline;

        // if our next point lies on the isoline, ignore this pass.  We'll pick
        // it up on the next loop around, and it stops us double counting the
        // point.
        if (fabs(next) < 10e-6)
            continue;

        // check to see if we actually /cross/ the isoline in this cell.
        // multiplying next by prev will only give a negative number if they
        // cross.
        if ((next*prev) > 0.0)
            continue;

        // we appear to now have an isoline in the edge of the cell that we're
        // currently considering.  Lets estimate where it is!
        // first the x position.
        intercept = ((next * cdx[n]) - (prev * cdx[n+1]))/(next - prev);
        if ((intercept < 0.0)||(intercept > 1.0)) {
            return -1;
        }
        intercepts[intercept_count].x = intercept;

        // then y position.
        intercept = ((next * cdy[n]) - (prev * cdy[n+1]))/(next - prev);
        if ((intercept < 0.0)||(intercept > 1.0)) {
            return -1;
        }
        intercepts[intercept_count].y = intercept;

        ++intercept_count;

    }

    return intercept_count;
}
