#pragma once
#include"tables.h"
//#include"TStack.h"
#include<vector>
#include"postfix.h"
#include "tableman.h"

#include<iostream>


enum class Cmds{Menu, ConstNameIn,ConstValIn, PolyNameIn, PolyValIn, PolyCalc,DelName, ExprIn, ResSave, SelectTable};


class CommandManager
{
	TableManager tableman;
	Postfix postfix;

	Polynom parsePolynom(const string& pstr);
	void processExpr(const string& estr);
	Cmds state;
	string inp;
public:
	CommandManager();
	void processInput(const string& inp_);
	void LoopInputProcessing();
};