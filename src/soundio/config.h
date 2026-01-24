#ifndef SOUNDIO_CONF
#define SOUNDIO_CONF

#include "cbpp/Constants.h"

#ifdef CBPP_LINUX
    #define SOUNDIO_HAVE_PULSEAUDIO
#else
    #define SOUNDIO_HAVE_WASAPI
#endif

#define SOUNDIO_VERSION_STRING "1.0.0"

#define SOUNDIO_VERSION_MINOR 0
#define SOUNDIO_VERSION_MAJOR 1
#define SOUNDIO_VERSION_PATCH 0

#endif
