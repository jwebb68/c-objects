#if !defined(COBJ_QUEUET_H)
#    define COBJ_QUEUET_H

#    include <cobj/core/cobj_defs.h> // WARN_UNUSED_RESULT
#    include <cobj/core/cobj_error.h>
#    include <cobj/t.h>
#    include <stdbool.h>
#    include <stddef.h> // size_t

typedef struct cobj_QueueT_s cobj_QueueT;
struct cobj_QueueT_s {
    T *ptr;
    size_t alloc;
    size_t len;
    size_t wp;
    size_t rp;
};

void cobj_QueueT_destroy_member(cobj_QueueT *const self);
void cobj_QueueT_destroy(cobj_QueueT *const self);

void cobj_QueueT_move_member(cobj_QueueT *const self, cobj_QueueT *const src);
void cobj_QueueT_move(cobj_QueueT *const self, cobj_QueueT *const src);

void cobj_QueueT_new(cobj_QueueT *const self, T *const ptr, size_t alloc);
void cobj_QueueT_clear(cobj_QueueT *const self);

bool cobj_QueueT_is_empty(cobj_QueueT const *const self);
size_t cobj_QueueT_len(cobj_QueueT const *const self);
size_t cobj_QueueT_alloc(cobj_QueueT const *const self);

bool WARN_UNUSED_RESULT cobj_QueueT_put(cobj_QueueT *const self, T *const item);
bool WARN_UNUSED_RESULT cobj_QueueT_get(cobj_QueueT *const self, T *const item);

#endif // COBJ_QUEUET_H
