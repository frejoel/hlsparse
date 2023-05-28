/*
 * Copyright 2015 Joel Freeman and other contributors
 * Released under the MIT license http://opensource.org/licenses/MIT
 * see LICENSE included with package
 */

#include "hlsparse.h"
#include "../src/parse.h"
#include "../src/utils.h"
#include "../src/mem.h"
#include "tests.h"
#include <CUnit/Basic.h>

int init(void)
{
    return 0;
}

int clean(void)
{
    return 0;
}

void parse_line_test1(void)
{
    const char *str = "line one\nline two\n\rline three";
    char *dest = NULL;

    int res = parse_line_to_str(str, &dest, strlen(str));
    CU_ASSERT_EQUAL(res, 8);
    CU_ASSERT_EQUAL(strcmp(dest, "line one"), 0);

    hls_free(dest);
    dest = NULL;
    res = parse_line_to_str(&str[9], &dest, strlen(&str[9]));
    CU_ASSERT_EQUAL(res, 8);
    CU_ASSERT_EQUAL(strcmp(dest, "line two"), 0);

    hls_free(dest);
    dest = NULL;
    res = parse_line_to_str(&str[19], &dest, strlen(&str[19]));
    CU_ASSERT_EQUAL(res, 10);
    CU_ASSERT_EQUAL(strcmp(dest, "line three"), 0);

    hls_free(dest);
    dest = NULL;
    res = parse_line_to_str("hello world", &dest, 12);
    CU_ASSERT_EQUAL(res, 11);
    CU_ASSERT_EQUAL(strcmp(dest, "hello world"), 0);

    hls_free(dest);
}

void parse_line_test2(void)
{
    char *dest = NULL;
    int res = parse_line_to_str(NULL, NULL, 0);
    CU_ASSERT_EQUAL(0, res);

    res = parse_line_to_str(NULL, &dest, 0);
    CU_ASSERT_EQUAL(0, res);

    res = parse_line_to_str("test", NULL, 4);
    CU_ASSERT_EQUAL(4, res);

    res = parse_line_to_str("test", NULL, 0);
    CU_ASSERT_EQUAL(res, 0);
}

void parse_int_test(void)
{
    int dest;
    int res = parse_str_to_int("5 dozen\n3 dozen", &dest, 16);
    CU_ASSERT_EQUAL(5, dest);
    CU_ASSERT_EQUAL(res, 1);

    res = parse_str_to_int("42,3\n2", &dest, 6);
    CU_ASSERT_EQUAL(42, dest);
    CU_ASSERT_EQUAL(res, 2);

    res = parse_str_to_int("L42\n2", &dest, 6);
    CU_ASSERT_EQUAL(res, 0);

    res = parse_str_to_int("", &dest, 0);
    CU_ASSERT_EQUAL(res, 0);

    res = parse_str_to_int(NULL, &dest, 0);
    CU_ASSERT_EQUAL(res, 0);

    res = parse_str_to_int("321", NULL, 3);
    CU_ASSERT_EQUAL(res, 3);

    res = parse_str_to_int(NULL, NULL, 3);
    CU_ASSERT_EQUAL(res, 0);
}

void parse_float_test(void)
{
    float dest;
    int res = parse_str_to_float("5.2 dozen\n3.6 dozen", &dest, 20);
    CU_ASSERT_EQUAL(5.2f, dest);
    CU_ASSERT_EQUAL(res, 3);

    res = parse_str_to_float("42.123,3.2\n2", &dest, 12);
    CU_ASSERT_EQUAL(42.123f, dest);
    CU_ASSERT_EQUAL(res, 6);

    res = parse_str_to_float("-66.991\n2", &dest, 9);
    CU_ASSERT_EQUAL(-66.991f, dest);
    CU_ASSERT_EQUAL(res, 7);

    res = parse_str_to_float("21", &dest, 3);
    CU_ASSERT_EQUAL(21.f, dest);
    CU_ASSERT_EQUAL(res, 2);

    dest = 0.f;
    res = parse_str_to_float("L42.1\n2", &dest, 6);
    CU_ASSERT_EQUAL(dest, 0.f);
    CU_ASSERT_EQUAL(res, 0);

    dest = 0.f;
    res = parse_str_to_float("", &dest, 0);
    CU_ASSERT_EQUAL(dest, 0.f);
    CU_ASSERT_EQUAL(res, 0);

    dest = 0.f;
    res = parse_str_to_float(NULL, &dest, 0);
    CU_ASSERT_EQUAL(dest, 0.f);
    CU_ASSERT_EQUAL(res, 0);

    res = parse_str_to_float("321.0", NULL, 5);
    CU_ASSERT_EQUAL(res, 5);

    res = parse_str_to_float(NULL, NULL, 3);
    CU_ASSERT_EQUAL(res, 0);
}

void parse_date_test(void)
{
    uint64_t dest;

    int res = parse_date(NULL, NULL, 0);
    CU_ASSERT_EQUAL(res, 0);

    dest = 0;
    res = parse_date(NULL, &dest, 0);
    CU_ASSERT_EQUAL(res, 0);
    CU_ASSERT_EQUAL(dest, 0);

    dest = 0;
    res = parse_date("invalid date", &dest, 13);
    CU_ASSERT_EQUAL(res, 0);
    CU_ASSERT_EQUAL(dest, 0);

    dest = 0;
    res = parse_date("2015-02-27T09:25", NULL, 0);
    CU_ASSERT_EQUAL(res, 0);
    CU_ASSERT_EQUAL(dest, 0);

    res = parse_date("2015", &dest, 4);
    CU_ASSERT_EQUAL(res, 4);
    CU_ASSERT_EQUAL(dest, 1420070400000);

    res = parse_date("2014-08", &dest, 7);
    CU_ASSERT_EQUAL(res, 7);
    CU_ASSERT_EQUAL(dest, 1406851200000);

    res = parse_date("2013-06-05", &dest, 10);
    CU_ASSERT_EQUAL(res, 10);
    CU_ASSERT_EQUAL(dest, 1370390400000);

    res = parse_date("2012-01-26T", &dest, 11);
    CU_ASSERT_EQUAL(res, 11);
    CU_ASSERT_EQUAL(dest, 1327536000000);

    res = parse_date("2011-02-13T02:30", &dest, 16);
    CU_ASSERT_EQUAL(res, 16);
    CU_ASSERT_EQUAL(dest, 1297564200000);

    res = parse_date("2010-09-15T20:07:32", &dest, 19);
    CU_ASSERT_EQUAL(res, 19);
    CU_ASSERT_EQUAL(dest, 1284581252000);

    res = parse_date("2009-08-23T03:29:38+00:00", &dest, 26);
    CU_ASSERT_EQUAL(res, 25);
    CU_ASSERT_EQUAL(dest, 1250998178000);

    res = parse_date("2008-09-19T00:29:30.07+02:30", &dest, 28);
    CU_ASSERT_EQUAL(res, 28);
    CU_ASSERT_EQUAL(dest, 1221775170070);

    res = parse_date("1970-08-18T01:28:29.06-01:00, is a date", &dest, 39);
    CU_ASSERT_EQUAL(res, 28);
    CU_ASSERT_EQUAL(dest, 19794509060);
}

void parse_attrib_str_test(void)
{
    char *dest = NULL;
    int res = parse_attrib_str("", &dest, 0);
    CU_ASSERT_EQUAL(dest, NULL);
    CU_ASSERT_EQUAL(res, 0);

    dest = NULL;
    res = parse_attrib_str(NULL, &dest, 0);
    CU_ASSERT_EQUAL(res, 0);
    CU_ASSERT_EQUAL(dest, NULL);

    dest = NULL;
    res = parse_attrib_str("\"hello world\"", NULL, 14 );
    CU_ASSERT_EQUAL(res, 13);
    CU_ASSERT_EQUAL(dest, NULL);

    dest = NULL;
    res = parse_attrib_str("\"hello world\"", &dest, 14);
    CU_ASSERT_EQUAL(res, 13);
    CU_ASSERT_EQUAL(strcmp(dest, "hello world"), 0 );
    hls_free(dest);

    dest = NULL;
    res = parse_attrib_str("before \"after\"", &dest, 14);
    CU_ASSERT_EQUAL(res, 14);
    CU_ASSERT_EQUAL(strcmp(dest, "after"), 0);

    dest = NULL;
    res = parse_attrib_str("\"before\" after", &dest, 14);
    CU_ASSERT_EQUAL(res, 8);
    CU_ASSERT_EQUAL(strcmp(dest, "before"), 0);
    hls_free(dest);

    dest = NULL;
    res = parse_attrib_str("\"one\",\"two\",\"three\"", &dest, 20);
    CU_ASSERT_EQUAL(res, 5);
    CU_ASSERT_EQUAL(strcmp(dest, "one"), 0);
    hls_free(dest);
}

void parse_attrib_data_test(void)
{
    char *dest = NULL;
    int res = parse_attrib_data("", &dest, 0);
    CU_ASSERT_EQUAL(res, 0);
    CU_ASSERT_EQUAL(dest, NULL);

    dest = NULL;
    res = parse_attrib_data(NULL, &dest, 0);
    CU_ASSERT_EQUAL(res, 0);
    CU_ASSERT_EQUAL(dest, NULL);

    dest = NULL;
    res = parse_attrib_data("0xFFFFFFFF", NULL, 10);
    CU_ASSERT_EQUAL(res, 10);
    CU_ASSERT_EQUAL(dest, NULL);

    dest = NULL;
    res = parse_attrib_data("0x474747", &dest, 8);
    CU_ASSERT_EQUAL(res, 8);
    CU_ASSERT_EQUAL(strcmp(dest, "GGG"), 0);

    dest = NULL;
    res = parse_attrib_data("invalid 0x474747", &dest, 17);
    CU_ASSERT_EQUAL(res, 0);
    CU_ASSERT_EQUAL(dest, NULL);

    dest = NULL;
    res = parse_attrib_data("0x474747 after", &dest, 15);
    CU_ASSERT_EQUAL(res, 8);
    CU_ASSERT_EQUAL(strcmp(dest, "GGG"), 0);
}

void setup()
{
    hlsparse_global_init();
    
    suite("parse", init, clean);
    test("parse_line1", parse_line_test1);
    test("parse_line2", parse_line_test2);
    test("parse_int", parse_int_test);
    test("parse_float", parse_float_test);
    test("parse_date", parse_date_test);
    test("parse_attrib_str", parse_attrib_str_test);
    test("parse_attrib_data", parse_attrib_data_test);
}

