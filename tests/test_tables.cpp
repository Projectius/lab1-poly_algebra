#include "gtest.h"
#include "tables.h"

// ------------------- LinearArrayTable -------------------
TEST(TableTest, linear_array_table_add)
{
    LinearArrayTable table;
    Polynom pol1, pol2;
    table.add("key1", &pol1);
    ASSERT_EQ(table.find("key1"), &pol1);
    // Обновление значения
    table.add("key1", &pol2);
    ASSERT_EQ(table.find("key1"), &pol2);
}

TEST(TableTest, linear_array_table_find)
{
    LinearArrayTable table;
    Polynom pol;
    ASSERT_EQ(table.find("nonexistent"), nullptr);
    table.add("key", &pol);
    ASSERT_EQ(table.find("key"), &pol);
}

TEST(TableTest, linear_array_table_remove)
{
    LinearArrayTable table;
    Polynom pol1, pol2;
    table.add("key1", &pol1);
    table.add("key2", &pol2);
    table.remove("key1");
    ASSERT_EQ(table.find("key1"), nullptr);
    ASSERT_EQ(table.find("key2"), &pol2);
}

// ------------------- LinearListTable -------------------
TEST(TableTest, linear_list_table_add)
{
    LinearListTable table;
    Polynom pol1, pol2;
    table.add("key1", &pol1);
    ASSERT_EQ(table.find("key1"), &pol1);
    table.add("key1", &pol2);
    ASSERT_EQ(table.find("key1"), &pol2);
}

TEST(TableTest, linear_list_table_find)
{
    LinearListTable table;
    Polynom pol;
    ASSERT_EQ(table.find("key"), nullptr);
    table.add("key", &pol);
    ASSERT_EQ(table.find("key"), &pol);
}

TEST(TableTest, linear_list_table_remove)
{
    LinearListTable table;
    Polynom pol1, pol2;
    table.add("key1", &pol1);
    table.add("key2", &pol2);
    table.remove("key1");
    ASSERT_EQ(table.find("key1"), nullptr);
    ASSERT_EQ(table.find("key2"), &pol2);
}

// ------------------- OrderedArrayTable -------------------
TEST(TableTest, ordered_array_table_add)
{
    OrderedArrayTable table;
    Polynom pol1, pol2, pol3;
    table.add("key3", &pol3);
    table.add("key1", &pol1);
    table.add("key2", &pol2);
    ASSERT_EQ(table.find("key1"), &pol1);
    ASSERT_EQ(table.find("key2"), &pol2);
    ASSERT_EQ(table.find("key3"), &pol3);
}

TEST(TableTest, ordered_array_table_find)
{
    OrderedArrayTable table;
    Polynom pol;
    ASSERT_EQ(table.find("key"), nullptr);
    table.add("key", &pol);
    ASSERT_EQ(table.find("key"), &pol);
}

TEST(TableTest, ordered_array_table_remove)
{
    OrderedArrayTable table;
    Polynom pol1, pol2;
    table.add("key1", &pol1);
    table.add("key2", &pol2);
    table.remove("key1");
    ASSERT_EQ(table.find("key1"), nullptr);
    ASSERT_EQ(table.find("key2"), &pol2);
}

// ------------------- TreeTable -------------------
TEST(TableTest, tree_table_add)
{
    TreeTable table;
    Polynom pol1, pol2;
    table.add("key1", &pol1);
    table.add("key2", &pol2);
    ASSERT_EQ(table.find("key1"), &pol1);
    ASSERT_EQ(table.find("key2"), &pol2);
}

TEST(TableTest, tree_table_find)
{
    TreeTable table;
    Polynom pol;
    ASSERT_EQ(table.find("key"), nullptr);
    table.add("key", &pol);
    ASSERT_EQ(table.find("key"), &pol);
}

TEST(TableTest, tree_table_remove)
{
    TreeTable table;
    Polynom pol1, pol2;
    table.add("key1", &pol1);
    table.add("key2", &pol2);
    table.remove("key1");
    ASSERT_EQ(table.find("key1"), nullptr);
    ASSERT_EQ(table.find("key2"), &pol2);
}

// ------------------- OpenHashTable -------------------
TEST(TableTest, open_hash_table_add)
{
    OpenHashTable table;
    Polynom pol;
    table.add("key", &pol);
    ASSERT_EQ(table.find("key"), &pol);
}

TEST(TableTest, open_hash_table_find)
{
    OpenHashTable table;
    ASSERT_EQ(table.find("nonexistent"), nullptr);
}

TEST(TableTest, open_hash_table_remove)
{
    OpenHashTable table;
    Polynom pol;
    table.add("key", &pol);
    table.remove("key");
    ASSERT_EQ(table.find("key"), nullptr);
}

// ------------------- ListHashTable -------------------
TEST(TableTest, list_hash_table_add)
{
    ListHashTable table;
    Polynom pol;
    table.add("key", &pol);
    ASSERT_EQ(table.find("key"), &pol);
}

TEST(TableTest, list_hash_table_find)
{
    ListHashTable table;
    ASSERT_EQ(table.find("nonexistent"), nullptr);
}

TEST(TableTest, list_hash_table_remove)
{
    ListHashTable table;
    Polynom pol;
    table.add("key", &pol);
    table.remove("key");
    ASSERT_EQ(table.find("key"), nullptr);
}
