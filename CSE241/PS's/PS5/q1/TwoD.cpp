// File: TwoD.cpp

#include <iostream>
#include "TwoD.h"

namespace TD
{
    int myRows(const TwoD& arg){ return arg.maxRows;}
    int myCols(const TwoD& arg){ return arg.maxCols;}

    TwoD operator+(const TwoD &lhs, const TwoD &rhs)
    {
        using namespace std;

        if (lhs.maxRows != rhs.maxRows || lhs.maxCols != rhs.maxCols)
        {
            cout << "Matrices not same size "
                 << " lhs matrix row, col sizes "
                 << lhs.maxRows << " " << lhs.maxCols << endl
                 << "rhs matrix row, col sizes"
                 << rhs.maxRows << " " << rhs.maxCols << endl;
            exit(1); // Die if matrices are not same
        }

        //sizes are same
        TwoD sum(lhs.maxRows, lhs.maxCols);

        for (int i = 0; i < lhs.maxRows; i++)
        for (int j = 0; j < lhs.maxCols; j++)
            sum.matrix[i][j] = rhs.matrix[i][j] + lhs.matrix[i][j];
        
        return sum;
    }
    double &TwoD::operator()(int r, int c) // returns row r and col c elements
    {
        return matrix[r][c];
    }
    const TwoD &TwoD::operator=(const TwoD &rhs)
    {
        if (matrix == rhs.matrix)
            return rhs; // lhs == rhs, do nothing
        //lhs != rhs , blow away lhs
        for (int i = 0; i < maxRows; i++)
            delete[] matrix[i];
        delete[] matrix;

        maxRows = rhs.maxRows;
        maxCols = rhs.maxCols;

        //reallocate
        matrix = new DoubleArrayPtr[maxRows];
        for (int i = 0; i < maxRows; i++)
            matrix[i] = new double[maxCols];

        //deep copy
        for (int i = 0; i < maxRows; i++)
            for (int j = 0; j < maxRows; j++)
                matrix[i][j] = rhs.matrix[i][j];

        return rhs;
    }
    TwoD::TwoD(const TwoD &rhs) : maxRows(rhs.maxRows), maxCols(rhs.maxCols)
    {
        matrix = new DoubleArrayPtr[maxRows];
        for (int i = 0; i < maxRows; i++)
            matrix[i] = new double[maxCols];

        for (int i = 0; i < maxRows; i++)
            for (int j = 0; j < maxRows; j++)
                matrix[i][j] = rhs.matrix[i][j];
    }
    TwoD::~TwoD()
    {
        for (int i = 0; i < maxRows; i++)
            delete[] matrix[i];
        delete[] matrix;
    }
    TwoD::TwoD() : maxRows(10), maxCols(10)
    {
        matrix = new DoubleArrayPtr[maxRows];
        for (int i = 0; i < maxRows; i++)
            matrix[i] = new double[maxCols];

        for (int i = 0; i < maxRows; i++)
            for (int j = 0; j < maxRows; j++)
                matrix[i][j] = 0;
    }

    TwoD::TwoD(int maxR, int maxC) : maxRows(maxR), maxCols(maxC)
    {
        matrix = new DoubleArrayPtr[maxRows];
        for (int i = 0; i < maxR; i++)
            matrix[i] = new double[maxCols];

        for (int i = 0; i < maxRows; i++)
            for (int j = 0; j < maxRows; j++)
                matrix[i][j] = 0;
    }

}