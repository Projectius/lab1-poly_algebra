#include "polynom.h"


unsigned MAX_P = 1 + 9; // 1 + max degree

// Monom
unsigned Monom::encode(unsigned x, unsigned y, unsigned z) {
    return x + y * MAX_P + z * MAX_P * MAX_P;
}

unsigned Monom::degOf(unsigned var_ind) {
    switch (var_ind) {
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

Monom::Monom(int coef, unsigned x, unsigned y, unsigned z) {
    degs = encode(x, y, z);
    c = coef;
    if ((x >= MAX_P) || (y >= MAX_P) || (z >= MAX_P))
        throw "Monome variable: too large degree";
}

int Monom::coef() { return c; }

mNode::mNode(const Monom m) : m(m) {}

//
// Polynom
//

void Polynom::insert(const Monom mon) {
    if (mon.c == 0)
        return;
    if ((head->m.degs == 0U) && (mon.degs == 0U)) {
        head->m.c = mon.c;
        return;
    }

    mNode* p = nullptr;
    mNode* pnext = head;

    if (pnext && (pnext->m.degs > mon.degs)) {
        while (pnext) {
            p = pnext;
            pnext = p->next;
            if (pnext && (pnext->m.degs <= mon.degs))
                break;
        }
    }
    if (pnext && (pnext->m.degs == mon.degs)) {
        if (mon.degs == 0U) {
            pnext->m.c = mon.c;
            return;
        }
        throw "Inserting existing monomial";
    }

    mNode* n = new mNode(mon);
    if (p)
        p->next = n;
    else {
        if (pnext) {
            if (mon.degs > head->m.degs) {
                p = head;
                head = n;
                n->next = p;
                return;
            }
            else {
                head->next = n;
                return;
            }
        }
        else
            throw "No polynom head!";
    }
    n->next = pnext;
}

Polynom::Polynom() { head = new mNode(Monom(0, 0, 0, 0)); }

void Polynom::copyFrom(const Polynom& other) {
    head = new mNode(other.head->m);
    mNode* currentOther = other.head->next; // Начинаем с первого узла после head
    mNode* currentThis = head;

    while (currentOther) {
        currentThis->next = new mNode(currentOther->m);
        currentThis = currentThis->next;
        currentOther = currentOther->next;
    }
}

Polynom::Polynom(const Polynom& y) {
    /*head = new mNode(Monom(0, 0, 0, 0));
    mNode* p = nullptr;
    mNode* pnext = y.head;
    while (pnext) {
        p = pnext;
        pnext = p->next;
        insert(p->m);
    }*/
    copyFrom(y);
}

Polynom::~Polynom() {
    mNode* current = head;
    while (current) {
        mNode* next = current->next;
        delete current;
        current = next;
    }
}

//
// operators
//

Polynom& Polynom::operator=(const Polynom& other) {
    if (this == &other) return *this;

    // Удаление
    mNode* current = head;
    while (current) {
        mNode* next = current->next;
        delete current;
        current = next;
    }

    copyFrom(other);
    return *this;
}

Polynom& Polynom::operator=(Polynom&& other) noexcept {
    if (this == &other) return *this;

    // Удаление текущих узлов
    mNode* current = head;
    while (current) {
        mNode* next = current->next;
        delete current;
        current = next;
    }

    // Перехват ресурсов
    head = other.head;
    other.head = nullptr; // Обнуляем указатель other

    return *this;
}


Monom operator*(Monom& mon, int k) {
    Monom a;
    a.degs = mon.degs;
    a.c = mon.c * k;
    return a;
}

ostream& operator<<(ostream& out, Monom& m) {
    bool a = 0;
    out << m.c;
    if (m.degOf(1) || a)
        out << "x" << m.degOf(1);
    if (m.degOf(2) || a)
        out << "y" << m.degOf(2);
    if (m.degOf(3) || a)
        out << "z" << m.degOf(3);
    return out;
}

Polynom operator+(Polynom& p1, Polynom& p2) {
    Polynom result;
    mNode* p1Node = p1.head;
    mNode* p2Node = p2.head;

    while (p1Node || p2Node) {
        if (!p1Node) {
            result.insert(p2Node->m);
            p2Node = p2Node->next;
        }
        else if (!p2Node) {
            result.insert(p1Node->m);
            p1Node = p1Node->next;
        }
        else if (p1Node->m.degs > p2Node->m.degs) {
            result.insert(p1Node->m);
            p1Node = p1Node->next;
        }
        else if (p1Node->m.degs < p2Node->m.degs) {
            result.insert(p2Node->m);
            p2Node = p2Node->next;
        }
        else {
            int newCoef = p1Node->m.c + p2Node->m.c;
            if (newCoef != 0) {
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

Polynom operator*(Polynom& p1, int k) {
    Polynom result;
    mNode* p = p1.head;
    while (p && ((p->m).coef())) {
        result.insert((p->m) * k);
        p = p->next;
    }
    return result;
}

Polynom operator-(Polynom& p1, Polynom& p2) {
    Polynom neg = p2 * (-1);
    return p1 + neg;
}

ostream& operator<<(ostream& os, Polynom& n) {
    mNode* p = n.head;
    if ((!p->next) && (!p->m.coef())) {
        os << "0";
        return os;
    }
    bool first = true;
    while (p && (p->m.coef())) {
        if ((!first) && (p->m.coef() > 0)) {
            os << "+ ";
        }
        os << p->m << " ";
        p = p->next;
        first = false;
    }
    return os;
}


