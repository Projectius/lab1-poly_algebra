#include "gtest.h"
#include "postfix.h"

TEST(PostfixTest, can_create_postfix)
{
    ASSERT_NO_THROW(Postfix p(););
}