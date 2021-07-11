#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <cstdio>
#include <string>
#include <ostream>
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <iostream>

#include <Eigen/Dense>
using Eigen::MatrixXd;
using Eigen::VectorXd;

#define vv std::vector

extern double EPS;

// https://stackoverflow.com/questions/12975341/to-string-is-not-a-member-of-std-says-g-mingw
#include <sstream>

namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

class MatrixBonus;

class Matrix {
    private:
        int n = 0;

        int cholesky = 0;

        vv<vv<double>> mat;
        
        vv<double> b;

        vv<double> d;

        void initFrom(const Matrix& m);

        friend class MatrixBonus;
    public:
        bool validMatrix();

        Matrix() = default;

        Matrix(const Matrix& m);

        void transpose();

        int size();
        
        double get(int i, int j);

        int bsize();

        double getb(int i);

        bool init(const char* fileName, bool readEquation = false);

        void initRandom(int size, double minRandValue, double maxRandValue);

        bool initConsole();

        bool choleskyFactorization();

        void printCholeskyFactorization(std::ostream& out);

        double computeDeterminant();

        double getNorm(const Matrix& x);

        Matrix solveEquation();

        Matrix getInverse();

        Matrix getB();

        void setB(const vv<double>& sb);

        double getNorm2(const VectorXd& eigenVector);

        double getNorm1(const MatrixXd& eigenInverse);

        Matrix& operator = (const Matrix& m);

        friend Matrix operator * (const Matrix& mat1, const Matrix& mat2);

        friend std::ostream& operator << (std::ostream& out, const Matrix& mat);
};

class MatrixBonus {
    private:
        bool cholesky;

        int n;

        vv<double> b;

        vv<double> A;
        
        vv<double> L;

        void initFrom(const Matrix& m);

        void initFrom(const MatrixBonus& m);
    public:
        MatrixBonus(const Matrix& m);

        MatrixBonus(const MatrixBonus& m);

        bool choleskyFactorization();

        void printCholeskyFactorization(std::ostream& out);

        Matrix solveEquation();

        MatrixBonus& operator = (const MatrixBonus& m);

        friend std::ostream& operator << (std::ostream& out, const MatrixBonus& mb);
};

#endif // MATRIX_H
