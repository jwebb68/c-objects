#include "cobj_memory.h"

#include "cobj_defs.h"  // UNUSED_ARG

#include <string.h>  // memset
#include <inttypes.h>  // uint8_t


void memwipe2(void *const b, void *const e) {
#if !defined(NDEBUG)
    for (uint8_t *it=(uint8_t *)b; it != (uint8_t *)e; ++it) {
        *it = 0xa5;
    }
#else
    UNUSED_ARG(b);
    UNUSED_ARG(e);
#endif//!defined(NDEBUG)
}

void memwipe(void *const p, size_t const n) {
    // memset(p, 0xa5, n);
    memwipe2(p, (uint8_t *)p + n);
}
