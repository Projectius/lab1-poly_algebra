#include "gtest.h"
#include "../include/postfix.h"

//TEST(PostfixTest, can_create_postfix)
//{
//    ASSERT_NO_THROW(Postfix p(););
//}
//
TEST(PostfixTest, can_calculate_num_infix_expression)
{
    Postfix p;
    p.inputInfix("3 * 2");
    p.parseToPostfix();
    p.Calculate();
    double res;
    res = p.getNumberResult();
    cout << "RESULT = " << res << endl;;
    ASSERT_EQ(res, 6);
}

TEST(PostfixTest, can_calculate_num_and_parentness_infix_expression)
{
    Postfix p;
    p.inputInfix("2 * ( 4 - 1 )");
    p.parseToPostfix();
    p.Calculate();
    double res;
    res = p.getNumberResult();
    cout << "RESULT = " << res << endl;;
    ASSERT_EQ(res, 6);
}

TEST(PostfixTest, can_calculate_polynom_expression)
{
    Postfix p;
    p.addPolynom("p1", "1 + x2 + y3z2");
    p.addPolynom("p2", "x2 - 4y3z2 + y6");
    p.inputInfix("p1 + p2");



    p.parseToPostfix();
    p.Calculate();
    Polynom res;
    res = p.getPolynomResult();
    cout << "\nRESULT = " << res << endl;;
    ASSERT_EQ(res, parsePoly("1 + 2x2 - 3y3z2 + y6"));
}

TEST(PostfixTest, can_calculate_simple_polynom_and_num_expression)
{
    Postfix p;
    p.addPolynom("p1", "1 - x2 + 3y3z2");
    //p.addPolynom("p2", "x2 - 4y3z2 + y6");
    p.inputInfix("2 * p1");



    p.parseToPostfix();
    p.Calculate();
    Polynom res;
    res = p.getPolynomResult();
    cout << "\nRESULT = " << res << endl;;
    ASSERT_EQ(parsePoly("2 - 2x2 + 6y3z2"), res);
}

TEST(PostfixTest, can_calculate_complex_polynom_and_num_expression)
{
    Postfix p;
    p.addPolynom("p1", "1 + x2 + y3z2");
    p.addPolynom("p2", "2x2 - 4y3z2 + 6y6");
    p.inputInfix("1 + 2 * p1 + p2 / 2");



    p.parseToPostfix();
    p.Calculate();
    Polynom res;
    res = p.getPolynomResult();
    cout << "\nRESULT = " << res << endl;;
    ASSERT_EQ(parsePoly("3y6 + 3x2 + 3"), res);
}

TEST(PostfixTest, can_calculate_num_and_named_const_infix_expression)
{
    Postfix p;
    p.inputInfix("3 + ( pi * 2 )");
    p.addConstant("pi", 3.14);
    p.parseToPostfix();
    p.Calculate();
    double res;
    res = p.getNumberResult();
    cout << "RESULT = " << res << endl;;
    ASSERT_EQ(res, 3.0 + 3.14*2.0);
}
//
//TEST(PostfixTest, returns_correct_postfix_expression)
//{
//    ADD_FAILURE() << "Test not implemented";
//}
//
//TEST(PostfixTest, calculate_returns_correct_result)
//{
//    ADD_FAILURE() << "Test not implemented";
//}
//
//TEST(PostfixTest, throws_exception_for_undefined_variables)
//{
//    ADD_FAILURE() << "Test not implemented";
//}
