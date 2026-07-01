#ifndef CBPP_MACRODEF_H
#define CBPP_MACRODEF_H

#ifdef __linux__
    #define CBPP_LINUX
    #define CBPP_ON_LINUX(...) __VA_ARGS__

    #define CBPP_ON_WINDOWS(...)
#else
    #define CBPP_WINDOWS
    #define CBPP_ON_LINUX(...)
    #define CBPP_ON_WINDOWS(...) __VA_ARGS__
#endif

#ifdef CBPP_DEBUG
    #define CBPP_ON_DEBUG(...) __VA_ARGS__
#else
    #define CBPP_ON_DEBUG(...)
#endif

// Macro to mark some declaration as non-threadsafe. Can later be changed to a static assert to quickly locate them.
#define non_thread_safe

/*
    A protected class cannot be copied or constructed from itself
*/
#define CBPP_PROTECTED_CLASS(classname)                     \
    classname(const classname&) = delete;                   \
    classname(classname&&) = delete;                        \
    classname& operator=(const classname&) = delete;        \
    classname& operator=(classname&&) = delete;

// The smallest possible number to consider in calculations
#define CBPP_EPSILON (1e-10f)

#define CBPP_PI (3.14159265358979323846f)

#endif
