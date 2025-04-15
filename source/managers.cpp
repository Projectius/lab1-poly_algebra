#include "managers.h"

int main()
{
	CommandManager man;
	man.LoopInputProcessing();
}

CommandManager::CommandManager():postfix(&tableman), state(Cmds::Menu)
{
}

void CommandManager::LoopInputProcessing()
{
	string name;
	shared_ptr<Operand> result;
	while (true)
	{
		getline(cin, inp);

		if (inp == "menu")
		{
			state = Cmds::Menu;
			cout << "MAIN COMMAND MENU\n";
		}
		else 
		{
			switch (state)
			{
			case Cmds::Menu:
				
				if (inp == "polynoms")
				{
					cout << tableman.names.size() << " POLYNOMS\t" << "SELECTED TABLE " << tableman.getSelected() << endl;
					cout << "NAME\t POLYNOM\n";
					tableman.PrintContent();
				}
				else if (inp == "tables")
				{
					tableman.printTableTypes();
				}
				else if (inp == "constants")
				{
					cout << "CONSTANT NAME \t VALUE\n";
					postfix.base.printConstants();
				}
				else if (inp == "constin")
				{
					state = Cmds::ConstNameIn;
					cout << "Input constant name: ";
				}
				else if (inp == "polyin")
				{
					state = Cmds::PolyNameIn;
					cout << "Input polynom name: ";
				}
				else if (inp == "polycalc")
				{
					state = Cmds::PolyCalc;
					cout << "Input name and point coordinates\nname\t x\ty\tz\n";
				}
				else if (inp == "del")
				{
					state = Cmds::DelName;
					cout << "Input polynom or constant name to delete: ";
				}
				else if (inp == "expr")
				{
					state = Cmds::ExprIn;
					cout << "Enter expression:\n";
				}
				else if (inp == "saveres")
				{
					state = Cmds::ResSave;
					cout << "Input name to save last expression calculation result: ";
				}
				else if (inp == "selecttable")
				{
					state = Cmds::SelectTable;
					cout << "Select table by index (try tables command)\n";
				}
				else
				{
					cout << "Cant recognise command.\n";
					cout << "Avaible commands: manu, polynoms, tables, selecttable, constants, constin, polyin, polycalc, del, expr, saveres" << endl;
				}
				break;
			case Cmds::ConstNameIn:
				if (postfix.base.checkIsInBase(inp)|| tableman.find(inp))
				{
					cout << "This name is already taken by other lexeme. Try again.\n";
					break;
				}
				name = inp;
				cout << "Input constant value: ";
				state = Cmds::ConstValIn;
				break;
			case Cmds::ConstValIn:
				double val;
				try
				{
					val = stod(inp);
				}
				catch (runtime_error e) {
					cout << "Cant convert to number value. Try again.\n";
				}
				try
				{
					postfix.addConstant(name, val);
					cout << "Added: " << name << " = " << val << endl;
				}
				catch (runtime_error e)
				{
					cout << e.what() << endl;
				}
				state = Cmds::Menu;
				break;
			case Cmds::PolyNameIn:
				if (tableman.find(inp)|| postfix.base.checkIsInBase(inp))
				{
					cout << "Polynom name already taken. Try again.\n";
					break;
				}
				name = inp;
				state = Cmds::PolyValIn;
				cout << "Input polynom: ";
				break;
			case Cmds::PolyValIn:
				

				try {
					Polynom p = parsePoly(inp);
					tableman.add(name, p);
					cout << "Added: " << name << " = " << p << endl;
				}
				catch (runtime_error e)
				{
					cout << "Cant parse Polynom: " << e.what() << endl;
					break;
				}

				
				state = Cmds::Menu;

				break;
			case Cmds::PolyCalc:
			{
				double x, y, z;
				istringstream iss(inp);
				if (!(iss >> name >> x >> y >> z))
				{
					cout << "Cant convert to numbers. Try again.\n";
					break;
				}
				Polynom* pp = tableman.find(name);
				if (!pp)
				{
					cout << "Cant find polynom: " << name << endl;
					break;
				}
				cout << *pp << " in " << " x = " << x << " y = " << y << " z = " << z << "\n" << " = " << CalcInPoint(*pp, x, y, z) << endl;
			}
				state = Cmds::Menu;
				break;
			case Cmds::DelName:
				if (postfix.base.checkIsInBase(inp))
				{
					postfix.base.deleteLex(inp);
					
				}
				else if (tableman.find(inp))
				{
					postfix.base.deleteLex(inp);
					tableman.remove(inp);
				}
				else
				{
					cout << "cant find" << name << endl;
					break;
				}
				cout << "Deleted " << inp<<endl;
				state = Cmds::Menu;
				break;
			case Cmds::ExprIn:
			{
				//size_t pos = inp.find('=');
				string expinp = inp;
				istringstream iss(inp);
				bool named = false;
				if (std::getline(iss, name, '=') && std::getline(iss, expinp)) {
		
					auto trim = [](std::string& s) {
						size_t start = s.find_first_not_of(" \t\r\n");
						size_t end = s.find_last_not_of(" \t\r\n");
						s = (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
						};

					trim(name);
					trim(expinp);
					inp = expinp;
					named = true;
					cout << "name = " << name << ", expr = " << inp << endl;
				}
				
				postfix.inputInfix(inp);
				try
				{
					postfix.parseToPostfix();

				}
				catch (exception e)
				{
					cout << "Error parsing expression: " << e.what() << endl;
					break;
				}
				try
				{
					result = postfix.Calculate();

				}
				catch (exception e)
				{
					cout << "Error calculating expression: " << e.what() << endl;
					break;
				}

				cout << "\nresult = " << postfix.getPolynomResult() << endl;
				if (named)
				{
					if (tableman.find(name) || postfix.base.checkIsInBase(name))
					{
						cout << "Canst save. Polynom name already taken.\n";

					}
					else
					{
						tableman.add(name, postfix.getPolynomResult());
						cout << "Saved as " << name << " = " << postfix.getPolynomResult();
					}
				}
			}
				state = Cmds::Menu;
				break;
			case Cmds::ResSave:
				name = inp;
				try
				{
					if (tableman.find(name) || postfix.base.checkIsInBase(name))
					{
						cout << "Canst save. Polynom name already taken.\n";

					}
					else
					{
						tableman.add(name, postfix.getPolynomResult());
						cout << "Saved as " << name << " = " << postfix.getPolynomResult();
					}
				}
				catch (exception e) { cout << "Error " << e.what() << endl; }
				state = Cmds::Menu;
				break;
			case Cmds::SelectTable:
				try
				{
					int tsi = stoi(inp);
					tableman.selectTable(tsi);
					cout << "Selected " << tableman.getTablename(tsi) << endl;
				}
				catch (exception e)
				{
					cout << "Error " << e.what() << ". Returning to menu" << endl;
					
				}
				state = Cmds::Menu;
				break;

			}
		}
		

	}
}

