/*
 * tip_trace.h
 * Jonathan D. Stott <jonathan.stott@gmail.com>
 *
 * The main tip trace library.  This header files contains the functional
 * definitions for linking against the library.  The functions within this
 * header file are generally laid out according to level within the library,
 * with higher level functions first.
 *
 */

#ifndef TIP_TRACE_H
#define TIP_TRACE_H

#include "point_t.h"

int find_tips(int x, int y, float ** sheet_1, float isoline_1, float ** sheet_2,
        float isoline_2, int ntips, point_t * tips, int num);
// This method calculates if there are any spiral wave tips on the sheets, given
// the desired isolines.  To find true tips, the two sheets should either
// represent the same variables at different time steps, or two different
// variables with an approximately 90 degree phase relationship.
//
// arguments:
//  x:              x-dimension of the sheet
//  y:              y-dimension of the sheet
//  sheet_1:        2D array of floats, the first sheet
//  isoline_1:      the level of the isoline on the first sheet
//  sheet_2:        2D array of floats, the second sheet
//  isoline_2:      the level of the isoline on the second sheet
//  ntips:          The number of tips allocated in the tips array
//  intercepts:     Array of points to store tip co-ordinates in
//
// returns:
//  -n:             Number of tips found was greater than number allocated. n is
//                  the actual number found.
//  0:              No tips found
//  >0:             Number of tips found.


int find_isoline(float isoline, float ** E, int i, int j, point_t * intercepts);
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

int calculate_tip_coordinates(point_t * first, point_t * second,
        point_t * tip);
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


void read_binary_float_sheet(const char *filename, int nx, int ny, float ** E);
#endif // TIP_TRACE_H

