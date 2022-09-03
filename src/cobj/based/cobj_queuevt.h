#if !defined(COBJ_QUEUEVT_H)
#    define COBJ_QUEUEVT_H

#    include <cobj/based/cobj_queuev.h>
#    include <cobj/t.h>

typedef struct QueueVT_s QueueVT;
struct QueueVT_s {
    QueueV inner;
};

void QueueVT_destroy_member(QueueVT *const self);
void QueueVT_destroy(QueueVT *const self);

void QueueVT_move_member(QueueVT *const self, QueueVT *const src);
void QueueVT_move(QueueVT *const self, QueueVT *const src);

void QueueVT_new(QueueVT *const self, void *const ptr, size_t alloc);
void QueueVT_clear(QueueVT *const self);

bool QueueVT_is_empty(QueueVT const *const self);
size_t QueueVT_len(QueueVT const *const self);
size_t QueueVT_alloc(QueueVT const *const self);

bool WARN_UNUSED_RESULT QueueVT_put(QueueVT *const self, T *const elem);
bool WARN_UNUSED_RESULT QueueVT_get(QueueVT *const self, T *const item);

#endif // COBJ_QUEUEVT_H
