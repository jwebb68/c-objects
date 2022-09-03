#include "cobj_boxt.h"

#include <cobj/core/cobj_memory.h>

// https://libcheck.github.io/check/
#include <check.h>

START_TEST(test_destroy)
{
    T t1;
    T_default(&t1);

    cobj_Error err;
    bool ok;
    cobj_BoxT bt;
    ok = cobj_BoxT_try_from_T(&bt, &t1, &err);
    ck_assert(ok);
    if (!ok) { cobj_Error_destroy(&err); }

    // can destroy an initialised box.
    cobj_BoxT_destroy(&bt);

    // TODO: how to check that thing boxed has been destroyed..
}
END_TEST

START_TEST(test_move)
{
    T t1;
    T_default(&t1);

    cobj_Error err;
    cobj_BoxT bt1;
    bool ok;
    ok = cobj_BoxT_try_from_T(&bt1, &t1, &err);
    ck_assert(ok);
    if (!ok) { cobj_Error_destroy(&err); }

    cobj_BoxT bt2;
    cobj_BoxT_move(&bt2, &bt1);

    // moving changes value in src to be something different from what's in dest.
    // ackkk: bt2 now uninitialised..
    // TODO: need better way to detect (as failing in is_ne since one ptr is invalid.
    // ck_assert(T_is_ne(cobj_BoxT_deref(&bt2), cobj_BoxT_deref(&bt1)));

    // TODO: how to check that src is deinitialised now..

    cobj_BoxT_destroy(&bt2);
}
END_TEST

START_TEST(test_try_copy)
{
    T t1;
    T_default(&t1);

    cobj_Error err;
    bool ok;
    cobj_BoxT bt1;
    ok = cobj_BoxT_try_from_T(&bt1, &t1, &err);
    ck_assert(ok);
    if (!ok) { cobj_Error_destroy(&err); }

    // maybe need to use a different T for tests, so can mock copy fail handlings..
    cobj_BoxT bt2;
    ok = cobj_BoxT_try_copy(&bt2, &bt1, &err);
    ck_assert(ok);
    if (!ok) { cobj_Error_destroy(&err); }

    // TODO: how to test copy failing?
    // on fail, src must not be moved, dest should stay uninitialsed.

    ck_assert(T_is_eq(cobj_BoxT_deref(&bt2), cobj_BoxT_deref(&bt1)));

    cobj_BoxT_destroy(&bt2);
    cobj_BoxT_destroy(&bt1);
}
END_TEST

START_TEST(test_try_from_T)
{
    // create a T, so we can move it in
    T t1, t2;
    T_default(&t1);
    T_default(&t2);

    cobj_Error err;
    cobj_BoxT bt;
    bool ok;
    // given a T, must be able to move it into a box.
    // but only on success.
    ok = cobj_BoxT_try_from_T(&bt, &t1, &err);
    ck_assert(ok);
    if (!ok) { cobj_Error_destroy(&err); }

    // value is in box.
    ck_assert(T_is_eq(&t2, cobj_BoxT_deref(&bt)));

    //? how to fail the try_from_T?
    // so to test the not moving on fail.

    // moving in a NULL is undefined.

    cobj_BoxT_destroy(&bt);
    // no destroy of t1, it was moved into bt;
    // was destroyed in bt
    T_destroy(&t2);
}
END_TEST

START_TEST(test_own)
{
    cobj_Error err;
    T *p = cobj_try_malloc(sizeof(T), &err);

    cobj_BoxT bt;
    // must be able to own an existing non-boxed heap obj.
    cobj_BoxT_own(&bt, p);

    ck_assert(cobj_BoxT_deref(&bt) == p);

    // owning a NULL is undefined.

    cobj_BoxT_destroy(&bt);
}
END_TEST

START_TEST(test_disown)
{
    cobj_Error err;
    T *p = cobj_try_malloc(sizeof(T), &err);
    T *q;

    cobj_BoxT bt;
    // must be able to own an existing non-boxed heap obj.
    cobj_BoxT_own(&bt, p);
    // ackk, p: use after consumed..
    ck_assert(cobj_BoxT_deref(&bt) == p);

    // implicitly destroys box.
    q = cobj_BoxT_disown(&bt);

    // ackk, p: use after consumed..
    ck_assert(q == p);
    // ackk, p: use after consumed..
    // ackk, bt: use after destroyed..
    ck_assert(cobj_BoxT_deref(&bt) != p);

    free(q);
}
END_TEST

Suite *cobj_BoxT_suite(void)
{
    Suite *s;
    TCase *tc;
    s = suite_create("cobj_BoxT");
    tc = tcase_create("Core");
    tcase_add_test(tc, test_destroy);
    tcase_add_test(tc, test_move);
    tcase_add_test(tc, test_try_copy);
    tcase_add_test(tc, test_try_from_T);
    tcase_add_test(tc, test_own);
    tcase_add_test(tc, test_disown);
    suite_add_tcase(s, tc);
    return s;
}

int main(int argc, char *argv[])
{
    ((void)(argc));
    ((void)(argv));

    int n_failed;
    Suite *s;
    SRunner *r;

    s = cobj_BoxT_suite();
    r = srunner_create(s);
    srunner_set_fork_status(r, CK_NOFORK);
    srunner_run_all(r, CK_NORMAL);
    n_failed = srunner_ntests_failed(r);
    srunner_free(r);

    return (n_failed == 0) ? 0 : 1;
}
