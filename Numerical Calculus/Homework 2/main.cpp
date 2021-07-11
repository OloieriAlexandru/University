#include <iostream>
#include <iomanip>
#include <string>
#include <cstdio>
#include "Matrix.h"

#include <Eigen/Dense>
using Eigen::MatrixXd;
using Eigen::VectorXd;

#define CHECK( funcCall ) \
    if (!(funcCall)) { \
        std::cout << errorMessage << '\n'; \
        return 1; \
    }

double EPS = 1e-6;
int precision = 6;

extern std::string errorMessage;

int getType(const std::string& message, const std::vector<std::string>& options) {
    while (true) {
        std::string line;
        std::cout << message << '\n';
        std::getline(std::cin, line);
        for (int i=0;i<options.size();++i) {
            if (options[i] == line) {
                return i;
            }
        }
    }
}

int main(int argc, char* argv[]) {
    std::cout << std::setprecision(precision) << std::fixed;
    int readType = getType("Input type (0 - console, 1 - file, 2 - random): ", { "console", "file", "random" });
    int writeType = getType("Output type (0 - console, 1 - file): ", { "console", "file" });

    Matrix a;
    const char *inFileName = "ignored/matrix.txt";
    if (readType == 1) {
        CHECK(a.init(inFileName, true));
        printf("Initialized matrix from file: \"%s\"\n", inFileName);
    } else if (readType == 0) {
        CHECK(a.initConsole());
    } else {
        std::cout << "Matrix size: ";
        int maxSize;
        std::cin >> maxSize;
        double minValue;
        std::cout << "Min rand. value: ";
        std::cin >> minValue;
        double maxValue;
        std::cout << "Max rand. value: ";
        std::cin >> maxValue;
        a.initRandom(maxSize, minValue, maxValue);
    }

    if (writeType == 1) {
        const char *outFileName = "ignored/output.txt";
        printf("Output file created: \"%s\"\n", outFileName);
        freopen(outFileName, "w", stdout);
    }

    std::cout << "A\n" << a << '\n';

    CHECK(a.choleskyFactorization());
    a.printCholeskyFactorization(std::cout);

    double det = a.computeDeterminant();
    std::cout << "\ndetA = " << det << '\n';
    
    if (abs(det) < EPS) {
        std::cout << "Cannot compute the solution of the equation! The matrix is singular (detA = 0)";
    }

    Matrix sol = a.solveEquation();
    std::cout << "\nx = \n" << sol << '\n';

    Matrix bmat = a.getB();
    std::cout << "        b = " << bmat;
    Matrix prod = a * sol;
    prod.transpose();
    std::cout << "Ainit * x = " << prod << '\n';

    double norm = a.getNorm(sol);
    std::cout << "norm = " << norm << '\n';
    if (abs(norm) < EPS) {
        std::cout << "The solution is good!\n";
    } else {
        std::cout << "The solution is bad!\n";
    }

    Matrix ainverse = a.getInverse();
    std::cout << "\nA^(-1)\n";
    std::cout << ainverse << '\n';

    std::cout << "Eigen library results:\n";

    MatrixXd eigenA(a.size(), a.size());
    std::cout << "A\n";
    for (int i=0;i<a.size();++i) {
        for (int j=0;j<a.size();++j) {
            eigenA(i, j) = a.get(i, j);
        }
    }
    std::cout << eigenA << '\n';

    // https://eigen.tuxfamily.org/dox/classEigen_1_1LLT.html
    MatrixXd eigenL = eigenA.llt().matrixL();
    std::cout << "L(eigen) = \n" << eigenL << '\n';
    std::cout << "L^T(eigen) = \n" << eigenL.transpose() << '\n';

    VectorXd eigenB(a.size());
    for (int i=0;i<a.bsize();++i) {
        eigenB(i) = a.getb(i);
    }
    VectorXd eigenX = eigenA.colPivHouseholderQr().solve(eigenB);

    std::cout << "\nx(eigen) = \n" << eigenX << '\n';
    std::cout << "\n||x - x(eigen)|| = " << sol.getNorm2(eigenX) << '\n';

    MatrixXd eigenAInverse = eigenA.inverse();
    std::cout << "\nA^(-1)(eigen)\n" << eigenAInverse << '\n';
    std::cout << "\n||A^(-1) - A^(-1)(eigen)|| = " << ainverse.getNorm1(eigenAInverse) << '\n';

    std::cout << "\n----------- BONUS -------------\n";

    MatrixBonus mBonus(a);
    CHECK(mBonus.choleskyFactorization());
    std::cout << "mBonus = \n";
    std::cout << mBonus << '\n';
    mBonus.printCholeskyFactorization(std::cout);

    Matrix solBonus = mBonus.solveEquation();
    std::cout << "\nx = \n" << solBonus << '\n';

    std::cin.get();
    return 0;
}
