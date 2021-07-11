#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <chrono>

#include "ExpressionParser.h"

const double PI = 3.1415926535897932384626; // PI/2 = 1.570796326794897
const double PI_2 = PI / 2.0;
const double PI_4 = PI / 4.0;
const double MIC = 1e-12;
double EPS;
const double INF = 1e12;

#define SPECIAL_CASE \
    if (specialCase()) { \
        return computeSpecialCaseValue(); \
    }

double myAbs(double x) {
    if (x < 0) {
        return -x;
    }
    return x;
}

class Tan {
    private:
        double x;
        double eps;

        bool eq(double val1, double val2) const {
            return myAbs(val1 - val2) < eps;
        }

        double get2MainLogic(double nx) const {
            static double c1 = 0.33333333333333333;
            static double c2 = 0.13333333333333333;
            static double c3 = 0.053968253968254;
            static double c4 = 0.0218694885361552;

            double x_2 = nx * nx;
            double x_3 = x_2 * nx;
            double x_5 = x_3 * x_2;
            double x_7 = x_5 * x_2;
            double x_9 = x_7 * x_2;

            return nx + x_3 * c1 + x_5 * c2 + x_7 * c3 + x_9 * c4;
        }

        void normalize(double& nx, double& mul) const {
            if (nx < 0) {
                mul = -1.0;
                nx = -1.0 * nx;
            }
            if (nx < PI_2) {
                return;
            }
            if (nx < 0) {
                std::cout << nx << '\n';
                exit(1);
            }
            nx = std::fmod(nx, PI);
            if (nx > PI_2) {
                nx = PI - nx;
                mul = -1.0 * mul;
            }
        }

        bool specialCase() const {
            return (eq(myAbs(this->x), PI_2));
        }

        double computeSpecialCaseValue() const  {
            if (eq(this->x, PI_2)) {
                return INF;
            } else {
                return -INF;
            }
        }
    public:
        Tan(const std::string& str, double eps) {
            ExpressionParser parser(str.c_str());
            x = parser.parse();
            this->eps = eps;
        }

        Tan(double x, double eps) {
            this->x = x;
            this->eps = eps;
        }

        double get1() const {
            SPECIAL_CASE;

            double nx = x, mul = 1.0;
            normalize(nx, mul);

            double b = 0.0;
            double f = b;
            if (eq(f, 0.0)) {
                f = MIC;
            }
            double C = f;
            double D = 0.0;
            double delta = 1.0;
            double a = nx;
            b = 1.0;
            do {
                D = b + a * D;
                if (eq(D, 0)) {
                    D = MIC;
                }
                C = b + a / C;
                if (eq(C, 0)) {
                    C = MIC;
                }
                D = 1.0 / D;
                delta = C * D;
                f = delta * f;
                a = -1.0 * nx * nx;
                b += 2.0;
            } while (!eq(delta, 1.0));

            return mul * f;
        }

        double get2(bool secondNormalize = true) const {
            SPECIAL_CASE;

            double nx = x, mul = 1.0, fval = 1.0;
            normalize(nx, mul);
            if (nx < 0) {
                mul = mul * -1.0;
                nx = -nx;
            }

            if (nx >= PI_4 && secondNormalize) {
                fval = 1.0 / get2MainLogic(PI_2 - nx);
            } else {
                fval = get2MainLogic(nx);
            }
            return mul * fval;
        }

        double get3() const {
            SPECIAL_CASE;

            return tan(x);
        }

        friend std::ostream& operator << (std::ostream& out, const Tan& tanV) {
            out << "Lentz value: " << tanV.get1() << '\n';
            out << "Poly. value: " << tanV.get2() << '\n';
            out << "Poly. value: " << tanV.get2(false) << " (no second normalization)\n";
            out << "cmath value: " << tanV.get3() << '\n';
            return out;
        }
};

// https://stackoverflow.com/a/2704552
double fRand(double fMin, double fMax) {
    double f = (double)rand() / (RAND_MAX-1);
    return fMin + f * (fMax - fMin);
}

double timer(double& microseconds, void(*compute)(void*, void*), void* obj) {
    auto start = std::chrono::high_resolution_clock::now();
    double retValue = 0.0;
    compute((void*)&retValue, obj);
    auto end = std::chrono::high_resolution_clock::now();
    microseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    return retValue;
}

void commandRand() {
    int num;
    std::cout << "Number of random numbers generated: ";
    std::cin >> num;
    std::cout << "Generating " << num << " numbers between (-pi/2,pi/2)\n";

    double errorMean[4];
    memset(errorMean, 0, sizeof(errorMean));
    double timeMean[3];
    memset(timeMean, 0, sizeof(timeMean));

    for (int i=1;i<=num;++i) {
        double x = fRand(-PI_2 + MIC, PI_2 - MIC);
        Tan tanV(x, EPS);

        double tmp;
        double cmathValue = timer(tmp, [] (void* context, void* obj) {
            *((double*)context) = ((Tan*)obj)->get3();
        }, (void*)&tanV);
        timeMean[2] += tmp;

        double lentzValue = timer(tmp, [] (void* context, void* obj) {
            *((double*)context) = ((Tan*)obj)->get1();
        }, (void*)&tanV);
        timeMean[0] += tmp, errorMean[0] += myAbs(cmathValue - lentzValue);

        double polyValue = timer(tmp, [] (void* context, void* obj) {
            *((double*)context) = ((Tan*)obj)->get2();
        }, (void*)&tanV);
        timeMean[1] += tmp, errorMean[1] += myAbs(cmathValue - polyValue);

        double polyNoNormalizeValue = timer(tmp, [] (void* context, void* obj) {
            *((double*)context) = ((Tan*)obj)->get2(false);
        }, (void*)&tanV);
        errorMean[2] += myAbs(cmathValue - polyNoNormalizeValue);
    }
    for (int i=0;i<3;++i) {
        errorMean[i] /= num;
    }
    for (int i=0;i<3;++i) {
        timeMean[i] /= num;
    }

    std::cout << "Mean of differences of Lentz and cmath: " << errorMean[0] << '\n';
    std::cout << "Mean of differences of Poli. and cmath: " << errorMean[1] << '\n';
    std::cout << "Mean of differences of Poli. and cmath: " << errorMean[2] << " (no second normalization)\n";
    std::cout << "Mean of time it took to compute tan using Lentz: " << timeMean[0] << '\n';
    std::cout << "Mean of time it took to compute tan using Poli.: " << timeMean[1] << '\n';
    std::cout << "Mean of time it took to compute tan using cmath: " << timeMean[2] << '\n';
}

void commandCalc() {
    std::string x;
    std::cout << "x = ";
    std::getline(std::cin, x);

    Tan tanV(x, EPS);
    std::cout << tanV;
}

void getEpsilon() {
    int p = 10;
    std::cout << "Precision: 10^(-p), p = " << p << '\n';
    // std::cin >> p;
    EPS = 1.0;
    for (int i=1;i<=p;++i) {
        EPS /= 10.0;
    }
    std::cout << "Precision: " << std::setprecision(20) << std::fixed << EPS << '\n';
}

int main() {
    srand(time(NULL));
    srand(rand());
    getEpsilon();

    bool running = true;
    while (running) {
        std::string line;
        std::cout << ">";
        std::getline(std::cin, line);
        if (!line.size()) {
            continue;
        }
        if (line == "stop") {
            running = false;
            continue;
        }
        if (line == "rand") {
            commandRand();
            continue;
        }
        if (line == "calc") {
            commandCalc();
            continue;
        }
        std::cout << "Invalid command!\n";
    }
    return 0;
}
