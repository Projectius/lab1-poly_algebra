#include "gtest.h"
#include "polynom.h"

//////////////////////

//Здесь тоже куча тестов не соответствует функционалу. Например полиномов из строк не делаем, это задача парсера


TEST(PolynomTest, can_create_polynom)
{
    ASSERT_NO_THROW(Polynom p;);
}

TEST(PolynomTest, can_create_polynom_from_correct_string)
{
    Monom m(42, 1, 2, 3);
    Monom n(55, 0, 0, 0);
    Monom b(-33, 4, 0, 3);
    Polynom p;
    //cout << p<<endl;
    p.insert(m);
    p.insert(n);
    p.insert(b);

    Polynom p2 = parsePoly("55 + 42xy2z3 - 33x4z3");

    ASSERT_EQ(p, p2);
}

TEST(PolynomTest, can_fill_polynom)
{
    try {
        Monom m(42, 1, 2, 3);
        Monom n(55, 0, 0, 0);
        Monom b(-33, 4, 0, 3);
        Polynom p;
        //cout << p<<endl;
        p.insert(m);
        p.insert(n);
        p.insert(b);
        //cout << p;
    }
    catch (char* e) { //cout << e; 
    }
    ASSERT_NO_THROW();
}

TEST(PolynomTest, throw_insert_exsisting)
{
    Monom m(42, 1, 2, 3);
    Monom n(55, 0, 3, 1);
    Monom v(2, 1, 2, 3);
    Polynom p;
    p.insert(m);
    p.insert(n);
    ASSERT_ANY_THROW(p.insert(v));

}

TEST(PolynomTest, throws_exception_for_polynom_with_excessive_degrees)
{
    ADD_FAILURE() << "Test not implemented";
}

TEST(PolynomTest, throws_exception_for_polynom_with_invalid_degrees)
{
    ADD_FAILURE() << "Test not implemented";
}

TEST(PolynomTest, can_copy_create_polynom)
{
    Monom m(42, 1, 2, 3);
    Monom n(55, 0, 0, 0);
    Monom b(-33, 4, 0, 3);
    Polynom p;
    p.insert(m);
    p.insert(n);
    p.insert(b);
    Polynom p2(p);
    ostringstream o1, o2;
    o1 << p;
    o2 << p2;
    ASSERT_EQ(o1.str(), o2.str());
}

TEST(PolynomTest, copied_polynom_is_independent)
{
    Monom m(42, 1, 2, 3);
    Monom n(55, 0, 0, 0);
    Monom b(-33, 4, 0, 3);
    Polynom p;
    p.insert(m);
    p.insert(n);
    p.insert(b);
    Polynom p2(p);
    Monom w(-1, 3, 4, 5);
    p2.insert(w);
    ostringstream o1, o2;
    o1 << p;
    o2 << p2;
    ASSERT_NE(o1.str(), o2.str());
}

TEST(PolynomTest, can_assign_polynom_to_another)
{
    Monom m(42, 1, 2, 3);
    Monom n(55, 0, 0, 0);
    Monom b(-33, 4, 0, 3);
    Polynom p;
    p.insert(m);
    p.insert(n);
    p.insert(b);
    Polynom p2;
    Monom w(-1, 3, 4, 5);
    p2.insert(w);
    p2 = p;
    ostringstream o1, o2;
    o1 << p;
    o2 << p2;
    ASSERT_EQ(o1.str(), o2.str());
}

TEST(PolynomTest, assign_polynom_is_independent)
{
    Monom m(42, 1, 2, 3);
    Monom n(55, 0, 0, 0);
    Monom b(-33, 4, 0, 3);
    Polynom p;
    p.insert(m);
    p.insert(n);
    p.insert(b);
    Polynom p2;
    p2 = p;
    Monom w(-1, 3, 4, 5);
    p2.insert(w);
    
    ostringstream o1, o2;
    o1 << p;
    o2 << p2;
    ASSERT_NE(o1.str(), o2.str());
}

TEST(PolynomTest, can_assign_polynom_to_itself)
{
    ADD_FAILURE() << "Test not implemented";
}

TEST(PolynomTest, equal_operator_returns_true_for_equal_polynoms)
{
    ADD_FAILURE() << "Test not implemented";
}

TEST(PolynomTest, equal_operator_returns_true_for_unequal_polynoms)
{
    ADD_FAILURE() << "Test not implemented";
}

TEST(PolynomTest, test_addition_operator)
{
    Monom m(42, 1, 2, 3);
    Monom n(55, 0, 3, 1);
    Monom v(2, 4, 0, 3);
    Polynom p;
    p.insert(m);
    p.insert(n);
    p.insert(v);

    Monom m2(-2, 1, 2, 3);
    Monom n2(1, 1, 0, 0);
    Monom v2(3, 4, 0, 3);
    Polynom p2;
    p2.insert(m2);
    p2.insert(n2);
    p2.insert(v2);
    Polynom p3(p + p2);
    ASSERT_NO_THROW();
    //cout << p3 << endl;

    ostringstream o1;
    o1 << p3;

    ASSERT_EQ("40x1y2z3 + 5x4z3 + 55y3z1 + 1x1 ", o1.str());

}

TEST(PolynomTest, test_subtraction_operator)
{
    Monom m(42, 1, 2, 3);
    Monom n(55, 0, 3, 1);
    Monom v(2, 4, 0, 3);
    Polynom p;
    p.insert(m);
    p.insert(n);
    p.insert(v);

    Monom m2(-2, 1, 2, 3);
    Monom n2(1, 1, 0, 0);
    Monom v2(3, 4, 0, 3);
    Polynom p2;
    p2.insert(m2);
    p2.insert(n2);
    p2.insert(v2);
    Polynom p3(p - p2);
    ASSERT_NO_THROW();
    //cout << p3 << endl;

    ostringstream o1;
    o1 << p3;

    ASSERT_EQ("44x1y2z3 -1x4z3 + 55y3z1 -1x1 ", o1.str());
}

TEST(PolynomTest, test_subtraction_assign_operator)
{
    ADD_FAILURE() << "Test not implemented";
}

TEST(PolynomTest, test_subtraction_assign_operator_with_negatives)
{
    ADD_FAILURE() << "Test not implemented";
}

TEST(PolynomTest, test_multiplication_by_positive_constant)
{
    Monom m(42, 1, 2, 3);
    Monom n(55, 0, 3, 1);
    Monom v(2, 4, 0, 3);
    Polynom p;
    p.insert(m);
    p.insert(n);
    p.insert(v);

    Polynom p3(p * 2);
    ASSERT_NO_THROW();
    //cout << p3 << endl;

    ostringstream o1;
    o1 << p3;

    ASSERT_EQ("84x1y2z3 + 4x4z3 + 110y3z1 ", o1.str());
}

TEST(PolynomTest, test_multiplication_by_negative_constant)
{
    ADD_FAILURE() << "Test not implemented";
}

TEST(PolynomTest, test_multiplication_zero)
{
    ADD_FAILURE() << "Test not implemented";
}
