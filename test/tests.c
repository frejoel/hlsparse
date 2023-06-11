/*
 * Copyright 2023 Joel Freeman and other contributors
 * Released under the MIT license http://opensource.org/licenses/MIT
 * see LICENSE included with package
 */

#include "tests.h"
#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>

/*
 * CUnit Test Suite
 */

CU_pSuite pSuite = NULL;

int run(void);

int main()
{

    /* Initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    setup();
    return run();
}

int default_suite_init_clean(void)
{

    return 0;
}

int suite(const char *name, suite_t init, suite_t clean)
{

    if (!init) {
        init = default_suite_init_clean;
    }
    if (!clean) {
        clean = default_suite_init_clean;
    }

    /* Add a suite to the registry */
    pSuite = CU_add_suite(name, init, clean);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    return 0;
}

int test(const char *name, test_t test)
{

    /* Add the tests to the suite */
    if (NULL == CU_add_test(pSuite, name, test)) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    return 0;
}

int run()
{

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    int nb_tests_failed = CU_get_number_of_tests_failed();
    CU_cleanup_registry();
    return nb_tests_failed > 0 ? 1 : 0;
}

void assert_string_equal(const char *a, const char *b, const char *msg, int line)
{
    if (a != NULL && b != NULL) {
        CU_ASSERT_STRING_EQUAL(a,b);
    } else {
        char buf[128];
        snprintf(buf, 128, "%s:%d", msg, line);
        if (a != b) {
            CU_FAIL(buf);
        } else {
            CU_PASS(buf);
        }
    }
}

