#if !defined(COBJ_MEMORY_H)
#define COB_MEMORY_H

#include <stddef.h> // size_t


void memwipe(void *const p, size_t const n);
#define STRUCTWIPE(s) memwipe(s, sizeof(*s))

#endif//defined(COBJ_MEMORY_H)
