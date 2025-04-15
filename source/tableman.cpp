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
	//cout << "TABLEMAN INIT " << t;

	/*for (int i = 0; i<TABLE_COUNT;i++)
		if (tables[i] == nullptr)
			throw("Tableman cant create table\n");*/

	if (tables.size() != TABLE_COUNT)
		throw exception("Tableman table count != tables.cpp TABLE_COUNT");

}

void TableManager::selectTable(int tindex)
{
	if (tindex < 0 || tindex >= TABLE_COUNT)
		throw runtime_error("Invalid table index");
	selected = tindex;
	t = tables[tindex];

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
	names.erase(std::remove(names.begin(), names.end(), name), names.end());
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
	for (auto& it : names)
		cout << it <<"\t"<< * t->find(it) << endl;
}

int TableManager::getSelected()
{
	return selected;
}

string TableManager::getTablename(int tindex)
{
	return TABLE_NAMES[tindex];
}

void TableManager::printTableTypes()
{
	for (int i = 0;i < TABLE_COUNT;i++)
	{
		cout << i << ") " << TABLE_NAMES[i] << endl;
	}
}

