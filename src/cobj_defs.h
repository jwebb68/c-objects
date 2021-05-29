#if !defined(COBJ_DEFS_H)
#    define COBJ_DEFS_H

#    define WARN_RESULT

#    include <stddef.h> // size_t
// an Index is not a size
// so need to prevent assignment/comparision etc.
typedef size_t Index;

#    define UNUSED_ARG(x) \
        do { \
            ((void)(x)); \
        } while (0)

#    define WARN_UNUSED_RESULT __attribute__((warn_unused_result))

#endif //! defined(COBJ_DEFS_H)
