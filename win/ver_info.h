#ifndef __VER_INFO_H_
#define __VER_INFO_H_

#include <stdio.h>

#define VERSION_MAJOR    1
#define VERSION_MINOR    4
#define VERSION_REVISION 2
#define VERSION_BUILD    0

#define STR_INDIR(x) #x
#define STR(x) STR_INDIR(x)

#define VERSION_STRING STR(VERSION_MAJOR) "." STR(VERSION_MINOR) "." STR(VERSION_REVISION)
#define VERSION_STRING_FULL VERSION_STRING "." STR(VERSION_BUILD)

static inline const char* getVersion(void) {
    static char versionString[32];
    snprintf(versionString, sizeof(versionString), "DDWolf v%s", VERSION_STRING);
    return versionString;
}
#endif
