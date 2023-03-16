/*
* Created by boil on 2023/2/19.
*/

#ifndef RENDU_TEST_TEST_H_
#define RENDU_TEST_TEST_H_

#include <gtest/gtest.h>
#include <test/throwing_allocator.h>
#include <test/throwing_type.h>

#ifdef NDEBUG
#    define RD_DEBUG_TEST(Case, Test) TEST(Case, DISABLED_##Test)
#    define RD_DEBUG_TEST_F(Case, Test) TEST_F(Case, DISABLED_##Test)
#    define RD_DEBUG_TYPED_TEST(Case, Test) TYPED_TEST(Case, DISABLED_##Test)
#else
#    define RD_DEBUG_TEST(Case, Test) TEST(Case, Test)
#    define RD_DEBUG_TEST_F(Case, Test) TEST_F(Case, Test)
#    define RD_DEBUG_TYPED_TEST(Case, Test) TYPED_TEST(Case, Test)
#endif

#endif //RENDU_TEST_TEST_H_
