/*
 * gifoutput.h
 * Jonathan D. Stott <jonathan.stott@gmail.com>
 * Created: Monday, April 07, 2008 @ 15:03
 * Modified: Wednesday, October 01, 2008 @ 13:56
 *
 * Copyright (c) Jonathan Stott, 2008
 *
 * GPL v2
 */

#ifndef GIFOUTPUT_H
#define GIFOUTPUT_H

// enumerable of the 'normal' colours, stored in the palette before the 250
// colours that make up the voltage map. START_SCALE is a special value, which
// indicates the start of the colour scale.  It should always be the last entry
// in the enum
enum colours {
    WHITE,
    BLACK,
    RED,
    GREEN,
    BLUE,
    START_SCALE
};

// Total number of colours to allocate for the colourmap
// NB: This MUST be a power of two, less than or equal to 256
// For the use it's put to, it must be 256, too.
#define NUM_GIF_COLOURS (256)

// The number of colours in the voltage map
#define MAP_COLOURS (250)

// enum which determines if a gif should interlace or not.
// ATM, no support for interlacing.
enum gif_interlace {
    NO_INTERLACE,
    INTERLACE
};

// int create_palette();
//
// Creates a palette, which will be stored in a static variable for the duration
// of the program.  This palette will have the first entry be white, the second
// black, and fill 250 entries with a hot metal colourscheme.
// returns 0 on success, 1 on an error (probably memory related)
int create_palette(void);

// void set_limts(double min, double max)
//
// Sets the limits of the spectrum.  Anything below minimum will be the minimum
// of the palette, anything above max will be max.  These are stored in static
// variables
void set_limits(double min, double max);

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
        const void * data);

// unsigned char double_to_pixel(double value)
//
// Converts the given value to a colour for a gif pixel.  Relies on maximum and
// minimum having been set previously via the set_limits function.
unsigned char double_to_pixel(double value);


#endif /* GIFOUTPUT_H */

