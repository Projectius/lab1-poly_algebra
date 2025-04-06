#pragma once
#include"tables.h"
#include"stack.h"
#include<vector>
#include"postfix.h"

class TableManager
{
	vector<Table*> tables;
	int selected;
public:
	TableManager();
	void selectTable(int tindex);
	Polynom* find(const string name);
	void add(const string name);
	void remove(const string name);
};

class CommandManager
{
	TableManager tableman;
	TPostfix<Polynom> postfixman;

	Polynom parsePolynom(const string& pstr);
	void processExpr(const string& estr);
public:
	void processInput(const string& inp);
};