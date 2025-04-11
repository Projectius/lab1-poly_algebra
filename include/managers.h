#pragma once
#include"tables.h"
//#include"TStack.h"
#include<vector>
#include"postfix.h"
#include "tableman.h"


class CommandManager
{
	TableManager tableman;


	Polynom parsePolynom(const string& pstr);
	void processExpr(const string& estr);
public:
	void processInput(const string& inp);
};