#include "polynom.h"

unsigned MAX_P = 1 + 9; // 1 + max degree

// Monom
unsigned Monom::encode(unsigned x, unsigned y, unsigned z) {
    return x + y * MAX_P + z * MAX_P * MAX_P;
}

unsigned const Monom::degOf(unsigned var_ind) const {
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

Monom::Monom(double coef, unsigned x, unsigned y, unsigned z) {
    degs = encode(x, y, z);
    c = coef;
    if ((x >= MAX_P) || (y >= MAX_P) || (z >= MAX_P))
        throw "Monome variable: too large degree";
}

Monom::Monom(double coef, Monom& mon) {
    degs = mon.degs;
    c = coef;
}

double Monom::coef() const { return c; }

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


Monom operator*(const Monom& mon, double k) {
    Monom a;
    a.degs = mon.degs;
    a.c = mon.c * k;
    return a;
}

ostream& operator<<(ostream& out, const Monom& m) {
    bool a = 0;
    out << m.coef();
    if (m.degOf(1) || a)
        out << "x" << m.degOf(1);
    if (m.degOf(2) || a)
        out << "y" << m.degOf(2);
    if (m.degOf(3) || a)
        out << "z" << m.degOf(3);
    return out;
}

template <typename Op>
Polynom combinePolynoms(const Polynom& p1, const Polynom& p2, Op op) {
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
            double newCoef = op(p1Node->m.c, p2Node->m.c);
            if (newCoef != 0.0) {
                result.insert(Monom(newCoef, p1Node->m));
            }
            p1Node = p1Node->next;
            p2Node = p2Node->next;
        }
    }
    return result;
}

Polynom operator+(const Polynom& p1, const Polynom& p2) {
    return combinePolynoms(p1, p2, [](double a, double b) { return a + b; });
}

Polynom operator+(const Polynom& p, double n)
{
    Polynom result(p);
    result.head->m.c += n;
    return result;
}

Polynom operator+(double n, const Polynom& p)
{
    return p + n;
}

Polynom operator-(const Polynom& p1, const Polynom& p2) {
    return combinePolynoms(p1, p2, [](double a, double b) { return a - b; });
}

Polynom operator-(const Polynom& p, double n)
{
    Polynom result(p);
    result.head->m.c -= n;
    return result;
}

Polynom operator-(double n, const Polynom& p)
{
    return (p*(-1)) + n;
}

Polynom operator*(const Polynom& p, double k) {
    //Polynom result;
    //mNode* p = p1.head;
    //while (p && ((p->m).coef())) {
    //    result.insert((p->m) * k);
    //    p = p->next;
    //}
    //return result;

    Polynom result(p);
    mNode* h = p.head;
    while (h)
    {
        h->m.c *= k;
        h = h->next;
    }
    return result;
}

Polynom operator*(double k, const Polynom& p ) {
    return p * k;
}



ostream& operator<<(ostream& os, const Polynom& n) {
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

bool operator==(const Monom& m1, const Monom& m2) {
    return (m1.c == m2.c) && (m1.degs == m2.degs);
}

bool operator==(const Polynom& p1, const Polynom& p2) {
    mNode* current1 = p1.head;
    mNode* current2 = p2.head;

    // Сравниваем каждый узел
    while (current1 != nullptr && current2 != nullptr) {
        if (!(current1->m == current2->m)) {
            return false;
        }
        current1 = current1->next;
        current2 = current2->next;
    }


    return (current1 == nullptr && current2 == nullptr);
}

Polynom parsePoly(const string& input) {
    Polynom result;
    istringstream stream(input);
    char ch;
    double coef = 0;
    unsigned x = 0, y = 0, z = 0;
    bool negative = false;

    while (!stream.eof()) {
        //cout << "!" << endl;
        //cout << stream.str() << endl;
        coef = 0; x = 0; y = 0; z = 0;
        negative = false;

        // Читаем знак перед мономом
        if (stream.peek() == '+') {
            stream.get();
        }
        else if (stream.peek() == '-') {
            stream.get();
            negative = true;
        }

        stream >> ws;

        // Читаем коэффициент
        while (isdigit(stream.peek())) {
            coef = coef * 10 + (stream.get() - '0');
        }
        if (negative) coef = -coef;

        // Если коэффициент отсутствует, он равен 1 или -1
        if (coef == 0) coef = (negative ? -1 : 1);

        // Читаем степени переменных
        while (isalpha(stream.peek())) {
            //cout << "pvar: " << char(stream.peek()) << endl;
            ch = stream.get();
            unsigned* target = nullptr;

            if (ch == 'x') target = &x;
            else if (ch == 'y') target = &y;
            else if (ch == 'z') target = &z;
            else throw "Unexpected variable in input";

            if (isdigit(stream.peek())) {
                while (isdigit(stream.peek())) {
                    //cout << "pdigit: " << char(stream.peek()) << endl;
                    *target = *target * 10 + (stream.get() - '0');
                }
            }
            else {
                *target = 1; // Если степень не указана то она равна 1
            }
        }


        result.insert(Monom(coef, x, y, z));

        // Пропускаем пробелы после монома
        //cout << stream.str() << endl;
       /* while (stream.peek() == ' ')
        {
            cout << "ws ";
            stream.get();
        }*/
        stream >> ws;

        //cout << stream.str() << endl;

        if (stream.peek() != '+' && stream.peek() != '-' && !stream.eof()) {
            cout << "This char:" << char(stream.peek()) << ":" << endl;
            throw "Unexpected character in input";
        }
    }

    return result;
}

