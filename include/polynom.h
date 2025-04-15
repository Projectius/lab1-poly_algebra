#pragma once

#include<iostream>
#include <string>
#include <sstream>

using namespace std;

extern unsigned MAX_P;

class Monom {
    unsigned degs = 0;
    double c = 0;
public:
    unsigned encode(unsigned x, unsigned y, unsigned z);
    unsigned const degOf(unsigned var_ind) const;
    Monom(double coef = 0, unsigned x = 0, unsigned y = 0, unsigned z = 0);
    Monom(double coef, Monom& mon);
    double coef() const;

    friend class Polynom;
    friend ostream& operator<<(ostream&, const Monom&);

    template <typename Op>
    friend Polynom combinePolynoms(const Polynom& p1, const Polynom& p2, Op op);

    friend Polynom operator+(const Polynom&, const Polynom&);
    friend Polynom operator+(const Polynom& p, double n);
    friend Polynom operator-(const Polynom& p, double n);
    friend Polynom operator*(const Polynom& p, double k);

    friend Monom operator*(const Monom&, double);

    friend bool operator==(const Monom& m1, const Monom& m2);

    friend double CalcInPoint(Polynom& p, double x, double y, double z);
};


struct mNode {
    Monom m;
    mNode* next = nullptr;
    mNode(const Monom m);
};


// POLYNOM

class Polynom {
    mNode* head = nullptr;
    mNode* back = nullptr;

    void copyFrom(const Polynom& other);
public:
    void insert(const Monom mon);

    ~Polynom();
    Polynom();

    Polynom(double n);
    
    Polynom(const Polynom& y);

    

    Polynom& operator=(const Polynom& other);

    Polynom& operator=(Polynom&& other) noexcept;

    friend ostream& operator<<(ostream&, const Polynom&);

    template <typename Op>
    friend Polynom combinePolynoms(const Polynom& p1, const Polynom& p2, Op op);
    
    friend Polynom operator+(const Polynom&, const Polynom&);
    friend Polynom operator+(const Polynom& p, double n);
    friend Polynom operator-(const Polynom&, const Polynom&);
    friend Polynom operator-(const Polynom& p, double n);
    friend Polynom operator*(const Polynom& p1, double k);
    friend Polynom operator*(double k, const Polynom& p);

    friend bool operator==(const Polynom& p1, const Polynom& p2); 

    friend double CalcInPoint(Polynom& p, double x, double y, double z);
};

// Объявления перегрузок операторов
Monom operator*(Monom& mon, double k);
ostream& operator<<(ostream& out, const Monom& m);
Polynom operator+(const Polynom& p1, const Polynom& p2);
Polynom operator+(const Polynom& p, double n);
Polynom operator+(double n, const Polynom& p);
Polynom operator-(const Polynom& p1, const Polynom& p2);
Polynom operator-(const Polynom& p, double n);
Polynom operator-(double n, const Polynom& p);
Polynom operator*(const Polynom& p1, double k);
Polynom operator*(double k, const Polynom& p);

ostream& operator<<(ostream& os, const Polynom& n);

bool operator==(const Monom& m1, const Monom& m2);
bool operator==(const Polynom& p1, const Polynom& p2);

Polynom parsePoly(const string& input);

double CalcInPoint(Polynom& p, double x, double y, double z);
