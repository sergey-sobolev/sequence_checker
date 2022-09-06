/** 
 * @file
 * @brief Debug headers, functions and macroses.
 */

#ifndef SQ_DEBUG_HDR
#define SQ_DEBUG_HDR

#include <stdio.h>  // printf
#include <assert.h> // assert macro

void exit(const int );

#define DEBUG
#define DEBUG_VERBOSE

extern int verbose_output;

#ifdef DEBUG
#define D(A) if (verbose_output) { A; }
#else
#define D(A) 
#endif

#ifdef DEBUG_VERBOSE
#define D2(A) if (verbose_output > 1) { A; }
#else
#define D2(A) 
#endif

#endif // SQ_DEBUG_HDR
