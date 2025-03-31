#pragma once
#include"polynom.h"
#include<string>

using namespace std;

// Реализовать таблицы и тесты к ним
// find возвращает nullptr если не нашёл

class Table
{
protected:
public:
	Table(Polynom* data, size_t size);
	virtual Polynom* find(const string& name);
	virtual void add(const string& name, const Polynom&& pol);
	virtual void remove(const string& name);
};

class LinearArrayTable : public Table
{
public:
	Polynom* find(const string& name) override;
	virtual void add(const string& name, const Polynom&& pol);
	void remove(const string& name) override;
};

class LinearListTable : public Table
{
public:
	Polynom* find(const string& name) override;
	virtual void add(const string& name, const Polynom&& pol);
	void remove(const string& name) override;
};

class OrderedArrayTable : public Table
{
public:
	Polynom* find(const string& name) override;
	virtual void add(const string& name, const Polynom&& pol);
	void remove(const string& name) override;
};

class TreeTable : public Table
{
public:
	Polynom* find(const string& name) override;
	virtual void add(const string& name, const Polynom&& pol);
	void remove(const string& name) override;
};

class OpenHashTable : public Table
{
public:
	Polynom* find(const string& name) override;
	virtual void add(const string& name, const Polynom&& pol);
	void remove(const string& name) override;
};

class ListHashTable : public Table
{
public:
	Polynom* find(const string& name) override;
	virtual void add(const string& name, const Polynom&& pol);
	void remove(const string& name) override;
};

