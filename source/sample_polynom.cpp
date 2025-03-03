#include<iostream>
#include"polynom.h"
#include <string>
#include <sstream>
#include <cctype>
#include <stdexcept>
#include <iostream>
#include<vector>

using namespace std;

Polynom parsePolynomial(const string& input) {
    Polynom result;
    istringstream stream(input);
    char ch;
    int coef = 0;
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
            cout << "This char:" << char(stream.peek()) <<":"<< endl;
            throw "Unexpected character in input";
        }
    }

    return result;
}


int main() {
    vector<Polynom> v; // Хранилище полиномов
    string input;

    while (true) {
        cout << "Enter polynomial or command (lib, mul, add, sub, delete): ";
        getline(cin, input);

        if (input.empty()) continue;

        try {
            if (input == "lib") {
                // Показать список полиномов
                cout << "Polynomial library:\n";
                for (size_t i = 0; i < v.size(); ++i) {
                    cout << i + 1 << ": " << v[i] << "\n";
                }
            }
            else if (input == "add" || input == "sub") {
                int index1, index2;
                cout << "Enter first polynomial number: ";
                cin >> index1;
                cout << "Enter second polynomial number: ";
                cin >> index2;

                if (index1 < 1 || index1 > v.size() || index2 < 1 || index2 > v.size()) {
                    cerr << "Invalid polynomial number.\n";
                }
                else {
                    Polynom result = (input == "add") ? (v[index1 - 1] + v[index2 - 1])
                        : (v[index1 - 1] - v[index2 - 1]);
                    cout << "Result: " << result << "\n";
                    v.push_back(result);
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            else if (input == "mul") {
                int index, scalar;
                cout << "Enter polynomial number: ";
                cin >> index;
                cout << "Enter scalar value: ";
                cin >> scalar;

                if (index < 1 || index > v.size()) {
                    cerr << "Invalid polynomial number.\n";
                }
                else {
                    Polynom result = v[index - 1] * scalar;
                    cout << "Result: " << result << "\n";
                    v.push_back(result);
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            else if (input == "delete") {
                int index;
                cout << "Enter polynomial number to delete: ";
                cin >> index;

                if (index < 1 || index > v.size()) {
                    cerr << "Invalid polynomial number.\n";
                }
                else {
                    v.erase(v.begin() + index - 1);
                    cout << "Polynomial " << index << " deleted.\n";
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            else {
                // Попытка парсинга как полинома
                try {
                    Polynom p = parsePolynomial(input);
                    v.push_back(p);
                    cout << "Polynomial added to library.\n";
                }
                catch (char* e) {
                    cout<<e<< ". Please try again.\n";
                }
            }
        }
        catch (const exception& e) {
            cerr << "Error: " << e.what() << ". Please try again.\n";
        }
    }

    return 0;
}

