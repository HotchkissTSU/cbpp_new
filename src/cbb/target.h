#ifndef CBB_TARGET_H
#define CBB_TARGET_H

#include "cbpp/Array.h"
#include "cbpp/String.h"

enum class ETargetType : uint32_t {
    EXE,
    OBJ,
    STATIC,
    DYNAMIC
};

struct BuildTargetRaw {
    char *sBase, *sOut, *sObj;

    cbpp::CArray<char*> aDepend;
    ETargetType iType;

    cbpp::CArray<char*> aSource, 
                        aInclude, 
                        aLink, 
                        aCCArgs, 
                        aLDArgs;
};

struct BuildTarget {
    BuildTarget* pBase;
    char *sOut, *sObj;

    cbpp::CArray<char*> aDepend;
    ETargetType iType;

    cbpp::CArray<char*> aSource, 
                        aInclude, 
                        aLink, 
                        aCCArgs, 
                        aLDArgs;
};

#endif
