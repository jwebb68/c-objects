#if !defined(COBJ_CARRAY_H)
#define COBJ_CARRAY_H

#define COBJ_CARRAY_LEN(arr) (sizeof(arr)/sizeof(arr[0]))
#define COBJ_CARRAY_BEGIN(arr) (arr)
#define COBJ_CARRAY_END(arr) (COBJ_CARRAY_BEGIN(arr)+COBJ_CARRAY_LEN(arr))

#endif//!defined(COBJ_CARRAY_H)
