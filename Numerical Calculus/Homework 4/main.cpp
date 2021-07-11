#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <memory>
#include <algorithm>
#include <chrono>

#include "TridiagonalMatrix.h"

#define vv std::vector

#define CHECK(funcCall, ret) \
    if (!(funcCall)) { \
        std::cout << errorMessage << '\n'; \
        return ret; \
    }

const int numberOfTests = 6;
double EPS = 1e-8;
int precision = 6;

std::string errorMessage;

void compareSolutionWithEigen(const vv<double>& sol1, const vv<double>& sol2) {
    bool equal = true;
    for (int i=0;i<std::min(sol1.size(), sol2.size());++i) {
        if (myAbs(sol1[i] - sol2[i]) > EPS) {
            equal = false;
            break;
        }
    }
    equal &= (sol1.size() == sol2.size());
    std::cout << "Solution is the same as the solution computed with eigen: " << (equal ? "true" : "false") << '\n';
}

void timerMySolve(TridiagonalMatrix *m, vv<double>& sol) {
    auto start = std::chrono::high_resolution_clock::now();
    sol = m->solveEquation();
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Duration using my implementation: " 
        << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "ms\n";
}

void timerEigenSolve(TridiagonalMatrix *m, vv<double>& solEigen) {
    auto start = std::chrono::high_resolution_clock::now();
    solEigen = m->solveEquationEigen();
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Duration using eigen: "
        << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "ms\n";
}

vv<double> solveEquation(TridiagonalMatrix *m, int testNumber, const char* fileSuff) {
    std::string aFileName = std::string("./ignored/a") + patch::to_string(testNumber) + std::string(".txt");
    std::string fFileName = std::string("./ignored/f") + patch::to_string(testNumber) + std::string(".txt");
    CHECK(m->init(aFileName.c_str(), fFileName.c_str()), vv<double>());
    std::cout << "The matrix is diagonally dominant: " << (m->isDiagonallyDominant() ? "true" : "false") << '\n';
    if (testNumber == 6) {
        std::cout << *m << '\n';
    }
    vv<double> sol;
    vv<double> solEigen;
    try {
        timerMySolve(m, sol);
        timerEigenSolve(m, solEigen);
        compareSolutionWithEigen(sol, solEigen);

        std::string fileOutName = std::string("./ignored/a") + patch::to_string(testNumber) + std::string(fileSuff) + std::string(".txt");
        std::ofstream solOut(fileOutName);
        solOut << std::setprecision(precision) << std::fixed;
        for (auto x : sol) {
            solOut << x << '\n';
        }
        solOut.close();
    
        fileOutName = std::string("./ignored/a") + patch::to_string(testNumber) + std::string(fileSuff) + "-eigen" + std::string(".txt");
        std::ofstream solOutEigen(fileOutName);
        solOutEigen << std::setprecision(precision) << std::fixed;
        for (auto x : solEigen) {
            solOutEigen << x << '\n';
        }
        solOutEigen.close();
    } catch (const std::exception& e) {
        std::cout << errorMessage << '\n';
    }
    return sol;
}

void genSolution0(int n, vv<double>& realSol) {
    realSol = {
        0.056974342340,
        0.064051964050,
        0.078876011657,
        0.088276609195,
        0.0084865996890
    };
}

void genSolution1(int n, vv<double>& realSol) {
    realSol.clear();
    for (int i=0;i<n;++i) {
        realSol.push_back(1.0);
    }
}

void genSolution2(int n, vv<double>& realSol) {
    realSol.clear();
    for (int i=0;i<n;++i) {
        realSol.push_back(1.0 / 3.0);
    }
}

void genSolution3(int n, vv<double>& realSol) {
    realSol.clear();
    for (int i=0;i<n;++i) {
        realSol.push_back(2.0 * (i + 1) / 5.0);
    }
}

void genSolution4(int n, vv<double>& realSol) {
    realSol.clear();
    for (int i=0;i<n;++i) {
        realSol.push_back(2000.0 / (i + 1));
    }
}

void genSolution5(int n, vv<double>& realSol) {
    realSol.clear();
    for (int i=0;i<n;++i) {
        realSol.push_back(2.0);
    }
}

void genSolution6(int n, vv<double>& realSol) {
    realSol = {
        1.33669376693766937669376693766937,
        26.131372259,
        1.341463414634146341463414634146,
        -0.51092387288
    };
}

void(*generators[])(int, vv<double>&) = {
    genSolution0,
    genSolution1,
    genSolution2,
    genSolution3,
    genSolution4,
    genSolution5,
    genSolution6
};

void validate(int testIndex, const vv<double>& obtainedSol) {
    vv<double> realSol;
    generators[testIndex](obtainedSol.size(), realSol);
    bool match = true;
    for (int i=0;i<obtainedSol.size();++i) {
        if (std::abs(realSol[i] - obtainedSol[i]) > EPS) {
            match = false;
            std::cout << std::setprecision(precision) << std::fixed << "Test " << testIndex << ", the solutions don't match, the " << i << "'th element is "
                    << obtainedSol[i] << ", expected " << realSol[i] << "!\n";
            break;
        }
    }
    if (match) {
        std::cout << "Test " << testIndex << ", the solutions match!\n";
    }
}

int main() {
    std::cout << std::setprecision(precision) << std::fixed;
    for (int i=0;i<=numberOfTests;++i) {
        std::unique_ptr<TridiagonalMatrix> t;
        t.reset(new TridiagonalMatrix());
        std::cout << "---------------------------------------------------\n";
        auto sol = solveEquation(t.get(), i, "_tri-sol");
        std::cout << "Computed norm: " << t->getNorm() << '\n';
        validate(i, sol);
    }
    std::cin.get();
    return 0;
}
