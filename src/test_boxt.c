#include "cobj_boxt.h"

#include <check.h>

START_TEST(test_try_new_from_T)
{
    // create a T, so we can move it in
    T t1, t2;
    T_new_default(&t1);
    T_new_default(&t2);

    Error err;
    BoxT bt;
    bool ok;
    ok = BoxT_try_new_from_T(&bt, &t1, &err);
    ck_assert(ok);

    // value is in box.
    ck_assert(T_is_eq(&t2, BoxT_deref(&bt)));
    ck_assert(T_is_ne(&t1, BoxT_deref(&bt)));

    BoxT_destroy(&bt);
    // no destroy of t1, it was moved into bt;
    // was destroyed in bt
    T_destroy(&t2);
}
END_TEST

Suite *boxt_suite(void)
{
    Suite *s;
    TCase *tc;
    s = suite_create("BoxT");
    tc = tcase_create("Core");
    tcase_add_test(tc, test_try_new_from_T);
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
    srunner_run_all(r, CK_NORMAL);
    n_failed = srunner_ntests_failed(r);
    srunner_free(r);

    return (n_failed == 0) ? 0 : 1;
}
