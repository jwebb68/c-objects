#if !defined(COBJ_QUEUEVT_H)
#    define COBJ_QUEUEVT_H

#    include <cobj/based/cobj_queuev.h>
#    include <cobj/t.h>

typedef struct cobj_QueueVT_s cobj_QueueVT;
struct cobj_QueueVT_s {
    cobj_QueueV inner;
};

void cobj_QueueVT_destroy_member(cobj_QueueVT *const self);
void cobj_QueueVT_destroy(cobj_QueueVT *const self);

void cobj_QueueVT_move_member(cobj_QueueVT *const self, cobj_QueueVT *const src);
void cobj_QueueVT_move(cobj_QueueVT *const self, cobj_QueueVT *const src);

void cobj_QueueVT_new(cobj_QueueVT *const self, void *const ptr, size_t alloc);
void cobj_QueueVT_clear(cobj_QueueVT *const self);

bool cobj_QueueVT_is_empty(cobj_QueueVT const *const self);
size_t cobj_QueueVT_len(cobj_QueueVT const *const self);
size_t cobj_QueueVT_alloc(cobj_QueueVT const *const self);

bool WARN_UNUSED_RESULT cobj_QueueVT_put(cobj_QueueVT *const self, T *const elem);
bool WARN_UNUSED_RESULT cobj_QueueVT_get(cobj_QueueVT *const self, T *const item);

#endif // COBJ_QUEUEVT_H
