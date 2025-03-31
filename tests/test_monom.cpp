#include "gtest.h"
#include "polynom.h"

/////////////////////////////////////////

// Многие тесты здесь бесполезны, у монома маленький функционал





TEST(MonomTest, can_create_monom)
{
    ASSERT_NO_THROW(Monom m(42, 1, 2, 3));
}

TEST(MonomTest, throws_exception_for_monom_with_excessive_degrees)
{
    ASSERT_ANY_THROW(Monom m(42, MAX_P, 2, 3));
}

TEST(MonomTest, can_create_monom_with_positive_coef_from_string)
{
    ADD_FAILURE() << "Test not implemented";
}

TEST(MonomTest, can_create_monom_with_negative_coef_from_string)
{
    ADD_FAILURE() << "Test not implemented";
}

TEST(MonomTest, can_create_monom_with_zero_coef_from_string)
{
    ADD_FAILURE() << "Test not implemented";
}

TEST(MonomTest, can_create_monom_with_default_degree)
{
    ADD_FAILURE() << "Test not implemented";
}

TEST(MonomTest, can_create_monom_with_default_positive_coefficient)
{
    ADD_FAILURE() << "Test not implemented";
}

TEST(MonomTest, can_create_monom_with_default_negative_coefficient)
{
    ADD_FAILURE() << "Test not implemented";
}

TEST(MonomTest, throws_exception_for_monom_from_invalid_string)
{
    ADD_FAILURE() << "Test not implemented";
}

TEST(MonomTest, throws_exception_for_monom_from_invalid_string_with_invalid_degrees)
{
    ADD_FAILURE() << "Test not implemented";
}

TEST(MonomTest, throws_exception_for_monom_from_invalid_string_with_excessive_degrees)
{
    ADD_FAILURE() << "Test not implemented";
}

TEST(MonomTest, test_operator_less_than)
{
    ADD_FAILURE() << "Test not implemented";
}

TEST(MonomTest, test_operator_greater_than)
{
    ADD_FAILURE() << "Test not implemented";
}

TEST(MonomTest, equal_operator_returns_true_for_equal_monoms)
{
    ADD_FAILURE() << "Test not implemented";
}

TEST(MonomTest, equal_operator_returns_true_for_unequal_monoms)
{
    ADD_FAILURE() << "Test not implemented";
}

TEST(MonomTest, test_operator_multiply_for_correct_degrees)
{
    ADD_FAILURE() << "Test not implemented";
}

TEST(MonomTest, test_operator_multiply_for_uncorrect_degrees)
{
    ADD_FAILURE() << "Test not implemented";
}

TEST(MonomTest, calculates_correct_value_for_monom)
{
    ADD_FAILURE() << "Test not implemented";
}
