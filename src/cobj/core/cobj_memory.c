#include <cobj/core/cobj_defs.h> // COBJ_UNUSED_ARG
#include <cobj/core/cobj_memory.h>
#include <inttypes.h> // uint8_t
#include <string.h> // memset

#if 0
void memwipe2(void *const b, void *const e)
{
#    if !defined(NDEBUG)
    for (uint8_t *it = (uint8_t *)b; it != (uint8_t *)e; ++it) {
        *it = 0xa5;
    }
#    else
    COBJ_UNUSED_ARG(b);
    COBJ_UNUSED_ARG(e);
#    endif //! defined(NDEBUG)
}
#endif

void memwipe(void *const p, size_t const n)
{
    memset(p, 0xa5, n);
    // memwipe2(p, (uint8_t *)p + n);
}

void *try_malloc(size_t size, Error *const err)
{
    void *p = malloc(size);
    if (p != NULL) { return p; }
    bool r = ERROR_RAISE(err, Error_ENOMEM);
    (void)r;
    return NULL;
}

void _swap32(uint32_t *a, uint32_t *b)
{
    uint32_t t = *a;
    *a = *b;
    *b = t;
}
void _swap16(uint16_t *a, uint16_t *b)
{
    uint16_t t = *a;
    *a = *b;
    *b = t;
}
void _swap8(uint8_t *a, uint8_t *b)
{
    uint8_t t = *a;
    *a = *b;
    *b = t;
}

void _memswap(uint8_t *ab, uint8_t *ae, uint8_t *bb)
{
    while ((ae - ab) >= (ptrdiff_t)sizeof(uint32_t)) {
        _swap32((uint32_t *)ab, (uint32_t *)bb);
        ab += sizeof(uint32_t);
        bb += sizeof(uint32_t);
    }
    while ((ae - ab) >= (ptrdiff_t)sizeof(uint16_t)) {
        _swap16((uint16_t *)ab, (uint16_t *)bb);
        ab += sizeof(uint16_t);
        bb += sizeof(uint16_t);
    }
    for (; ab != ae; ++ab, ++bb) {
        _swap8(ab, bb);
    }
}

void memswap(void *const a, void *const b, size_t const n)
{
    _memswap(a, (uint8_t *)a + n, b);
}

void ptrswap(void **const a, void **const b)
{
    void *p = *a;
    *a = *b;
    *b = p;
}

void sizetswap(size_t *const a, size_t *const b)
{
    size_t v = *a;
    *a = *b;
    *b = v;
}
