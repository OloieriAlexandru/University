#include <iostream>
#include <fstream>
#include <iomanip>

#include <graphics.h>

#include "ExpressionParser.h"
#include "InterpolationSolver.h"

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

#define CHECK(funcCall, ret) \
	if (!(funcCall)) { \
		std::cout << errorMessage << '\n'; \
	}

double ssin(double x) {
	return std::sin(x) * PI / 180.0;
}

double ccos(double x) {
	return std::cos(x) * PI / 180.0;
}

const int C_WIDTH = 800;
const int C_HEIGHT = 600;
const double PI = 3.14159265359;
std::string errorMessage;

double logPow(double x, int p) {
	if (p <= 0) {
		return 1.0;
	}
	if (p & 1 ) {
		return logPow(x * x, p >> 1) * x;
	}
	return logPow(x * x, p >> 1);
}

double f1(double x) {
    return logPow(x, 2) - 12 * x + 30;
}

double f1d(double x) {
    return x * 2 - 12;
}

double f2(double x) {
	return ssin(x) - ccos(x);
}

double f2d(double x) {
    return ssin(x) + ccos(x);
}

double f3(double x) {
    return logPow(x, 3) * 2 - 3 * x + 15.0;
}

double f3d(double x) {
    return 6 * logPow(x, 2) - 3;
}

double(*f[])(double) = { f1, f2, f3 };
double(*fd[])(double) = { f1d, f2d, f3d };

const int functionsCount = 3;

const char *files[] = {
	"inputs/input1.txt",
	"inputs/input2.txt",
	"inputs/input3.txt",
	"inputs/input4.txt",
	NULL
};

bool parseInterpolationLimits(InputFileParams& params, const std::string& limitsLine) {
	int spacePos = -1;
	for (int i=0;i<limitsLine.size();++i) {
		if (limitsLine[i] == ' ') {
			if (spacePos == -1) {
				spacePos = i;
			} else {
				return false;
			}
		}
	}
	if (spacePos == -1) {
		return false;
	}
	std::string x0 = limitsLine.substr(0, spacePos);
	std::string xN = limitsLine.substr(spacePos + 1, limitsLine.size() - spacePos);
	params.x0 = ExpressionParser(x0.c_str()).parse();
	params.xN = ExpressionParser(xN.c_str()).parse();
	return true;
}

bool readFile(InputFileParams& params, const char *fileName) {
	std::ifstream fileIn(fileName);
	if (!fileIn.is_open()) {
		errorMessage = "Failed to open \"" + std::string(fileName) + "\"";
		return false;
	}
	if (!(fileIn >> params.interpolationType)) {
		errorMessage = "Expected the interpolation type on the first line of file \"" + std::string(fileName) + "\"";
		return false;
	}
	std::string limitsLine;
	std::getline(fileIn, limitsLine);
	if (!std::getline(fileIn, limitsLine)) {
		errorMessage = "Expected x0 and xN values on the second line of file \"" + std::string(fileName) + "\"";
		return false;
	}
	if (!(fileIn >> params.functionIndex)) {
		errorMessage = "Expected the function index on the third line of file \"" + std::string(fileName) + "\"";
		return false;
	}
	if (!(params.functionIndex >= 0 && params.functionIndex < functionsCount)) {
		errorMessage = "The function index on the third line of file \"" + std::string(fileName) +
				"\" should be greater or equal to zero and smaller than " + patch::to_string(functionsCount);
		return false;
	}
	params.f = f[params.functionIndex];
	params.fd = fd[params.functionIndex];
	if (!(fileIn >> params.interpolationPoints)) {
		errorMessage = "Expected the number of interpolation points on the forth line of file \"" + std::string(fileName) + "\"";
		return false;
	}
	if (!(params.interpolationPoints >= 2)) {
		errorMessage = "Invalid number of interpolation points on the forth line of file \"" + std::string(fileName) + "\"";
		return false;
	}

	if (!parseInterpolationLimits(params, limitsLine)) {
		errorMessage = "Invalid limits line! It should contain two expression with a space between them!";
		return false;
	}

	return true;
}

int main() {
    initwindow(C_WIDTH, C_HEIGHT);

	int fileIndex = -1;
	int numberOfDigits = 10;
	while (files[++fileIndex]) {
		InputFileParams params;
		CHECK(readFile(params, files[fileIndex]), 1);
		double xi = (params.x0 + params.xN) / 2.0;
		printf("-------------------------------------\n");
		double res = InterpolationSolver(params).solve(xi);
		printf("File %d\n", fileIndex);
		printf("Interpolation point: x = %.*f\n", numberOfDigits, xi);
		printf("Computed value: %.*f\n", numberOfDigits, res);
		printf("Expected value: %.*f\n", numberOfDigits, params.f(xi));
	}

    closegraph();
	return 0;
}
