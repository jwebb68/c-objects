#include "cobj_boxt.h"

#include <cobj/core/cobj_memory.h>

// https://libcheck.github.io/check/
#include <check.h>

START_TEST(test_destroy)
{
    T t1;
    T_default(&t1);

    Error err;
    bool ok;
    BoxT bt;
    ok = BoxT_try_from_T(&bt, &t1, &err);
    ck_assert(ok);
    if (!ok) { Error_destroy(&err); }

    // can destroy an initialised box.
    BoxT_destroy(&bt);

    // TODO: how to check that thing boxed has been destroyed..
}
END_TEST

START_TEST(test_move)
{
    T t1;
    T_default(&t1);

    Error err;
    BoxT bt1;
    bool ok;
    ok = BoxT_try_from_T(&bt1, &t1, &err);
    ck_assert(ok);
    if (!ok) { Error_destroy(&err); }

    BoxT bt2;
    BoxT_move(&bt2, &bt1);

    // moving changes value in src to be something different from what's in dest.
    // ackkk: bt2 now uninitialised..
    // TODO: need better way to detect (as failing in is_ne since one ptr is invalid.
    // ck_assert(T_is_ne(BoxT_deref(&bt2), BoxT_deref(&bt1)));

    // TODO: how to check that src is deinitialised now..

    BoxT_destroy(&bt2);
}
END_TEST

START_TEST(test_try_copy)
{
    T t1;
    T_default(&t1);

    Error err;
    bool ok;
    BoxT bt1;
    ok = BoxT_try_from_T(&bt1, &t1, &err);
    ck_assert(ok);
    if (!ok) { Error_destroy(&err); }

    // maybe need to use a different T for tests, so can mock copy fail handlings..
    BoxT bt2;
    ok = BoxT_try_copy(&bt2, &bt1, &err);
    ck_assert(ok);
    if (!ok) { Error_destroy(&err); }

    // TODO: how to test copy failing?
    // on fail, src must not be moved, dest should stay uninitialsed.

    ck_assert(T_is_eq(BoxT_deref(&bt2), BoxT_deref(&bt1)));

    BoxT_destroy(&bt2);
    BoxT_destroy(&bt1);
}
END_TEST

START_TEST(test_try_from_T)
{
    // create a T, so we can move it in
    T t1, t2;
    T_default(&t1);
    T_default(&t2);

    Error err;
    BoxT bt;
    bool ok;
    // given a T, must be able to move it into a box.
    // but only on success.
    ok = BoxT_try_from_T(&bt, &t1, &err);
    ck_assert(ok);
    if (!ok) { Error_destroy(&err); }

    // value is in box.
    ck_assert(T_is_eq(&t2, BoxT_deref(&bt)));

    //? how to fail the try_from_T?
    // so to test the not moving on fail.

    // moving in a NULL is undefined.

    BoxT_destroy(&bt);
    // no destroy of t1, it was moved into bt;
    // was destroyed in bt
    T_destroy(&t2);
}
END_TEST

START_TEST(test_own)
{
    Error err;
    T *p = try_malloc(sizeof(T), &err);

    BoxT bt;
    // must be able to own an existing non-boxed heap obj.
    BoxT_own(&bt, p);

    ck_assert(BoxT_deref(&bt) == p);

    // owning a NULL is undefined.

    BoxT_destroy(&bt);
}
END_TEST

START_TEST(test_disown)
{
    Error err;
    T *p = try_malloc(sizeof(T), &err);
    T *q;

    BoxT bt;
    // must be able to own an existing non-boxed heap obj.
    BoxT_own(&bt, p);
    // ackk, p: use after consumed..
    ck_assert(BoxT_deref(&bt) == p);

    // implicitly destroys box.
    q = BoxT_disown(&bt);

    // ackk, p: use after consumed..
    ck_assert(q == p);
    // ackk, p: use after consumed..
    // ackk, bt: use after destroyed..
    ck_assert(BoxT_deref(&bt) != p);

    free(q);
}
END_TEST

Suite *boxt_suite(void)
{
    Suite *s;
    TCase *tc;
    s = suite_create("BoxT");
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

    s = boxt_suite();
    r = srunner_create(s);
    srunner_set_fork_status(r, CK_NOFORK);
    srunner_run_all(r, CK_NORMAL);
    n_failed = srunner_ntests_failed(r);
    srunner_free(r);

    return (n_failed == 0) ? 0 : 1;
}
