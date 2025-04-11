#pragma once
#include <vector>
#include "tables.h"

class TableManager
{
	vector<Table*> tables;
	vector<Polynom> data;
	Table* t;
public:
	TableManager();
	bool selectTable(int tindex);
	Polynom* find(const string name);
	void add(const string name, const Polynom pol);
	void remove(const string name);
	~TableManager();
};