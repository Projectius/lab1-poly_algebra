// tables.h Ч обновлЄнный заголовочный файл
#pragma once
#include "polynom.h"
#include <string>

using namespace std;

extern const char* const TABLE_NAMES[];
extern const int TABLE_COUNT;

class Table {
public:
    Table(Polynom* data = nullptr, size_t size = 0);
    virtual ~Table() = default;

    virtual Polynom* find(const string& name) = 0;
    virtual void add(const string& name, const Polynom* pol) = 0;
    virtual void remove(const string& name) = 0;
};

class LinearArrayTable : public Table {
protected:
    struct ArrayEntry { string key; Polynom* value; };
    ArrayEntry* entries;
    size_t capacity;
    size_t count;
    void resize();

public:
    LinearArrayTable();
    ~LinearArrayTable();
    Polynom* find(const string& name) override;
    void add(const string& name, const Polynom* pol) override;
    void remove(const string& name) override;
};

class LinearListTable : public Table {
protected:
    struct ListNode { string key; Polynom* value; ListNode* next; };
    ListNode* head;

public:
    LinearListTable();
    ~LinearListTable();
    Polynom* find(const string& name) override;
    void add(const string& name, const Polynom* pol) override;
    void remove(const string& name) override;
};

class OrderedArrayTable : public Table {
protected:
    struct OrderedEntry { string key; Polynom* value; };
    OrderedEntry* entries;
    size_t capacity;
    size_t count;
    void resize();
    int binarySearch(const string& key) const;

public:
    OrderedArrayTable();
    ~OrderedArrayTable();
    Polynom* find(const string& name) override;
    void add(const string& name, const Polynom* pol) override;
    void remove(const string& name) override;
};

class TreeTable : public Table {
protected:
    struct TreeNode {
        string key;
        Polynom* value;
        TreeNode* left;
        TreeNode* right;
    };
    TreeNode* root;
    TreeNode* addNode(TreeNode* node, const string& key, const Polynom* pol);
    TreeNode* findNode(TreeNode* node, const string& key);
    TreeNode* removeNode(TreeNode* node, const string& key);
    void deleteTree(TreeNode* node);

public:
    TreeTable();
    ~TreeTable();
    Polynom* find(const string& name) override;
    void add(const string& name, const Polynom* pol) override;
    void remove(const string& name) override;
};

class OpenHashTable : public Table {
protected:
    enum SlotState { EMPTY, OCCUPIED, DELETED };
    struct HashSlot {
        string key;
        Polynom* value;
        SlotState state;
    };
    HashSlot* table;
    size_t capacity;
    size_t count;
    size_t hashKey(const string& key) const;
    void resize();

public:
    OpenHashTable();
    ~OpenHashTable();
    Polynom* find(const string& name) override;
    void add(const string& name, const Polynom* pol) override;
    void remove(const string& name) override;
};

class ListHashTable : public Table {
protected:
    struct ChainNode {
        string key;
        Polynom* value;
        ChainNode* next;
    };
    ChainNode** buckets;
    size_t capacity;
    size_t count;
    size_t hashKey(const string& key) const;

public:
    ListHashTable();
    ~ListHashTable();
    Polynom* find(const string& name) override;
    void add(const string& name, const Polynom* pol) override;
    void remove(const string& name) override;
};
