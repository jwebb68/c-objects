#if !defined(COBJ_DEFS_H)
#    define COBJ_DEFS_H

#    include <stddef.h> // size_t

// an cobj_Index is not a size
// so need to prevent assignment/comparision etc.
typedef size_t cobj_Index;

#    define COBJ_UNUSED_ARG(x) \
        do { \
            ((void)(x)); \
        } while (0)

#    define WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#    define NO_RETURN __attribute__((noreturn))

#endif //! defined(COBJ_DEFS_H)
