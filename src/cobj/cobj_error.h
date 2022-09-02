#if !defined(COBJ_ERROR_H)
#    define COBJ_ERROR_H

// forward declares
typedef enum ErrCode_ {
    Error_None = 0,
    Error_EFAIL,
    Error_ENOMEM,
    Error_ENOTIMPL,
} ErrorCode;

typedef struct Error_ Error;

// includes
#    include "cobj_defs.h" // WARN_UNUSED_RESULT

#    include <stdbool.h>
#    include <stddef.h> // size_t

// defines
struct Error_ {
    ErrorCode code;
    size_t line;
    char const *filen;
};

void Error_destroy(Error *const self);
void Error_new(Error *const self, ErrorCode code, size_t line, char const *const filen);
bool WARN_UNUSED_RESULT Error_raise(Error *const self,
                                    ErrorCode code,
                                    size_t line,
                                    char const *const filen);

#    define ERROR_RAISE(self, code) Error_raise(self, code, __LINE__, __FILE__)

#endif //! defined(COBJ_ERROR_H)
