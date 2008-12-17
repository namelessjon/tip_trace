/******************************************************************************
 * helper.h
 *
 *
 * defines certain useful macros for source code.
 *
 * The intent of these macros is provide cleaner, safer code that tests for the
 * correct functioning of various file and memory operations. While the program
 * will in all likelihood crash anyway as result of one of these operations
 * failing and this fact being untested, by testing when the operation occurs
 * we can eliminate or confirm it as the point of failure.
 */

#ifndef HELPER_H
#define HELPER_H

#include <stdio.h>
#include <stdlib.h>

// File opening macro
// takes
//  * file pointer, file
//  * file mode, mode
//  * parameters to construct the file name, string
// The string _must_ start with a string constant, which can optionally
// include any number of format specifiers and the associated variables
// as you would for a normal printf statement.
//
// Once opened, the file is checked and if it is NULL (i.e. the opening
// failed), the program exits, with a message informing you why.

#define open(file,mode,...) {  \
                                        char _str[64]; \
                                        sprintf(_str,__VA_ARGS__); \
                                        file = fopen(_str,mode); \
                                        if (NULL == file) { \
                                            oops(_str); \
                                        } \
                                        }

// Ooops macro
// takes
//  * string that this prepended before the system error message, string
//  e.g. file_name.dat: No such file or directory when string = file_name.dat
#define oops(string) { perror((string)); exit(EXIT_FAILURE); }



/* ----------- malloc macros ---------- */

// malloc macro
// takes
//  * pointer to be allocated, p
//  * size to be allocated, s
//  * message should there be an error, m
#define MALLOC(p,s,m) {(p) = malloc((s));if(NULL==(p)){oops((m));}}

// double malloc
// takes
//  * pointer to be allocated, p
//  * number of doubles to be allocated, s
//  * message should there be an error, m
#define MALLOC_D(p,s,m) MALLOC(p,(s)*sizeof(double),m)

// float malloc
// takes
//  * pointer to be allocated, p
//  * number of floats to be allocated, s
//  * message should there be an error, m
#define MALLOC_F(p,s,m) MALLOC(p,(s)*sizeof(float),m)

// char malloc
// takes
//  * pointer to be allocated, p
//  * number of chars to be allocated, s
//  * message should there be an error, m
#define MALLOC_C(p,s,m) MALLOC(p,(s)*sizeof(char),m)

// int malloc
// takes
//  * pointer to be allocated, p
//  * number of ints to be allocated, s
//  * message should there be an error, m
#define MALLOC_I(p,s,m) MALLOC(p,(s)*sizeof(int),m)

// double pointer malloc
// takes
//  * pointer to be allocated, p
//  * number of doubles to be allocated, s
//  * message should there be an error, m
#define MALLOC_DP(p,s,m) MALLOC(p,(s)*sizeof(double*),m)

// float pointer malloc
// takes
//  * pointer to be allocated, p
//  * number of float pointers to be allocated, s
//  * message should there be an error, m
#define MALLOC_FP(p,s,m) MALLOC(p,(s)*sizeof(float*),m)

// char pointer malloc
// takes
//  * pointer to be allocated, p
//  * number of chars to be allocated, s
//  * message should there be an error, m
#define MALLOC_CP(p,s,m) MALLOC(p,(s)*sizeof(char*),m)

// int pointer malloc
// takes
//  * pointer to be allocated, p
//  * number of ints to be allocated, s
//  * message should there be an error, m
#define MALLOC_IP(p,s,m) MALLOC(p,(s)*sizeof(int*),m)

// int array 2d
// Allocates to give 2d array accessible through array[j][i] notation but with
// contiguous storage of the variables, allowing for easy output to files and
// transmission via mpi etc.
#define INT_ARRAY_2D(p,rows,cols) { \
                                    int _i_; \
                                    MALLOC_IP(p,(rows),"row alloc failure"); \
                                    MALLOC_I(p[0],(rows)*(cols),"array alloc failure"); \
                                    for (_i_=1;_i_<(rows);++_i_) p[_i_] = p[0] + (_i_*(cols)); \
                               }


// double array 2d
// Allocates to give 2d array accessible through array[j][i] notation but with
// contiguous storage of the variables, allowing for easy output to files and
// transmission via mpi etc.
#define D_ARRAY_2D(p,rows,cols) { \
                                    int _i_; \
                                    MALLOC_DP(p,(rows),"row alloc failure"); \
                                    MALLOC_D(p[0],(rows)*(cols),"array alloc failure"); \
                                    for (_i_=1;_i_<(rows);++_i_) p[_i_] = p[0] + (_i_*(cols)); \
                               }

// float array 2d
// Allocates to give 2d array accessible through array[j][i] notation but with
// contiguous storage of the variables, allowing for easy output to files and
// transmission via mpi etc.
#define F_ARRAY_2D(p,rows,cols) { \
                                    int _i_; \
                                    MALLOC_FP(p,(rows),"row alloc failure"); \
                                    MALLOC_F(p[0],(rows)*(cols),"array alloc failure"); \
                                    for (_i_=1;_i_<(rows);++_i_) p[_i_] = p[0] + (_i_*(cols)); \
                               }

// char array 2d
// Allocates to give 2d array accessible through array[j][i] notation but with
// contiguous storage of the variables, allowing for easy output to files and
// transmission via mpi etc.
#define UC_ARRAY_2D(p,rows,cols) { \
                                    int _i_; \
                                    MALLOC(p,(rows)*sizeof(unsigned char*),"row alloc failure"); \
                                    MALLOC(p[0],(rows)*(cols)*sizeof(unsigned char),"array alloc failure"); \
                                    for (_i_=1;_i_<(rows);++_i_) p[_i_] = p[0] + (_i_*(cols)); \
                               }

// double pointer array 2d
// Allocates to give 2d array accessible through array[j][i] notation but with
// contiguous storage of the variables, allowing for easy output to files and
// transmission via mpi etc.
#define DP_ARRAY_2D(p,rows,cols) { \
                                    int _i_; \
                                    MALLOC(p,(rows)*sizeof(double**),"row alloc failure"); \
                                    MALLOC(p[0],(rows)*(cols)*sizeof(double*),"array alloc failure"); \
                                    for (_i_=1;_i_<(rows);++_i_) p[_i_] = p[0] + (_i_*(cols)); \
                               }

// free 2D
// frees data allocated in the above pattern, first closing the array[0] 
// pointer, then the array pointer
#define FREE_2D(array) { \
                            free( ## array ## [0]); \
                            free( ## array ## ); \
                        }


/* ---------- fread macros ---------- */
// general READ macro
// takes
//  * pointer to read into, pointer
//  * size of variable to read, size
//  * number to read, count
//  * file to read from, file
//  * message for failure, message.
// attempts to read in the total number specified. If it fails to read in
// this many, displays error message to stderr and exits program.
#define READ(pointer,size,count,file,message) { \
            int _read_ = fread((pointer),(size),(count),(file)); \
            if ((count) != _read_) { \
                fprintf(stderr,"%d: %s (%d of %d read)\n",__LINE__,(message),_read_,(count)); \
                fflush(stderr); \
                exit(EXIT_FAILURE); \
            } \
        }

// reads in doubles from a file, using the above macro
#define READ_D(pointer,count,file,message) READ(pointer,sizeof(double),count,file,message)

// reads in ints from a file, using the above macro
#define READ_I(pointer,count,file,message) READ(pointer,sizeof(int),count,file,message)

// reads in doubles from a file, using the above macro
#define READ_C(pointer,count,file,message) READ(pointer,sizeof(char),count,file,message)

// reads in unsigned chars
#define READ_UC(pointer,count,file,message) READ(pointer,sizeof(unsigned char),count,file,message)

#endif //end inclusion guard
