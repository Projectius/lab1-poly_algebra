#include "gtest.h"
#include "../include/postfix.h"

//TEST(PostfixTest, can_create_postfix)
//{
//    ASSERT_NO_THROW(Postfix p(););
//}
//
TEST(PostfixTest, can_parse_num_infix_expression)
{
    Postfix p;
    p.inputInfix("1 + 2");
    p.parseToPostfix();
    p.Calculate();
    double res;
    res = p.getNumberResult();
    cout << "RESULT = " << res << endl;;
    ASSERT_EQ(res, 3);
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
