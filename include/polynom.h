#pragma once

#include<iostream>

using namespace std;

extern unsigned MAX_P;

class Monom {
    unsigned degs = 0;
    int c = 0;
public:
    unsigned encode(unsigned x, unsigned y, unsigned z);
    unsigned degOf(unsigned var_ind);
    Monom(int coef = 0, unsigned x = 0, unsigned y = 0, unsigned z = 0);
    int coef();

    friend class Polynom;
    friend ostream& operator<<(ostream&, Monom&);
    friend Polynom operator+(Polynom&, Polynom&);
    friend Monom operator*(Monom&, int);
};

struct mNode {
    Monom m;
    mNode* next = nullptr;
    mNode(const Monom m);
};

class Polynom {
    mNode* head = nullptr;

    void copyFrom(const Polynom& other);
public:
    void insert(const Monom mon);

    ~Polynom();
    Polynom();
    
    Polynom(const Polynom& y);

    

    Polynom& operator=(const Polynom& other);

    Polynom& operator=(Polynom&& other) noexcept;

    friend ostream& operator<<(ostream&, Polynom&);
    friend Polynom operator+(Polynom&, Polynom&);
    friend Polynom operator*(Polynom& p1, int k);
    friend Polynom operator-(Polynom&, Polynom&);
};

// Объявления перегрузок операторов
Monom operator*(Monom& mon, int k);
ostream& operator<<(ostream& out, Monom& m);
Polynom operator+(Polynom& p1, Polynom& p2);
Polynom operator*(Polynom& p1, int k);
Polynom operator-(Polynom& p1, Polynom& p2);
ostream& operator<<(ostream& os, Polynom& n);