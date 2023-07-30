// File: TwoD.h

#ifndef PROTECTED_TWOD_H
#define PROTECTED_TWOD_H

namespace TD
{   
    typedef double* DoubleArrayPtr;
    class TwoD
    {
    public:
        TwoD(); //sets maxRows and maxCols each to 10
        TwoD(int maxR, int maxC);
        ~TwoD();
        TwoD(const TwoD &);
        const TwoD &operator=(const TwoD &rhs);
        double &operator()(int r, int c); // returns row r, col c element
        double *operator()(int r);        //returns pointer to row r, Ordinary index
                                          //extracts elements from this operator
        friend TwoD operator+(const TwoD &lhs, const TwoD &rhs);
        friend int myRows(const TwoD &);
        friend int myCols(const TwoD &);

    private:
        double **matrix;
        int maxRows;
        int maxCols;
    };
}


#endif