#ifndef _WINDOWTYPE_H
#define _WINDOWTYPE_H

#include <libwnck/libwnck.h>

const char* window_type_to_str(WnckWindowType type);
WnckWindowType window_type_from_str(const char *name);

#endif
