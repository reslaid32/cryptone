#ifndef API_H
#define API_H

#define VIS_DEF __attribute__ ((visibility ("default")))
#define WEAK    __attribute__ ((weak))
#define PUBLIC  VIS_DEF
#define PRIVATE static inline

#endif