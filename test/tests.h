/*
 * Copyright 2023 Joel Freeman and other contributors
 * Released under the MIT license http://opensource.org/licenses/MIT
 * see LICENSE included with package
 */

#ifndef _TESTS_H
#define _TESTS_H

#include <CUnit/Basic.h>

// CU_ASSERT_STRING_EQUAL_2(a,b)   CU_ASSERT_STRING_EQUAL((const char *)(((a) != NULL) ? (a) : ""),(const char *)(((b)!=NULL) ? (b) : ""))

typedef int (*suite_t)(void);
typedef void (*test_t)(void);

int suite(const char *name, suite_t init, suite_t clean);
int test(const char *name, test_t);

void assert_string_equal(const char *a, const char *b, const char *msg, int line);

void setup(void);

#endif
