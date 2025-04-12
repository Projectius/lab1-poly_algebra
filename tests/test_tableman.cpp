#include "gtest.h"
#include "tableman.h"

TEST(TableManTest, tableman_add_find)
{
    TableManager t;
    Polynom p;
    t.add("key", p);

    Polynom* pp = t.find("key");

    for (int i = 1; i < TABLE_COUNT;i++)
    {
        t.selectTable(i);
        ASSERT_EQ(pp, t.find("key"));
    }
}

TEST(TableManTest, tableman_remove)
{
    TableManager t;
    Polynom p;
    t.add("key", p);

    ASSERT_NE(nullptr, t.find("key"));

    t.remove("key");

    for (int i = 0; i < TABLE_COUNT;i++)
    {
        t.selectTable(i);
        ASSERT_EQ(t.find("key"), nullptr);
    }
}

