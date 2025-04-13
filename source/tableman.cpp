#include "tableman.h"

#define ADD_TABLE(cls) tables.push_back(new cls);
TableManager::TableManager()
{
	ADD_TABLE(LinearArrayTable)
	ADD_TABLE(LinearListTable)
	ADD_TABLE(OrderedArrayTable)
	ADD_TABLE(TreeTable)
	ADD_TABLE(OpenHashTable)
	ADD_TABLE(ListHashTable)
	t = tables[0];

	if (tables.size() != TABLE_COUNT)
		throw exception("Tableman table count != tables.cpp TABLE_COUNT");

}

bool TableManager::selectTable(int tindex)
{
	if (tindex < 0 || tindex >= TABLE_COUNT)
		return 1;
	t = tables[tindex];
	return 0;
}

Polynom* TableManager::find(const string name)
{
	return t->find(name);
}

void TableManager::add(const string name, const Polynom& pol)
{
	names.push_back(name);
	data.push_back(pol);
	Polynom* np = &data.back();

	for (auto& tb : tables)
		tb->add(name,np);
}

void TableManager::remove(const string name)
{
	for (auto& tb : tables)
		tb->remove(name);
}

TableManager::~TableManager()
{
	for (auto& tb : tables)
		delete tb;
}

void TableManager::PrintContent()
{ 
	for (auto& it : data)
		cout << "[ " << it << endl;
}

