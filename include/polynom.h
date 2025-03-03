#pragma once

#include<iostream>

using namespace std;


unsigned MAX_P = 1 + 9; // 1 + max degree
//#define MAX_V 3 //max variables


class Monom
{
	unsigned degs = 0;
	int c = 0;
public:
	unsigned encode(unsigned x, unsigned y, unsigned z)
	{
		return x + y * MAX_P + z * MAX_P * MAX_P;
	}

	unsigned degOf(unsigned var_ind)
	{
		switch (var_ind)
		{
		//case 0:
		//	return c;
		case 1:
			return degs % MAX_P;
		case 2:
			return (degs / MAX_P) % MAX_P;
		case 3:
			return (degs / (MAX_P * MAX_P)) % MAX_P;
		default:
			throw "Invalid monome variable index";
		}
	}

	Monom(int coef = 0, unsigned x = 0, unsigned y = 0, unsigned z = 0)
	{
		degs = encode(x, y, z);
		//cout << degs << endl;
		c = coef;
		if ((x >= MAX_P) || (y >= MAX_P) || (z >= MAX_P))
			throw "Monome variable: too large degree";
	}

	int coef() { return c; }


	friend class Polynom;
	friend ostream& operator<<(ostream&, Monom&);
	friend Polynom operator+(Polynom&, Polynom&);
	friend Monom operator*(Monom&, int );
};

Monom operator*(Monom& mon, int k)
{
	Monom a;
	a.degs = mon.degs;
	a.c = mon.c * k;
	return a;
}

ostream& operator<<(ostream& out, Monom& m)
{
	bool a = 0;
	out << m.c;
	if (m.degOf(1)||a)
		out  << "x" << m.degOf(1);
	if (m.degOf(2)||a)
		out << "y" << m.degOf(2);
	if (m.degOf(3)||a)
		out<< "z" << m.degOf(3);
	return out;
}

struct mNode
{
	Monom m;
	mNode* next = nullptr;
	mNode(const Monom m) : m(m) {}
};

class Polynom
{
	mNode* head = nullptr;
public:

	void insert(const Monom mon)
	{
		if (mon.c == 0)
			return;
		if ((head->m.degs == 0U)&&(mon.degs == 0U))
		{
			head->m.c = mon.c;
			return;
		}

		mNode* p = nullptr;
		mNode* pnext = head;

		if (pnext && (pnext->m.degs > mon.degs))
		{
			while (pnext)
			{
				p = pnext;
				pnext = p->next;

				if (pnext && (pnext->m.degs <= mon.degs))
					break;
			}
		}
		if (pnext && (pnext->m.degs == mon.degs))
		{
			if (mon.degs == 0U)
			{
				pnext->m.c = mon.c;
				return;
			}
			throw "Inserting existing monomial";
		}
		//если сюда бросило исключение то это то которое выше
		mNode* n = new mNode(mon);

		if (p)
			p->next = n;
		else
		{
			if (pnext)
			{
				if (mon.degs > head->m.degs)
				{
					p = head;
					head = n;
					n->next = p;
					return;
				}
				else
				{
					head->next = n;
					return;
				}

			}
			else
				throw "No polynom head!";
		}
		//	head = n;
		n->next = pnext;

	}

	Polynom() { head = new mNode(Monom(0, 0, 0, 0)); }

	Polynom(const Polynom& y)
	{
		head = new mNode(Monom(0, 0, 0, 0));
		mNode* p = nullptr;
		mNode* pnext = y.head;
		while (pnext)
		{
			p = pnext;
			pnext = p->next;

			insert(p->m);
		}

	}

	//Polynom operator=(Polynom& y)
	//{
	//	head = new mNode(Monom(0, 0, 0, 0));
	//	mNode* p = nullptr;
	//	mNode* pnext = y.head;
	//	while (pnext)
	//	{
	//		p = pnext;
	//		pnext = p->next;

	//		insert(p->m);
	//	}

	//}


	friend ostream& operator<<(ostream&, Polynom&);
	friend Polynom operator+(Polynom&, Polynom&);
	friend Polynom operator*(Polynom& p1, int k);
	friend Polynom operator-(Polynom&, Polynom&);
};

Polynom operator+(Polynom& p1, Polynom& p2)
{
	Polynom result;
	mNode* p1Node = p1.head;
	mNode* p2Node = p2.head;

	while (p1Node || p2Node)
	{
		if (!p1Node) // Если p1 закончился, добавляем оставшиеся из p2
		{
			result.insert(p2Node->m);
			p2Node = p2Node->next;
		}
		else if (!p2Node) // Если p2 закончился, добавляем оставшиеся из p1
		{
			result.insert(p1Node->m);
			p1Node = p1Node->next;
		}
		else if (p1Node->m.degs > p2Node->m.degs) // Степень текущего монома p1 больше
		{
			result.insert(p1Node->m);
			p1Node = p1Node->next;
		}
		else if (p1Node->m.degs < p2Node->m.degs) // Степень текущего монома p2 больше
		{
			result.insert(p2Node->m);
			p2Node = p2Node->next;
		}
		else // Степени равны, нужно сложить коэффициенты
		{
			int newCoef = p1Node->m.c + p2Node->m.c;
			if (newCoef != 0) // Добавляем только ненулевые мономы
			{
				result.insert(Monom(newCoef,
					p1Node->m.degOf(1),
					p1Node->m.degOf(2),
					p1Node->m.degOf(3)));
			}
			p1Node = p1Node->next;
			p2Node = p2Node->next;
		}
	}

	return result;
}

Polynom operator*(Polynom& p1, int k)
{
	Polynom result;
	mNode* p = p1.head;
	while (p&&((p->m).coef()))
	{

		result.insert((p->m) * k);
		p = p->next;
	}

	return result;
}

Polynom operator-(Polynom& p1, Polynom& p2) { return p1 + (p2*(-1)); }


ostream& operator<<(ostream& os, Polynom& n)
{
	mNode* p = n.head;
	if ((!p->next)&&(!p->m.coef()))
	{
		os << "0";
		return os;
	}
	bool first = true;
	while (p&& (p->m.coef()))
	{
		
		if ((!first) && (p->m.coef() > 0))
		{
			
			os << "+ ";
		}
		os << p->m<<" ";
		p = p->next;
		first = false;
	}
	return os;
}