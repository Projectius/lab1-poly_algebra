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

void Polynom::insert(const Monom& mon) {
    // Если коэффициент монома нулевой, игнорируем вставку
    if (mon.c == 0.0) {
        return;
    }

    mNode** current = &head; // Указатель для изменения связей
    bool exists = false;

    // Ищем позицию для вставки или объединения
    while (*current != nullptr) {
        if ((*current)->m.degs == mon.degs) {
            // Объединяем коэффициенты
            (*current)->m.c += mon.c;

            // Удаляем узел, если коэффициент стал нулевым (но не последний)
            if ((*current)->m.c == 0.0 && (*current)->next != nullptr) {
                mNode* toDelete = *current;
                *current = toDelete->next;
                delete toDelete;
            }
            exists = true;
            break;
        }
        else if ((*current)->m.degs < mon.degs) {
            // Вставляем перед текущим узлом (степени упорядочены по убыванию)
            break;
        }
        current = &((*current)->next);
    }

    // Вставляем новый узел, если не было совпадения
    if (!exists) {
        mNode* newNode = new mNode(mon);
        newNode->next = *current;
        *current = newNode;
    }

    // Гарантируем, что полином не пуст (минимум нулевой моном)
    if (head == nullptr) {
        head = new mNode(Monom(0.0, 0, 0, 0));
    }
}

Polynom::Polynom() { head = new mNode(Monom(0, 0, 0, 0)); back = head; }
Polynom::Polynom(double n) { head = new mNode(Monom(n, 0, 0, 0)); back = head; }

void Polynom::copyFrom(const Polynom& other) {
    head = new mNode(other.head->m);
    
    mNode* currentOther = other.head->next; // Начинаем с первого узла после head
    mNode* currentThis = head;

    while (currentOther) {
        back = currentThis->next = new mNode(currentOther->m);
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
    back = other.back;
    other.head = nullptr; // Обнуляем указатель other
    other.back = nullptr;
    return *this;
}


Monom operator*(const Monom& mon, double k) {
    Monom a;
    a.degs = mon.degs;
    a.c = mon.c * k;
    return a;
}

Monom operator*(const Monom& m1, const Monom& m2) {
    unsigned x = m1.degOf(1) + m2.degOf(1);
    unsigned y = m1.degOf(2) + m2.degOf(2);
    unsigned z = m1.degOf(3) + m2.degOf(3);
    return Monom(m1.c * m2.c, x, y, z);
}

ostream& operator<<(ostream& out, const Monom& m) {
    bool a = 0;
    if(m.coef()!=1)
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
    /*if (result.back->m.degs != 0)
        throw("Polynom BACK invalid\n");*/
    return result;
}

Polynom operator+(const Polynom& p1, const Polynom& p2) {
    return combinePolynoms(p1, p2, [](double a, double b) { return a + b; });
}

Polynom operator+(const Polynom& p, double n)
{
    Polynom result(p);
    result.back->m.c += n;
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
    result.back->m.c -= n;
    return result;
}

Polynom operator-(double n, const Polynom& p)
{
    return (p*(-1)) + n;
}

Polynom operator*(const Polynom& p, double k) {
    if (k == 0)
        return 0;
    Polynom result(p);
    //cout << "res = " << result << endl;
    mNode* h = result.head;
    while (h)
    {
        //cout << h->m << " - > ";
        h->m.c *= k;
        //cout << h->m << endl;
        h = h->next;
        
    }
    //cout << "*res = " << result << endl;
    return result;
}

Polynom operator*(double k, const Polynom& p ) {
    return p * k;
}

Polynom operator*(const Polynom& p1, const Polynom& p2) {
    Polynom result;
    mNode* currentP1 = p1.head;

    // Перемножаем каждый моном первого полинома с каждым мономом второго
    while (currentP1 != nullptr) {
        mNode* currentP2 = p2.head;
        while (currentP2 != nullptr) {
            Monom product = currentP1->m * currentP2->m;
            result.insert(product); // insert объединит одинаковые степени
            currentP2 = currentP2->next;
        }
        currentP1 = currentP1->next;
    }

    return result;
}

ostream& operator<<(ostream& os, const Polynom& n) {
    mNode* p = n.head;
    if (!p) {
        os << "INVALID_POLYNOM, HEAD = nullptr!\n";
        return os;
    }
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

bool operator!=(const Polynom& p1, const Polynom& p2) {
    return !(p1 == p2);
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
            else throw runtime_error("Unexpected variable in input: " + ch);

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
            throw runtime_error("Unexpected character in input: " + char(stream.peek()));
        }
    }

    return result;
}

double CalcInPoint(Polynom& p, double x, double y, double z)
{
    double result = 0;
    mNode* h = p.head;
    while (h)
    {
        Monom& m = h->m;
        result += m.c * pow(x, m.degOf(1)) * pow(y, m.degOf(2)) * pow(z, m.degOf(3));
        h = h->next;
    }
    return result;
}