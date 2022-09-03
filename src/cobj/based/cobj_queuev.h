#if !defined(COBJ_QUEUEV_H)
#    define COBJ_QUEUEV_H

#    include <cobj/core/cobj_defs.h> // WARN_UNUSED_RESULT
#    include <cobj/core/cobj_error.h>
#    include <inttypes.h> // uint8_t
#    include <stdbool.h>
#    include <stddef.h> // size_t

//==========================================================================

typedef struct cobj_QueueV_s cobj_QueueV;
struct cobj_QueueV_s {
    uint8_t *ptr;
    size_t alloc;
    size_t len;
    size_t wp;
    size_t rp;
};

void cobj_QueueV_destroy_member(cobj_QueueV *const self,
                                void *(*elem_ptr_mut)(void *const arr, size_t idx),
                                void (*elem_destroy)(void *const));
void cobj_QueueV_destroy(cobj_QueueV *const self,
                         void *(*elem_ptr_mut)(void *const arr, size_t idx),
                         void (*elem_destroy)(void *const));

void cobj_QueueV_move_member(cobj_QueueV *const self, cobj_QueueV *const src);
void cobj_QueueV_move(cobj_QueueV *const self, cobj_QueueV *const src);

void cobj_QueueV_new(cobj_QueueV *const self, void *const ptr, size_t alloc);
void cobj_QueueV_clear(cobj_QueueV *const self,
                       void *(*elem_ptr_mut)(void *const arr, size_t idx),
                       void (*elem_destroy)(void *const));

bool cobj_QueueV_is_empty(cobj_QueueV const *const self);
size_t cobj_QueueV_len(cobj_QueueV const *const self);
size_t cobj_QueueV_alloc(cobj_QueueV const *const self);

bool WARN_UNUSED_RESULT cobj_QueueV_put(cobj_QueueV *const self,
                                        void *const elem,
                                        void *(*elem_ptr_mut)(void *const arr, size_t idx),
                                        void (*elem_move)(void *const, void *const));
bool WARN_UNUSED_RESULT cobj_QueueV_get(cobj_QueueV *const self,
                                        void *const item,
                                        void *(*elem_ptr_mut)(void *const arr, size_t idx),
                                        void (*elem_move)(void *const, void *const));

#endif // COBJ_QUEUEV_H
