#include <iomanip>
#include <vector>
#include <fstream>
#include <memory>
#include <algorithm>
#include <iostream>

#include "Matrix.h"

#define vv std::vector

#define CHECK(funcCall, ret) \
    if (!(funcCall)) { \
        std::cout << errorMessage << '\n'; \
        return ret; \
    }

const int numberOfTests = 4;
double EPS = 1e-8;
int precision = 6;

std::string errorMessage;

bool solveForMatrix(int testNumber, const char* fileSuff) {
    std::string aFileName = std::string("./ignored/matrix") + patch::to_string(testNumber) + std::string(".txt");
    Matrix m;
    CHECK(m.init(aFileName.c_str()), false);
    std::cout << "---------------------------------\n";
    std::cout << "Testul " << testNumber << ":\n";
    std::cout << m;
    try {
        std::string fileOutName = std::string("./ignored/matrix") + patch::to_string(testNumber) + std::string(fileSuff) + std::string(".txt");
        std::ofstream solOut(fileOutName);
        solOut << std::setprecision(6) << std::fixed;
        if (!m.solve(solOut)) {
            std::cout << errorMessage << '\n';
            return false;
        }
        solOut.close();
    } catch (const std::exception& e) {
        std::cout << errorMessage << '\n';
        return false;
    }
    return true;
}

int main() {
    for (int i=1;i<=numberOfTests;++i) {
        if (!solveForMatrix(i, "_sol")) {
            std::cout << "Error - matrix " << i << '\n';
        }
    }
    std::cin.get();
    return 0;
}
