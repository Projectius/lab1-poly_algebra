#pragma once
#include"polynom.h"
#include<string>

using namespace std;

class Table
{
protected:
public:
	Table(Polynom* data, size_t size);
	virtual Polynom* find(const string& name);
	virtual void add(const string& name);
	virtual void remove(const string& name);
};

class LinearArrayTable : public Table
{
public:
	Polynom* find(const string& name) override;
	void add(const string& name) override;
	void remove(const string& name) override;
};

class LinearListTable : public Table
{
public:
	Polynom* find(const string& name) override;
	void add(const string& name) override;
	void remove(const string& name) override;
};

class OrderedArrayTable : public Table
{
public:
	Polynom* find(const string& name) override;
	void add(const string& name) override;
	void remove(const string& name) override;
};

class TreeTable : public Table
{
public:
	Polynom* find(const string& name) override;
	void add(const string& name) override;
	void remove(const string& name) override;
};

class OpenHashTable : public Table
{
public:
	Polynom* find(const string& name) override;
	void add(const string& name) override;
	void remove(const string& name) override;
};

class ListHashTable : public Table
{
public:
	Polynom* find(const string& name) override;
	void add(const string& name) override;
	void remove(const string& name) override;
};

