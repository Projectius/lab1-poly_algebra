#include "gtest.h"
#include "polynom.h"

TEST(PolynomTest, can_create_polynom) {
    ASSERT_NO_THROW(Polynom p);
}

TEST(PolynomTest, can_fill_polynom_with_monomes) {
    Polynom p;
    ASSERT_NO_THROW({
        p.insert(Monom(42, 1, 2, 3));
        p.insert(Monom(55, 0, 0, 0));
        p.insert(Monom(-33, 4, 0, 3));
        });
}

TEST(PolynomTest, throws_on_excessive_degrees) {
    ASSERT_ANY_THROW(Monom(1.0, 20, 0, 0)); // x >= MAX_P
    ASSERT_ANY_THROW(Monom(1.0, 0, 20, 0));
    ASSERT_ANY_THROW(Monom(1.0, 0, 0, 20));
}

TEST(PolynomTest, copy_constructor_creates_equal_copy) {
    Polynom p;
    p.insert(Monom(1, 1, 1, 1));
    p.insert(Monom(2, 0, 0, 0));
    Polynom p_copy(p);
    ASSERT_EQ(p, p_copy);
}

TEST(PolynomTest, copy_constructor_creates_independent_copy) {
    Polynom p;
    p.insert(Monom(1, 1, 1, 1));
    Polynom p_copy(p);
    p_copy.insert(Monom(2, 2, 0, 0));
    ASSERT_NE(p, p_copy);
}

TEST(PolynomTest, assignment_operator_creates_equal_copy) {
    Polynom p;
    p.insert(Monom(5, 2, 1, 0));
    Polynom other;
    other = p;
    ASSERT_EQ(p, other);
}

TEST(PolynomTest, assignment_operator_creates_independent_copy) {
    Polynom p;
    p.insert(Monom(5, 2, 1, 0));
    Polynom other;
    other = p;
    other.insert(Monom(1, 1, 1, 1));
    ASSERT_NE(p, other);
}

TEST(PolynomTest, self_assignment_does_not_crash) {
    Polynom p;
    p.insert(Monom(5, 1, 1, 1));
    ASSERT_NO_THROW(p = p);
}

TEST(PolynomTest, equality_operator_works) {
    Polynom p1, p2;
    p1.insert(Monom(3, 1, 0, 0));
    p2.insert(Monom(3, 1, 0, 0));
    ASSERT_TRUE(p1 == p2);

    p2.insert(Monom(1, 0, 1, 0));
    ASSERT_FALSE(p1 == p2);
}

TEST(PolynomTest, addition_operator_works) {
    Polynom p1, p2;
    p1.insert(Monom(3, 1, 0, 0));
    p2.insert(Monom(2, 1, 0, 0));
    Polynom result = p1 + p2;
    Polynom expected;
    expected.insert(Monom(5, 1, 0, 0));
    ASSERT_EQ(result, expected);
}

TEST(PolynomTest, subtraction_operator_works) {
    Polynom p1, p2;
    p1.insert(Monom(3, 1, 0, 0));
    p2.insert(Monom(2, 1, 0, 0));
    Polynom result = p1 - p2;
    Polynom expected;
    expected.insert(Monom(1, 1, 0, 0));
    ASSERT_EQ(result, expected);
}

TEST(PolynomTest, multiplication_by_scalar_works) {
    Polynom p;
    p.insert(Monom(3, 1, 0, 0));
    p.insert(Monom(2, 0, 1, 0));
    Polynom result = p * 2;
    Polynom expected;
    expected.insert(Monom(6, 1, 0, 0));
    expected.insert(Monom(4, 0, 1, 0));
    ASSERT_EQ(result, expected);
}

TEST(PolynomTest, multiplication_by_zero_gives_zero_polynom) {
    Polynom p;
    p.insert(Monom(3, 1, 0, 0));
    Polynom result = p * 0;
    Polynom expected(0);
    ASSERT_EQ(result, expected);
}
