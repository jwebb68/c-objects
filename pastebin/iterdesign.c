// some notes on vector iter design

// or would it be better to use something like jinja2 templates to just create
// a file with the specialisation in it?

// alternative iters
// what I want is:
// void foo() {
//     for (veciter it = vec.iter(); it.is_done(); it.next()) {
//         it.item().
//     }
// }
// but in C.
typedef struct vec2_ vec2;
struct vec2_ {
    T *arr;
    size_t alloc;
    size_t len;
};
typedef struct iter2_ iter2;
struct iter2_ {
   vec2 *vec;
   T *it;
};
void iter2_destroy(iter2 *const self) {
    STRUCTWIPE(self);
}
// maybe can return a iter, so long as it's max 2 ptrs big
// no, makes destorying iter after use harder.. (it still has to be out of
// the for-loop scope
void vec2_iter(vec2 *const self, iter2 *const it) {
    iter2_new(it, self);
}
iter2 vec2_iter2(vec2 *const self) {
    return iter2_new2(self);
}
bool iter2_is_done(iter2 const *const self) {
    // could use end instead of vec in iter
    // make this a const..
    T * const end = (self->vec->arr + self->vec->len);
    return (self->it != end);
}
// one compare per iteration of forloop
void iter2_next(iter2 *const self) {
    // wot no check it is not at end?
    T * const end = (self->vec->arr + self->vec->len);
    if (self->it != end) {
        ++self->it;
    }
}
T *iter2_item(iter2 *const self) {
    return self->it;
}
void foo21() {
    vec2 v;
    iter2 it;
    for (vec2_iter(&v, &it); iter2_is_done(&it); iter2_next(&it)) {
        iter2_item(&it)->...
    }
    iter2_destroy(&it);
}
// more typing, but faster (1 compare, one advance, one deref)
// more typing, a little faster (2 compares, one advance, one deref)
// more typing ans less safe.

void foo22() {
    vec2 v;
    for (iter2 it = vec2_iter2(&v); iter2_is_done(&it); iter2_next(&it)) {
        iter2_item(&it)->...
    }
    iter2_destroy(&it);
    // ah, prob, 'it' is not in scope at this point, and this is the only place to
    // destroy it.
}
//or

typedef struct iter1_ iter1;
struct iter1_ {
};
void iter1_destroy(iter1 *const self);
void iter1_new(iter1 *const self, ...);
// return true: val is valid and there's more
// return false: val not valid and no more.
bool iter1_next(iter1 *const self, T **val);
//usage
void main2() {
    iter1 it;
    iter1_new(&it);
    T *t;
    for (; iter1_next(&it, &t); ) {
        // do something with t
        //t-> ...
    }
    iter1_destroy(&it)
}
// too much in the outer scope, it's not contained in the for()
// i.e. t and it are not contained in for-loop scope
