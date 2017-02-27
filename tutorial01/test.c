#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leptjson.h"

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

#define EXPECT_EQ_BASE(equality, expect, actual, format) \
    do {\
        test_count++;\
        if (equality)\
            test_pass++;\
        else {\
            fprintf(stderr, "%s:%d: expect: " format " actual: " format "\n", __FILE__, __LINE__, expect, actual);\
            main_ret = 1;\
        }\
    } while(0)

#define EXPECT_EQ_INT(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%d")

//能否正确解析单值类型(null,true,false)
static void test_parse_single_type(const char *c, lept_type expected_type) {
    lept_value v;
    v.type = LEPT_NULL;
    EXPECT_EQ_INT(LEPT_PARSE_OK, lept_parse(&v, c));
    EXPECT_EQ_INT(expected_type, lept_get_type(&v));
}
//只有空格或空串
static void test_parse_expect_value() {
    lept_value v;

    v.type = LEPT_FALSE;
    EXPECT_EQ_INT(LEPT_PARSE_EXPECT_VALUE, lept_parse(&v, ""));
    EXPECT_EQ_INT(LEPT_NULL, lept_get_type(&v));

    v.type = LEPT_FALSE;
    EXPECT_EQ_INT(LEPT_PARSE_EXPECT_VALUE, lept_parse(&v, " "));
    EXPECT_EQ_INT(LEPT_NULL, lept_get_type(&v));
}
//不合理的值
static void test_parse_invalid_value() {
    lept_value v;
    v.type = LEPT_FALSE;
    EXPECT_EQ_INT(LEPT_PARSE_INVALID_VALUE, lept_parse(&v, "nul"));
    EXPECT_EQ_INT(LEPT_NULL, lept_get_type(&v));

    v.type = LEPT_FALSE;
    EXPECT_EQ_INT(LEPT_PARSE_INVALID_VALUE, lept_parse(&v, "?"));
    EXPECT_EQ_INT(LEPT_NULL, lept_get_type(&v));
}
//多个值
static void test_parse_root_not_singular() {
    lept_value v;
    v.type = LEPT_FALSE;
    EXPECT_EQ_INT(LEPT_PARSE_ROOT_NOT_SINGULAR, lept_parse(&v, "null x"));
    EXPECT_EQ_INT(LEPT_NULL, lept_get_type(&v));
}
/* 
 * 只能解析形如"ws value ws"形式的json串
 * 解析成功返回LEPT_PARSE_OK
 * 只有空格返回LEPT_PARSE_EXPECT_VALUE
 * 不合理的值返回LEPT_PARSE_INVALID_VALUE
 * 有多个值返回LEPT_PARSE_ROOT_NOT_SINGULAR
 *
 * 解析成功的类型即为解析得到的值的类型
 * 解析失败v的类型为LEPT_NULL
 */
static void test_parse() {
    test_parse_single_type(" null ", LEPT_NULL);
    test_parse_single_type(" true ", LEPT_TRUE);
    test_parse_single_type(" \n\n\r\n\tfalse ", LEPT_FALSE);
    test_parse_expect_value();
    test_parse_invalid_value();
    test_parse_root_not_singular();
}

int main() {
    test_parse();
    printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
    return main_ret;
}
