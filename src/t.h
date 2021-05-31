#if !defined(COBJ_T_H)
#    define COBJ_T_H
/**
 * @file
 */

// design notes:
/**
 * functions that can fail
 * -----------------------
 * It is not for the function to determine if a panic is the action to take,
 * so leave that decision to the caller. as such:
 * Must return true/false and set err on fail.
 * Must not assert or panic.
 *
 * Return value
 * ------------
 * If it's a simple type (i.e. non-struct) then can use.
 * Never return a struct, use an outptr and allocate struct memory before call.
 * The reason is that C is very bad at return value optimisation (the rules for it are not defined)
 * and it's easier/simpler to preallocate and pass in as an 'outptr'.
 *
 * C does have the WARN if unused warning, so a return value can be used to signal fails and force
 * callers to handle that fail status (instead of simply ignoring it).
 *
 * function fail and outparams
 * ---------------------------
 * Functions must not leave things to be deallocated by the caller on fail,
 * i.e. on fail must clean up after themselves.
 * They can use the outparam for intermediate work,
 * i.e. on fail, it's not guaranteed that the outparam has not been used
 * (also it's not preserved on fail).
 *
 * On success: any outparams must be initialised.
 * On fail, any outparams must be in an uninitialised state.
 * On Success the err param must be uninitialised.
 * On fail the err param must be initialised.
 *
 * Checking for NULL for outparm locations
 * ---------------------------------------
 * Dont.
 * It's a performance drag to do so.
 * maybe better to write another function that does not use the param
 *
 * struct initialisation/de-initialisation
 * ----------------------------------------
 * structs need initialising and sometimes de-initialising.
 * de-initialisation is needed if there is a member that allocs heap and owns that heap allocation.
 * de-allocation is also needed for any external resources being managed.
 * In C, usually can simply ignore the de-initialisation (depends on the design of the struct)
 *
 * struct allocation
 * -----------------
 * A struct is allocated on the heap/on the stack/in global memory.
 * In this state it is allocated but not initialised.
 * it requires initialisation to be initialised, usually by setting values into it's members,
 * either by init-list initialisation, by assignment or setting members directly. Sometimes, as a
 * hack, memset is used. There are 4+ ways to init: Move an already initialised value into it,
 * de-initing the src struct. Copy an already initialised value into it, not de-initing the src
 * struct, but may fail as it requires resource allocation Create from new with default values.
 * Create from new with parameterised values.
 *
 * Moving
 * ------
 * As copying/cloning requires a resource allocation (if struct has such), then it's safer to move
 * than copy. But move is not the default in C, copy (and bitwise copy at that) is. C does not have
 * the concept of struct initialised or not, so an assignment just copies (assumes the LHS is
 * uninitialised) but then does not mark the RHS as uninitialised, so non-shared pointers are a
 * problem. and moving is implemented using the assignment operation.
 *
 * Care must also be taken when moving into a struct that is already initialised,
 * the dest needs destroying first.
 *
 * Copying/Cloning
 * ---------------
 * Can fail as the copy/clone may not get the resources it need in the copy.
 * So, must always be a try_ and return a worked/failed return value and have an Error outparam.
 * Copy is considered expensive, so should not be used if not needed.
 *
 * Care must also be taken when copying into a struct that is already initialised,
 * the dest needs destroying first.
 * No helper is possible as this would mean the dest is preserved on fail, which, since the copy
 * is a copy in-place then the dest would need preserving first..
 * (unless defined such that on copy fail, the dest is uninitialised)
 */

/**
 * An example of a type/struct.
 * This is a placeholder, substitute your struct for it.
 *
 * You'll optionally need to define (the base set):
 * destroy, move, try_copy and new_ functions
 * others added as needed.
 *
 * Does not handle runtime polymorphism (use interfaces for that (maybe, not thought it through)).
 */

// forward declares
typedef struct T_ T;

// includes
#    include "cobj_defs.h" // WARN_RESULT
#    include "cobj_error.h"

#    include <stdbool.h>
#    include <stddef.h> //size_t

// defines

// a generic struct, replace with your own
struct T_ {
    // some members to keep compiler happy
    // use own as needed
    int dummy;
};

/**
 * Destroy/de-initialise the struct.
 *
 * Destroys+deallocates any heap objects owned by the struct.
 * Releases any other resources owned by the struct.
 *
 * After the call `self` is deemed not initialised.
 *
 * @param self The struct to destroy/de-initialise.
 *
 * @warning UB if self is not readable.
 * @warning UB if self is not writable.
 * @warning UB if self is NULL.
 * @warning UB if self is not initialised.
 */
void T_destroy(T *const self);

/**
 * Move the value of the struct from one memory location to another.
 *
 * After the call, self is deemed initialised.
 * After the call, src is deemed de-initialised.
 *
 * @param self the struct to receive the value.
 * @param src the struct to supply the value.
 *
 * @warning UB if self is not readable.
 * @warning UB if self is not writable.
 * @warning UB if self is NULL.
 * @warning UB if self is initialised.
 *
 * @warning UB if src is not readable.
 * @warning UB if src is not writable.
 * @warning UB if src is NULL.
 * @warning UB if src is not initialised.
 */
void T_move(T *const self, T *const src);

/**
 * Copy/clone the value of the struct.
 *
 * on entry, err is to be de-initialised.
 * on success, err is still de-initialised (do not assume it's not touched).
 * on error, err is initialised.
 * It is the callers responsibility to destroy err if initialised.
 *
 * @param self the struct to receive the copy.
 * @param src the struct to supply the value.
 * @param err the struct to receive error info if failed.
 *
 * @returns true if the copy succeeded.
 * @returns false if the copy fails, in this case err is initialised.
 *
 * @warning UB if self is not readable.
 * @warning UB if self is not writable.
 * @warning UB if self is NULL.
 * @warning UB if self is initialised.
 *
 * @warning UB if src is not readable.
 * @warning UB if src is NULL.
 * @warning UB if src is not initialised.
 *
 * @warning UB if err is not readable.
 * @warning UB if err is not writable.
 * @warning UB if err is NULL.
 * @warning UB if err is initialised.
 */
bool WARN_UNUSED_RESULT T_try_copy(T *const self, T const *const src, Error *const err);
// using WARN_RESULT for force callers to handle the status.

/**
 * Determines if self is equal by value to another.
 *
 * @param self the value to compare.
 * @param b the other value to compare.
 *
 * @return true if self and b are equal in value.
 * @return false if self and b are not equal in value.
 *
 * @warning UB if self is not readable.
 * @warning UB if self is not writable.
 * @warning UB if self is NULL.
 * @warning UB if self is not initialised.
 *
 * @warning UB if b is not readable.
 * @warning UB if b is not writable.
 * @warning UB if b is NULL.
 * @warning UB if b is initialised.
 */
bool T_is_eq(T const *const self, T const *const b);

bool T_is_ne(T const *const self, T const *const b);

/**
 * Determines if self is less than by value to another.
 *
 * @param self the value to compare.
 * @param b the other value to compare.
 *
 * @return true if self and b are less than in value.
 * @return false if self and b are not less than in value.
 *
 * @warning UB if self is not readable.
 * @warning UB if self is not writable.
 * @warning UB if self is NULL.
 * @warning UB if self is not initialised.
 *
 * @warning UB if b is not readable.
 * @warning UB if b is not writable.
 * @warning UB if b is NULL.
 * @warning UB if b is initialised.
 */
bool T_is_lt(T const *const self, T const *const b);

bool T_is_le(T const *const self, T const *const b);

/**
 * Determines if self is greater than by value to another.
 *
 * @param self the value to compare.
 * @param b the other value to compare.
 *
 * @return true if self and b are greater than in value.
 * @return false if self and b are not greater than in value.
 *
 * @warning UB if self is not readable.
 * @warning UB if self is not writable.
 * @warning UB if self is NULL.
 * @warning UB if self is not initialised.
 *
 * @warning UB if b is not readable.
 * @warning UB if b is not writable.
 * @warning UB if b is NULL.
 * @warning UB if b is initialised.
 */
bool T_is_gt(T const *const self, T const *const b);

bool T_is_ge(T const *const self, T const *const b);

/**
 * initialisers,
 * used to initialise the struct, usually from another
 * (so a form of 'from')
 */
/**
 * Initialise the struct using other values.
 *
 * @param self the struct to initialise.
 * @param v the values to use to initialise self.
 *
 * @warning UB if self is not readable.
 * @warning UB if self is not writable.
 * @warning UB if self is NULL.
 * @warning UB if self is initialised.
 */
void T_new_int(T *const self, int v);

/**
 * Initialise the struct using default values.
 *
 * @param self the struct to initialise.
 *
 * @warning UB if self is not readable.
 * @warning UB if self is not writable.
 * @warning UB if self is NULL.
 * @warning UB if self is initialised.
 */
void T_new_default(T *const self);

/**
 * Allocate but don't initialise a struct on the main system heap.
 *
 * a.k.a a typesafe allocator.
 *  * Similar to allocating but not initialising a variable on the stack.
 *
 * @warning It is the callers responsibility to call free() after struct is
 *          de-initialised after use and no longer required.
 *
 * @returns NULL if struct could not be allocated,
 *          not-NULL if allocated ok.
 *
 *
 * @note Consider using BoxT instead of direct heap usage, BoxT calls destroy as needed.
 *
 * @note there is no T_malloc_destroy().
 */
// T *T_malloc(void);
// don't need T_malloc as this is covered in Box

// no T_free(T *) as that's just free()
// T_malloc_new ?, no, use BoxT
// T_free_destroy()? no use BoxT

// array of T manipulation functions

// void ArrayT_destroy_t(T *const arr, size_t len);
// void ArrayT_destroy_p(T *const arr, T *const arr_e);
// void ArrayT_move(T *const arr, T *const src, size_t len);
// bool WARN_UNUSED_RESULT ArrayT_try_copy(T *const arr, T const *const src, size_t len, Error
// *const err); void ArrayT_new_default_i(T *const arr, size_t len); void ArrayT_new_default_p(T
// *const arr, T *const arr_e);

// Slice ?
// Iter/IterMut ?

#endif // defined(COBJ_T_H)
