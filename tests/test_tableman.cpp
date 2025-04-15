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

TEST(TableManTest, tableman_add_find_many)
{
    TableManager t;

    t.add("key1", parsePoly("3 + 2x2"));
    t.add("key2", parsePoly("4 + xy3 - z4"));

    auto it = t.data.begin();
    for (int i = 0; i<t.data.size();i++)
    {
        string name = t.names[i];
        Polynom pp = *it;
        cout << "Checking: " << name << " = " << pp;
        for (int j = 0;j < TABLE_COUNT;j++)
        {
            cout << " in " << TABLE_NAMES[j]<<endl;
            t.selectTable(j);
            ASSERT_EQ(pp, *(t.find(name)));
        }
        advance(it, 1);

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

