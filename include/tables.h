#pragma once
#include"polynom.h"
#include<string>

using namespace std;

class Table
{
protected:
	Polynom* data;
public:
	Table(Polynom* data_);
	virtual void find(const string& name);
	virtual void add(const string& name);
	virtual void remove(const string& name);
};

class LinearArrayTable : public Table
{
public:
	void find(const string& name) override;
	void add(const string& name) override;
	void remove(const string& name) override;
};

class LinearListTable : public Table
{
public:
	void find(const string& name) override;
	void add(const string& name) override;
	void remove(const string& name) override;
};

class OrderedArrayTable : public Table
{
public:
	void find(const string& name) override;
	void add(const string& name) override;
	void remove(const string& name) override;
};

class TreeTable : public Table
{
public:
	void find(const string& name) override;
	void add(const string& name) override;
	void remove(const string& name) override;
};

class OpenHashTable : public Table
{
public:
	void find(const string& name) override;
	void add(const string& name) override;
	void remove(const string& name) override;
};

class ListHashTable : public Table
{
public:
	void find(const string& name) override;
	void add(const string& name) override;
	void remove(const string& name) override;
};

