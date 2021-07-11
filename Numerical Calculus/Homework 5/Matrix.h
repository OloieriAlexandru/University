#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <string>
#include <cstdio>
#include <fstream>
#include <stdexcept>
#include <cmath>
#include <utility>

#include <iostream>

#include <Eigen/Dense>
#include <Eigen/SVD> 
using Eigen::MatrixXd;
using Eigen::VectorXd;

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

class Matrix {
    private:
        int p;
        int n;
        
        vv<vv<double>> a;

        pair<int,int> computePQ(const vv<vv<double>>& a);

        pair<double, pair<double,double>> computeTCS(const vv<vv<double>>& a, pair<int,int> pq);

        vv<vv<double>> generateIn(int n);

        void task1(std::ofstream& out);

        void task2(std::ofstream& out);

        void task3(std::ofstream& out);

    public:
        bool init(const char *fileName);

        bool solve(std::ofstream& out);

        friend std::ostream& operator << (std::ostream& out, const Matrix& m);
};

#endif // MATRIX_H
