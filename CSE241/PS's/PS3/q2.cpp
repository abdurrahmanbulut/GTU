#include <iostream> 

using namespace std;

class Fraction
{
public:
    double getDouble();
    void outputReducedFraction();
    void setNumerator(int n);
    void setDenominator(int d);

private:
    int numerator;
    int denominator;
    int gcd();    
};
//=======================
//Fraction::getDouble
//Returns the fractiion's value as a double
//=======================
double Fraction::getDouble()
{
    return static_cast<double>(numerator) / denominator;
}
//=======================
//Fraction::outputReducedFraction
//Reduces the fraction and prints it
//=======================
void Fraction::outputReducedFraction()
{
    int g;
    
    g = gcd();

    cout << numerator / g <<" / " << denominator / g <<endl;
    return;
}
//=======================
//Fraction::setNumerator
//Set numerator to n
//=======================
void Fraction::setNumerator(int n)
{
    numerator = n;
}
//=======================
//Fraction::setDenominator
//Set denominator to d
//=======================
void Fraction::setDenominator(int d)
{
    denominator = d;
}
//=======================
//Fraction::gcd
//Finds greatest common divisor of numerator and dominator  by brute force
//=======================
int Fraction::gcd()
{
    int g;

    if(numerator > denominator)
        g = denominator;
    else
        g = numerator;
    //work down to 1, testing to see if both numerator and denominator
    //can be divided by g. if so, return it.
    while(g > 1)
    {
        if(((numerator % g) == 0) && ((denominator % g) == 0))
            return g;
        g--;
    }
    return 1;
}
//=======================
//  main function
//=======================
int main()
{
    //some test fractions
    Fraction f1, f2;

    f1.setNumerator(4);
    f1.setDenominator(2);
    cout << f1.getDouble() << endl;
    f1.outputReducedFraction();
    cout <<endl;

    f2.setNumerator(20);
    f2.setDenominator(60);
    cout << f2.getDouble() << endl;
    f2.outputReducedFraction();
    cout <<endl;

    return 0;
}