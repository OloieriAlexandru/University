#include <iostream>
#include <cmath>
#include <string>
#include <cstdlib>
#include <stdexcept>
#include <fstream>
#include <iomanip>
#include <chrono>

#include "SparseMatrix.h"
#include "TridiagonalMatrix.h"

#define EPS 1e-6

extern std::string errorMessage;

bool doubleCmp(const double& v1, const double& v2) {
    return (std::abs(v1 - v2) < EPS);
}

int doubleRead(FILE* fileIn, double& el, int& line, int& column) {
    return fscanf(fileIn, "%lf, %d, %d\n", &el, &line, &column);
}

int doubleReadTri(FILE* fileIn, double& el) {
    return fscanf(fileIn, "%lf\n", &el);
}

#define Matrix SparseMatrix<double, doubleCmp, doubleRead>
#define TriMatrix TridiagonalMatrix<double, doubleCmp, doubleReadTri>

void readMatrix(Matrix& m, const char* fileName, bool checkNumberOfElementsOnLine = false, bool doPrint = false) {
    if (!m.init(fileName, checkNumberOfElementsOnLine)) {
        std::cout << errorMessage << '\n';
        exit(1);
    }
    if (doPrint) {
        std::cout << m << '\n';
    }
}

void readTriMatrix(TriMatrix& tm, const char *fileName, bool doPrint) {
    if (!tm.init(fileName)) {
        std::cout << errorMessage << '\n';
        exit(1);
    }
    if (doPrint) {
        std::cout << tm << '\n';
    }
}

void timer(Matrix& product, double& microseconds, Matrix (*multiply)(const void*, const void*), const void* mat1, const void* mat2) {
    auto start = std::chrono::high_resolution_clock::now();
    product = multiply(mat1, mat2);
    auto end = std::chrono::high_resolution_clock::now();
    microseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

void bonusProductHelper(const Matrix& triMatrixAsSparse, const TriMatrix& triMatrix) {
    const int numberOfProducts = 3;
    Matrix products[3];
    double durations[numberOfProducts] = {};
    const std::vector<std::string> names = {
        "  Sparse    *    Sparse  ",
        "Tridiagonal * Tridiagonal",
        "  Sparse    * Tridiagonal"
    };

    timer(products[0], durations[0], [](const void* mat1Ptr, const void* mat2Ptr) -> Matrix {
        const Matrix mat1 = *((const Matrix*)mat1Ptr);
        const Matrix mat2 = *((const Matrix*)mat2Ptr);
        return mat1 * mat2;
    }, (const void*)&triMatrixAsSparse, (const void*)&triMatrixAsSparse);

    timer(products[1], durations[1], [](const void* mat1Ptr, const void* mat2Ptr) -> Matrix {
        const TriMatrix mat1 = *((const TriMatrix*)mat1Ptr);
        const TriMatrix mat2 = *((const TriMatrix*)mat2Ptr);
        return mat1 * mat2;
    }, (const void*)&triMatrix, (const void*)&triMatrix);

    timer(products[2], durations[2], [](const void* mat1Ptr, const void* mat2Ptr) -> Matrix {
        const Matrix mat1 = *((const Matrix*)mat1Ptr);
        const TriMatrix mat2 = *((const TriMatrix*)mat2Ptr);
        return mat1 * mat2;
    }, (const void*)&triMatrixAsSparse, (const void*)&triMatrix);

    for (int i=0;i<numberOfProducts;++i) {
        std::cout << names[i] << '\n';
        std::cout << "Duration: " << durations[i] << "ms\n";
        std::cout << "Correct result: " << (products[i] == products[0] ? "yes" : "no") << '\n';
        std::cout << products[i] << "\n\n";
    }
}

void bonusProductTest(const TriMatrix& triMatrix) {
    Matrix triMatrixAsSparse(triMatrix);
    bonusProductHelper(triMatrixAsSparse, triMatrix);

    TriMatrix triMatrixCopy = triMatrix;
    triMatrixCopy.moveB(3);
    triMatrixCopy.moveC(50);
    Matrix triMatrixAsSparseCopy(triMatrixCopy);
    bonusProductHelper(triMatrixAsSparseCopy, triMatrixCopy);
}

void logic(const std::vector<std::string>& fileNames, bool doPrint = false) {
    if (fileNames.size() != 4) {
        std::cout << "ERROR! \"fileNames\" array is supposed to have length exactly four!\n";
        return;
    }
    try {       
        Matrix a;
        readMatrix(a, fileNames[0].c_str(), true, doPrint);

        TriMatrix b;
        readTriMatrix(b, fileNames[1].c_str(), doPrint);

        Matrix expectedSum;
        readMatrix(expectedSum, fileNames[2].c_str(), true, doPrint);

        Matrix expectedProd;
        readMatrix(expectedProd, fileNames[3].c_str(), true, doPrint);

        Matrix sum = a + b;
        if (doPrint) {
            std::cout << "sum:\n" << sum << "\n\n";
        }
        std::cout << "a + b == expected sum ? " << (sum == expectedSum ? "yes" : "no") << '\n';

        Matrix prod = a * b;
        if (doPrint) {
            std::cout << "prod:\n" << prod << "\n\n";
        }
        std::cout << "a * b == expected product ? " << (prod == expectedProd ? "yes" : "no") << '\n';
    
        std::cout << "----------------------------------------------\n";
        std::cout << "BONUS\n";
        bonusProductTest(b);
    } catch (const std::exception& ex) {
        std::cout << ex.what() << '\n';
    }
}

int main() {
    bool toFile = true;
    std::cout << std::setprecision(6) << std::fixed;
    // logic({ "./ignored/a_test.txt", "./ignored/b_test.txt", "./ignored/aplusb_test.txt", "./ignored/aorib_test.txt" }, true);
    if (toFile) {
        freopen("ignored/test.txt","w",stdout);
    }
    logic({ "./ignored/a.txt", "./ignored/b.txt", "./ignored/aplusb.txt", "./ignored/aorib.txt" }, false);
    if (!toFile) {
        std::cin.get();
    }
    return 0;
}
