#pragma once
#include "polynom.h"
#include<string>
#include<vector>
#include <list>
#include "tables.h"


class TableManager
{
	vector<Table*> tables;
	
	
	Table* t;
public:
	list<Polynom> data;
	vector<string> names;

	TableManager();
	bool selectTable(int tindex);
	Polynom* find(const string name);
	void add(const string name, const Polynom& pol);
	void remove(const string name);
	~TableManager();
	void PrintContent();
};