#ifndef CBPP_MACRODEF_H
#define CBPP_MACRODEF_H

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

// The smallest possible number to consider in calculations
#define CBPP_EPSILON 1e-10f

#define CBPP_PI (3.14159265358979323846f)

#endif
