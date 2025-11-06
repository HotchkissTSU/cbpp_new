#ifndef CBPP_MACRODEF_H
#define CBPP_MACRODEF_H

/*
    All useful macros are stored here
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

// Currently, i only have a linux computer, so some W*ndows-dependent code is yet impossible to write and test
#define CBPP_NO_WINDOWS_ASSERT static_assert(false, "Non-implemented Windows-dependent code here");

#ifdef __linux__
    #define CBPP_LINUX
    #define CBPP_ON_LINUX(code) code
    #define CBPP_ON_WINDOWS(code)
#else
    #define CBPP_WINDOWS
    #define CBPP_ON_LINUX(code)
    #define CBPP_ON_WINDOWS(code) code
#endif

#ifdef CBPP_DEBUG
    #define CBPP_ON_DEBUG(code) code
#else
    #define CBPP_ON_DEBUG(code)
#endif

#define CbAssert(expr, msg) if((bool)(expr) == true) {\
    printf("File: '%s', Line: %lu\nAssertion [ %s ] has failed: %s\n", __FILE__, __LINE__, #expr, msg); exit(EXIT_FAILURE); }

#define forever while(1)
//#define for_bitmask(iter, bword, current_bit) for((iter)=0;(iter)<sizeof(bword)*8;(iter)++,(current_bit)=((bword)&(1<<(iter)))>>(iter))

// The smallest possible number to consider in calculations
#define CBPP_EPSILON 1e-10f

#define CBPP_PI 3.14159265358979323846f

/*
    We are building a serverside library by default
*/

#ifdef CBPP_CLIENTSIDE
    #define CBPP_ON_SERVER(code)
    #define CBPP_ON_CLIENT(code) code
#else
    #define CBPP_SERVERSIDE
    #define CBPP_ON_SERVER(code) code
    #define CBPP_ON_CLIENT(code)
#endif

#endif
