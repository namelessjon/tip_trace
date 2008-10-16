/*
 * calculate_tip_coordinates.c
 * Jonathan D. Stott <jonathan.stott@gmail.com>
 *
 */

#include <math.h>
#include "point_t.h"

int calculate_tip_coordinates(point_t * first, point_t * second,
        point_t * tip) {
// calculates the intercept co-ordinates, if they are within the local cell,
// relative to the local cell.  The lines representing the two isolines to
// consider are first tested to see if they cross anywhere, and then to see if
// they cross within the cell.
//
// arguments:
//  first:          points representing the first isoline  (length 2)
//  second:         points representing the second isoline (length 2)
//  tip:            point representing the coordinates of the tip, if it is
//                  within the cell
//
// returns:
//  0:              No tip within the current cell.
//  1:              tip within the current cell, values assigned to tip.

    float a11, a12, a21, a22, d1, d2, det, tu, tv;

    // find out if they cross via calculation of determinant
    a11 = first[0].x - first[1].x;
    a12 = second[1].x - second[0].x;
    a21 = first[0].y - first[1].y;
    a22 = second[1].y - second[0].y;
    d1  = second[1].x - first[1].x;
    d2  = second[1].y - first[1].y;

    det = (a11 * a22) - (a12 * a21);

    // det = 0 ---> no solution
    if (fabs(det) < 1e-6)
        return 0;

    tu = ((d1 * a22) - (a12 * d2))/det;
    tv = ((d2 * a11) - (a21 * d1))/det;

    // check to see if we intercept within the current cell
    if ((tu >= 0.0)&&(tu <= 1.0)&&(tv >= 0.0)&&(tv <= 1.0)) {
        // we are!

        // calculate the intercept in the local cell
        tip->x = (first[0].x * tu) + (first[1].x * (1 - tu));
        tip->y = (first[0].y * tv) + (first[1].y * (1 - tv));

        return 1;

    }   else {
        // no tip here!
        return 0;
    }
}
