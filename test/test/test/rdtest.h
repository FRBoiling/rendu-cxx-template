/*
* Created by boil on 2023/2/19.
*/

#ifndef TEST_TEST_TEST_RDTEST_H_
#define TEST_TEST_TEST_RDTEST_H_

#include <gtest/gtest.h>

#ifdef NDEBUG
#    define RD_DEBUG_TEST(Case, Test) RD_TEST(Case, DISABLED_##Test)
#    define RD_DEBUG_TEST_F(Case, Test) RD_TEST_F(Case, DISABLED_##Test)
#    define RD_DEBUG_TYPED_TEST(Case, Test) TYPED_TEST(Case, DISABLED_##Test)
#else
#    define RD_DEBUG_TEST(Case, Test) RD_TEST(Case, Test)
#    define RD_DEBUG_TEST_F(Case, Test) RD_TEST_F(Case, Test)
#    define RD_DEBUG_TYPED_TEST(Case, Test) TYPED_TEST(Case, Test)
#endif

#define RD_TEST(test_suite_name, test_name) TEST(test_suite_name, test_name)
#define RD_TEST_F(test_fixture, test_name) TEST_F(test_fixture, test_name)

#define RD_EXPECT_NE(val1, val2)  EXPECT_NE(val1, val2)
#define RD_EXPECT_EQ(val1, val2)  EXPECT_EQ(val1, val2)
#define RD_EXPECT_LE(val1, val2)  EXPECT_LE(val1, val2)
#define RD_EXPECT_LT(val1, val2)  EXPECT_LT(val1, val2)
#define RD_EXPECT_GE(val1, val2)  EXPECT_GE(val1, val2)
#define RD_EXPECT_GT(val1, val2)  EXPECT_GT(val1, val2)
#define RD_EXPECT_TRUE(condition) EXPECT_TRUE(condition)
#define RD_EXPECT_FALSE(condition) EXPECT_FALSE(condition)

#define RD_ASSERT_NE(val1, val2) ASSERT_NE(val1, val2)
#define RD_ASSERT_EQ(val1, val2) ASSERT_EQ(val1, val2)
#define RD_ASSERT_NE(val1, val2) ASSERT_NE(val1, val2)
#define RD_ASSERT_LE(val1, val2) ASSERT_LE(val1, val2)
#define RD_ASSERT_LT(val1, val2) ASSERT_LT(val1, val2)
#define RD_ASSERT_GE(val1, val2) ASSERT_GE(val1, val2)
#define RD_ASSERT_GT(val1, val2) ASSERT_GT(val1, val2)
#define RD_ASSERT_FALSE(condition) ASSERT_FALSE(condition)
#define RD_ASSERT_TRUE(condition) ASSERT_TRUE(condition)

#define RD_ASSERT_NO_FATAL_FAILURE(statement) ASSERT_NO_FATAL_FAILURE(statement)
#define RD_EXPECT_NO_FATAL_FAILURE(statement) EXPECT_NO_FATAL_FAILURE(statement)

#define RD_ASSERT_DEATH(statement, matcher) ASSERT_DEATH(statement, matcher)
#define RD_EXPECT_DEATH(statement, matcher) EXPECT_DEATH(statement, matcher)

#define RD_EXPECT_THROW(statement, expected_exception) EXPECT_THROW(statement, expected_exception)
#define RD_EXPECT_NO_THROW(statement) EXPECT_NO_THROW(statement)
#define RD_EXPECT_ANY_THROW(statement) EXPECT_ANY_THROW(statement)

#define RD_ASSERT_THROW(statement, expected_exception) ASSERT_THROW(statement, expected_exception)
#define RD_ASSERT_NO_THROW(statement) ASSERT_NO_THROW(statement)
#define RD_ASSERT_ANY_THROW(statement) ASSERT_ANY_THROW(statement)

#endif //TEST_TEST_TEST_RDTEST_H_
