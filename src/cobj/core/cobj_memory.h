#if !defined(COBJ_MEMORY_H)
#    define COBJ_MEMORY_H

#    include <cobj/core/cobj_error.h>
#    include <stddef.h> // size_t
#    include <stdlib.h> // malloc/free

void memwipe(void *const p, size_t const n);
#    define STRUCTWIPE(s) memwipe(s, sizeof(*s))

void memwipe2(void *const b, void *const e);

void *try_malloc(size_t size, Error *const err);

void memswap(void *a, void *b, size_t n);

void ptrswap(void **a, void **b);
void sizetswap(size_t *a, size_t *b);

#endif // defined(COBJ_MEMORY_H)
