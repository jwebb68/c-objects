#include "cobj_memory.h"

#include <string.h> // memset

void memwipe(void *const p, size_t const n) {
    memset(p, 0xa5, n);
}
