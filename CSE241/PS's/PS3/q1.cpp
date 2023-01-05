#include <iostream>     

using namespace std;

class Point
{
public:
    //set x to first, y to second
    void set(double first, double second);
    //move point via x and y
    void move(double first, double second);
    //rotate point 90 degree clockwise
    void rotate();
    //return x coord.
    double getFirst();
    //return y coord.
    double getSecond();

private:
    double x;
    double y;

};
double Point::getFirst()
{
    return x;
}
double Point::getSecond()
{
    return y;
}
void Point::set(double first, double second)
{
    x = first;
    y = second;
}
void Point::move(double first, double second)
{
    x += first;
    y += second;
}
void Point::rotate()
{
    double temp = x;
    x = -y;
    y = temp;
}


int main()
{
    Point A, B, C;

    A.set(1, 2);
    cout << A.getFirst() << ", " << A.getSecond() << endl;
    A.rotate();
    cout << A.getFirst() << ", " << A.getSecond() << endl;
    A.rotate();
    cout << A.getFirst() << ", " << A.getSecond() << endl;
    A.rotate();
    cout << A.getFirst() << ", " << A.getSecond() << endl;
    A.rotate();
    cout << A.getFirst() << ", " << A.getSecond() << endl;
    A.rotate();
    cout << A.getFirst() << ", " << A.getSecond() << endl;

    B.set(2,3);
    cout << B.getFirst() << ", " << B.getSecond() << endl;
    B.move(1,1);
    cout << B.getFirst() << ", " << B.getSecond() << endl;

    C.set(5,-4);
    cout << C.getFirst() << ", " << C.getSecond() << endl;
    cout << "Move C by -5 horizontally and 4 vertically. " <<endl;
    C.move(-5, 4);
    cout << C.getFirst() << ", " << C.getSecond() << endl;

    return 0;
}
