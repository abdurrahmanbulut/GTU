// File: TwoDApp.cpp

#include <iostream>
#include <cstdlib>
#include "TwoD.h"

int main()
{
    using std::cin;
    using std::cout;
    using std::endl;
    using std::exit;
    using namespace TD;

    int d1, d2, i, j;
    cout <<"Enter the row and column dimensions of the array\n";
    cin >> d1 >> d2;

    TwoD matrix1(d1, d2);

    cout << "Enter " << d1 << " rows of " << d2 << " doubles each\n";

    for (i = 0; i < d1; i++)
        for (j = 0; j < d2; j++)
            cin >> matrix1(i, j);
    
    cout <<"Echoing the 2 dim. array, matrix1\n";
    for (i = 0; i < d1; i++)
    {
        for (j = 0; j < d2; j++)
            cout << matrix1(i, j) << " ";
        cout << endl;
    }

    cout << "Enter the row and column dimensions of the array\n";
    cin >> d1 >> d2;

    TwoD matrix2(d1, d2), matrix3;

     cout << "Enter " << d1 << " rows of " << d2 << " doubles each\n";
     for (i = 0; i < d1; i++)
     {
         for (j = 0; j < d2; j++)
             cin >> matrix2(i, j);
     }

     cout <<"Echoing the 2 dim. array, matrix2\n";
    for (i = 0; i < myRows(matrix2); i++)
    {
        for (j = 0; j < myCols(matrix2); j++)
            cout << matrix1(i, j) << " ";
        cout << endl;
    }

    cout << "assigning matrix 2 to matrix 3 " << endl;
    matrix3 = matrix2;

    cout << "Displaying the 2 dim array, matrix3 " <<"resulting from assignment.\n";
    cout << "rows " << myRows(matrix3) << "  " << "cols " <<myCols(matrix3) << endl;

    for (i = 0; i < myRows(matrix3); i++)
    {
        for (j = 0; j < myCols(matrix3); j++)
            cout << matrix3(i, j) << " ";
        cout << endl;
    }

    for (i = 0; i < myRows(matrix3); i++)
    {
        for (j = 0; j < myCols(matrix3); j++)
        {
            //cout << "i,j " << i << " " << j << "   "; // debugging
            cout << matrix3(i, j) << " ";
        }
        cout << endl;
    }

    return 0;
}