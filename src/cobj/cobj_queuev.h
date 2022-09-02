#if !defined(COBJ_QUEUEV_H)
#    define COBJ_QUEUEV_H

#    include "cobj_defs.h" // WARN_UNUSED_RESULT
#    include "cobj_error.h"

#    include <inttypes.h> // uint8_t
#    include <stdbool.h>
#    include <stddef.h> // size_t

//==========================================================================

typedef struct QueueV_s QueueV;
struct QueueV_s {
    uint8_t *ptr;
    size_t alloc;
    size_t len;
    size_t wp;
    size_t rp;
};

void QueueV_destroy_member(QueueV *const self,
                           void *(*elem_ptr_mut)(void *const arr, size_t idx),
                           void (*elem_destroy)(void *const));
void QueueV_destroy(QueueV *const self,
                    void *(*elem_ptr_mut)(void *const arr, size_t idx),
                    void (*elem_destroy)(void *const));

void QueueV_move_member(QueueV *const self, QueueV *const src);
void QueueV_move(QueueV *const self, QueueV *const src);

void QueueV_new(QueueV *const self, void *const ptr, size_t alloc);
void QueueV_clear(QueueV *const self,
                  void *(*elem_ptr_mut)(void *const arr, size_t idx),
                  void (*elem_destroy)(void *const));

bool QueueV_is_empty(QueueV const *const self);
size_t QueueV_len(QueueV const *const self);
size_t QueueV_alloc(QueueV const *const self);

bool WARN_UNUSED_RESULT QueueV_put(QueueV *const self,
                                   void *const elem,
                                   void *(*elem_ptr_mut)(void *const arr, size_t idx),
                                   void (*elem_move)(void *const, void *const));
bool WARN_UNUSED_RESULT QueueV_get(QueueV *const self,
                                   void *const item,
                                   void *(*elem_ptr_mut)(void *const arr, size_t idx),
                                   void (*elem_move)(void *const, void *const));

#endif // COBJ_QUEUEV_H
