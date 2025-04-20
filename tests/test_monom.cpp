#include "gtest.h"
#include "polynom.h"

/////////////////////////////////////////
TEST(MonomTest, can_create_valid_monom) {
    ASSERT_NO_THROW(Monom(5.0, 2, 1, 3));
}

TEST(MonomTest, throws_on_invalid_variable_index) {
    Monom m(1.0, 1, 2, 3);
    ASSERT_ANY_THROW(m.degOf(0));
    ASSERT_ANY_THROW(m.degOf(4));
}

TEST(MonomTest, equality_operator_works_correctly) {
    Monom m1(2.5, 1, 2, 3);
    Monom m2(2.5, 1, 2, 3);
    Monom m3(2.5, 2, 1, 3);
    ASSERT_TRUE(m1 == m2);
    ASSERT_FALSE(m1 == m3);
}

TEST(MonomTest, multiplication_by_scalar_works) {
    const Monom m(3.0, 1, 0, 0);
    Monom result = m * 2;
    ASSERT_EQ(result.coef(), 6.0);
    ASSERT_EQ(result.degOf(1), 1);
}

TEST(MonomTest, multiplication_of_monoms_adds_degrees) {
    Monom m1(2.0, 1, 2, 0);
    Monom m2(3.0, 0, 1, 4);
    const Monom result = m1 * m2;
    ASSERT_EQ(result.coef(), 6.0);
    ASSERT_EQ(result.degOf(1), 1);
    ASSERT_EQ(result.degOf(2), 3);
    ASSERT_EQ(result.degOf(3), 4);
}
