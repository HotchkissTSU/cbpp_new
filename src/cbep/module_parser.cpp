#include "cbep/module_parser.h"

bool MountSearchPaths(yyjson_val* jPaths) {
    if( !yyjson_is_obj(jPaths) ) {
        return false;
    }

    yyjson_obj_foreach(jPaths, )
}
