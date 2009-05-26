/*
 * gifoutput.c
 * Jonathan D. Stott <jonathan.stott@gmail.com>
 * Created: Monday, April 07, 2008 @ 15:24
 * Modified: Thursday, April 17, 2008 @ 22:14
 *
 * Copyright (c) Jonathan Stott, 2008
 *
 * GPL v2.
 */
#include "gifoutput.h"
#include <math.h>
#include <gif_lib.h>
#include <stdio.h>
#include <stdlib.h>

static ColorMapObject * colour_map = NULL;
static double image_min = -90.0;
static double image_max = 0.0;
static int palette_made = 0;

// determines the value of a colour channel from the formula given, and the
// value.  Formulae are the same as gnuplot, and can be found at
//
// http://zimg.sourceforge.net/zimg.1.html
static unsigned char colour_channel_from_formula(int formula, double value) {
    // if the formula is negative, we first invert the value
    if (formula < 0) {
        value = 1 - value;
        formula = -formula;
    }

    switch (formula) {
        case 0:
            value = 0;
        case 1:
            value = 0.5;
        case 2:
            value = 1;
        case 3:
            break;
        case 4:
            value = value * value;
            break;
        case 5:
            value = value * value * value;
            break;
        case 6:
            value = value * value * value * value;
            break;
        case 7:
            value = sqrt(value);
            break;
        case 8:
            value = sqrt(sqrt(value));
            break;
        case 9:
            value = sin(M_PI_2 * value);
            break;
        case 10:
            value = cos(M_PI_2 * value);
            break;
        case 11:
            value = fabs(value - 0.5);
            break;
        case 12:
            value = (2 * value - 1) * (2.0 * value - 1);
            break;
        case 13:
            value = sin(M_PI * value);
            break;
        case 14:
            value = fabs(cos(M_PI * value));
            break;
        case 15:
            value = sin(2 * M_PI * value);
            break;
        case 16:
            value = cos(2 * M_PI * value);
            break;
        case 17:
            value = fabs(sin(2 * M_PI * value));
            break;
        case 18:
            value = fabs(cos(2 * M_PI * value));
            break;
        case 19:
            value = fabs(sin(4 * M_PI * value));
            break;
        case 20:
            value = fabs(cos(4 * M_PI * value));
            break;
        case 21:
            value = 3 * value;
            break;
        case 22:
            value = 3 * value - 1;
            break;
        case 23:
            value = 3 * value - 2;
            break;
        case 24:
            value = fabs(3 * value - 1);
            break;
        case 25:
            value = fabs(3 * value - 2);
            break;
        case 26:
            value = (1.5 * value - 0.5);
            break;
        case 27:
            value = (1.5 * value - 1);
            break;
        case 28:
            value = fabs(1.5 * value - 0.5);
            break;
        case 29:
            value = fabs(1.5 * value - 1);
            break;
        case 30:
            if (value <= 0.25)
                value = 0.0;
            if (value >= 0.57)
                value = 1.0;
            value = value / 0.32 - 0.78125;
            break;
        case 31:
            if (value <= 0.42)
                value = 0.0;
            if (value >= 0.92)
                value = 1.0;
            value = 2 * value - 0.84;
            break;
        case 32:
            if (value <= 0.42)
                value *= 4;
            else
                value = (value <= 0.92) ? -2 * value + 1.84 : value / 0.08 - 11.5;
            break;
        case 33:
            value = fabs(2 * value - 0.5);
            break;
        case 34:
            value = 2 * value;
            break;
        case 35:
            value = 2 * value - 0.5;
            break;
        case 36:
            value = 2 * value - 1;
            break;
        default:
            fprintf(stderr, "%d is not a valid formula number, -36 to 36 only\n", formula);
            exit(1);
            break;
    }

    // make sure our value is within bounds!
    if (value <= 0)
        value = 0;
    if (value >= 1)
        value = 1;

    return (unsigned char) (255 * value);
}

static GifColorType generate_hot_metal(double value) {
    GifColorType colour;

    colour.Red = colour_channel_from_formula(21, value);
    colour.Green = colour_channel_from_formula(22, value);
    colour.Blue = colour_channel_from_formula(23, value);

    return colour;
}

static GifColorType generate_rainbow(double value) {
    GifColorType colour;

    colour.Red = colour_channel_from_formula(22, value);
    colour.Green = colour_channel_from_formula(13, value);
    colour.Blue = colour_channel_from_formula(-31, value);

    return colour;
}

// int create_palette();
//
// Creates a palette, which will be stored in a static variable for the duration
// of the program.  This palette will have the first entry be white, the second
// black, and fill 250 entries with a hot metal colourscheme.
// returns 0 on success, 1 on an error (probably memory related)
int create_palette() {
    if (palette_made)
        return 0;

    int i;
    GifColorType colour;

    colour_map = MakeMapObject(NUM_GIF_COLOURS, NULL);
    if (!colour_map) {
        fprintf(stderr, "Argh, failed to make map\n");
        return 1;
    }

    // WHITE is white
    colour.Red = 255;
    colour.Green = 255;
    colour.Blue = 255;
    colour_map->Colors[WHITE] = colour;

    // BLACK is black
    colour.Red = 0;
    colour.Green = 0;
    colour.Blue = 0;
    colour_map->Colors[BLACK] = colour;

    // RED is red
    colour.Red = 255;
    colour.Green = 0;
    colour.Blue = 0;
    colour_map->Colors[RED] = colour;

    // GREEN is green
    colour.Red = 0;
    colour.Green = 255;
    colour.Blue = 0;
    colour_map->Colors[GREEN] = colour;

    // BLUE is blue
    colour.Red = 0;
    colour.Green = 0;
    colour.Blue = 255;
    colour_map->Colors[BLUE] = colour;

    double step = 1.0/MAP_COLOURS;
    for (i = START_SCALE; i < (MAP_COLOURS + START_SCALE) ; ++i) {
        colour_map->Colors[i] = generate_rainbow(step*(i-START_SCALE));
    }
    palette_made = 1;
    return 0;
}

// void set_limts(double min, double max)
//
// Sets the limits of the spectrum.  Anything below minimum will be black,
// anything above max will be white.  These are stored in static variables
void set_limits(double min, double max) {
   image_min = min;
   image_max = max;
}

// int output_double_image(const char * filename, int x, int y, double ** data)
//
// outputs the information held in data (which must be y by x) in size, to a
// file named filename.  The limits set by set limits will be used to classify
// the data.
// returns 0 on success, 1 on failure
int output_double_image(const char * filename, int x, int y, double ** data);

// int output_image(const char * filename, int x, int y, const unsigned char * data)
//
// outputs the information held in data, which must be y*x long, to a file named
// filename.
// returns 0 on success, 1 on failure
int output_image(const char * filename, int x, int y,
        const void * data) {

    GifFileType *file;

    // open our file
    file = EGifOpenFileName(filename, FALSE);

    // generate a palette, just to be sure (note, we have a test to prevent
    // multiple generation)
    create_palette();


    // put our screen description here.
    EGifPutScreenDesc(file, x, y, NUM_GIF_COLOURS, BLACK, colour_map);

    // now describe our image
    EGifPutImageDesc(file, 0, 0, x, y, NO_INTERLACE, NULL);

    // and write the line
    EGifPutLine(file, (GifPixelType*) data, x*y);

    EGifCloseFile(file);

    return 0;

}

unsigned char double_to_pixel(double value) {
    double temp;
    if (value >= image_max) {
        return MAP_COLOURS - 1 + START_SCALE;
    } else if (value <= image_min) {
        return START_SCALE;
    } else {
        // scale the value between 0 and 249
        temp = (((value - image_min)/(image_max - image_min))*MAP_COLOURS);
        // add two, to avoid the black and white pre-definitions.
        temp += START_SCALE;
        return (unsigned char) temp;
    }
}



