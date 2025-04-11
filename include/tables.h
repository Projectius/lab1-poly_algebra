#pragma once
#include "polynom.h"
#include <string>

using namespace std;

extern const char* const TABLE_NAMES[];
extern const int TABLE_COUNT;

// Абстрактная базовая таблица
class Table {
public:
    Table(Polynom* data = nullptr, size_t size = 0);
    virtual ~Table() = default;

    virtual Polynom* find(const string& name) = 0;
    virtual void add(const string& name, const Polynom* pol) = 0;
    virtual void remove(const string& name) = 0;
};

// Таблица на основе линейного массива
class LinearArrayTable : public Table {
public:
    LinearArrayTable();
    virtual ~LinearArrayTable();
    Polynom* find(const string& name) override;
    void add(const string& name, const Polynom* pol) override;
    void remove(const string& name) override;
};

// Таблица на основе связного списка
class LinearListTable : public Table {
public:
    LinearListTable();
    virtual ~LinearListTable();
    Polynom* find(const string& name) override;
    void add(const string& name, const Polynom* pol) override;
    void remove(const string& name) override;
};

// Упорядоченная таблица на основе массива
class OrderedArrayTable : public Table {
public:
    OrderedArrayTable();
    virtual ~OrderedArrayTable();
    Polynom* find(const string& name) override;
    void add(const string& name, const Polynom* pol) override;
    void remove(const string& name) override;
};

// Таблица на основе бинарного дерева поиска
class TreeTable : public Table {
public:
    TreeTable();
    virtual ~TreeTable();
    Polynom* find(const string& name) override;
    void add(const string& name, const Polynom* pol) override;
    void remove(const string& name) override;
};

// Хеш-таблица с открытой адресацией
class OpenHashTable : public Table {
public:
    OpenHashTable();
    virtual ~OpenHashTable();
    Polynom* find(const string& name) override;
    void add(const string& name, const Polynom* pol) override;
    void remove(const string& name) override;
};

// Хеш-таблица с цепочками
class ListHashTable : public Table {
public:
    ListHashTable();
    virtual ~ListHashTable();
    Polynom* find(const string& name) override;
    void add(const string& name, const Polynom* pol) override;
    void remove(const string& name) override;
};
