#ifndef TRIDIAGONAL_MATRIX_H
#define TRIDIAGONAL_MATRIX_H

#include <vector>
#include <string>
#include <cstdio>
#include <ostream>
#include <stdexcept>
#include <cmath>
#include <utility>
#include <iostream>

#include <Eigen/Sparse>

#define vv std::vector
#define pair std::pair
#define mp std::make_pair

// https://stackoverflow.com/questions/12975341/to-string-is-not-a-member-of-std-says-g-mingw
#include <sstream>

static const int maxNumberOfIterations = 100000;
extern std::string errorMessage;
extern double EPS;

extern void err(const std::string& message);

namespace patch {
    template < typename T > std::string to_string( const T& n ) {
        std::ostringstream stm;
        stm << n;
        return stm.str();
    }
}

double myAbs(double x);

class TridiagonalMatrix {
    private:
        int n;
        int p;
        int q;

        vv<double> a;
        vv<double> b;
        vv<double> c;
        vv<double> f;

        vv<double> sol;

        bool readArray(FILE *fileIn, int elCount, vv<double>& arr, std::string arrName);

        vv<pair<int, double>> getElementsOnLine(int line, int& cIndex, int& bIndex) const;

    public:
        int size();

        bool init(const char* aFileName, const char* fFileName);

        bool isDiagonallyDominant();

        vv<double> solveEquation();

        vv<double> solveEquationEigen();

        double getNorm();

        friend std::ostream& operator << (std::ostream& out, const TridiagonalMatrix& tm);
};

#endif // TRIDIAGONAL_MATRIX_H
