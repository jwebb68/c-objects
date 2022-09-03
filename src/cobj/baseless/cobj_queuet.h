#if !defined(COBJ_QUEUET_H)
#    define COBJ_QUEUET_H

#    include <cobj/core/cobj_defs.h> // WARN_UNUSED_RESULT
#    include <cobj/core/cobj_error.h>
#    include <cobj/t.h>
#    include <stdbool.h>
#    include <stddef.h> // size_t

typedef struct QueueT_s QueueT;
struct QueueT_s {
    T *ptr;
    size_t alloc;
    size_t len;
    size_t wp;
    size_t rp;
};

void QueueT_destroy_member(QueueT *const self);
void QueueT_destroy(QueueT *const self);

void QueueT_move_member(QueueT *const self, QueueT *const src);
void QueueT_move(QueueT *const self, QueueT *const src);

void QueueT_new(QueueT *const self, T *const ptr, size_t alloc);
void QueueT_clear(QueueT *const self);

bool QueueT_is_empty(QueueT const *const self);
size_t QueueT_len(QueueT const *const self);
size_t QueueT_alloc(QueueT const *const self);

bool WARN_UNUSED_RESULT QueueT_put(QueueT *const self, T *const item);
bool WARN_UNUSED_RESULT QueueT_get(QueueT *const self, T *const item);

#endif // COBJ_QUEUET_H
