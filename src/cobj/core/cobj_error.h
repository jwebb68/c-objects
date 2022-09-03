#if !defined(COBJ_ERROR_H)
#    define COBJ_ERROR_H

// forward declares
typedef enum cobj_ErrCode_e {
    cobj_ErrorCode_None = 0,
    cobj_ErrorCode_EFAIL,
    cobj_ErrorCode_ENOMEM,
    cobj_ErrorCode_ENOTIMPL,
} cobj_ErrorCode;

typedef struct cobj_Error_s cobj_Error;

// includes
#    include <cobj/core/cobj_defs.h> // WARN_UNUSED_RESULT
#    include <stdbool.h>
#    include <stddef.h> // size_t

// defines
struct cobj_Error_s {
    cobj_ErrorCode code;
    size_t line;
    char const *filen;
};

void cobj_Error_destroy(cobj_Error *const self);
void cobj_Error_new(cobj_Error *const self,
                    cobj_ErrorCode code,
                    size_t line,
                    char const *const filen);
bool WARN_UNUSED_RESULT cobj_Error_raise(cobj_Error *const self,
                                         cobj_ErrorCode code,
                                         size_t line,
                                         char const *const filen);

#    define COBJ_ERROR_RAISE(self, code) cobj_Error_raise(self, code, __LINE__, __FILE__)

#endif //! defined(COBJ_ERROR_H)
