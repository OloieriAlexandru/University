#include "InterpolationSolver.h"

// https://stackoverflow.com/questions/2704521/generate-random-double-numbers-in-c
void InterpolationSolver::generateXY(std::vector<double>& x, std::vector<double>& y) {
    std::uniform_real_distribution<double> unif(params.x0, params.xN);
    std::default_random_engine re;
    for (int i=0;i<params.interpolationPoints-2;++i) {
        x.push_back(unif(re));
    }
    x.push_back(params.x0);
    x.push_back(params.xN);
    sort(x.begin(), x.end());
    for (int i=0;i<x.size();++i) {
        y.push_back(params.f(x[i]));
    }
}

double InterpolationSolver::leastSquaresInterpolationValue(const std::vector<double>& a, double value) {
    double res = a[0];
    for (int i=1;i<a.size();++i) {
        res = a[i] + res * value;
    }
    return res;
}

double InterpolationSolver::leastSquaresInterpolation(const std::vector<double>& x, const std::vector<double>& y, double value) {
    std::cout << "Entering Least Squares Interpolation\n";
    int n = params.interpolationPoints;
    MatrixXd b(n, n);
    for (int i=0;i<n;++i) {
        double v = 1.0;
        for (int j=0;j<n;++j) {
            b(i, j) = v;
            v *= x[i];
        }
    }
    VectorXd vy(n);
    for (int i=0;i<n;++i) {
        vy(i) = y[i];
    }
    VectorXd eigena = b.colPivHouseholderQr().solve(vy);
    std::vector<double> va(n);
    for (int i=0;i<n;++i) {
        va[i] = eigena(i);
    }
    std::reverse(va.begin(), va.end());

    resetPlottingValues();
    for (int i=0;i<numberOfPoints;++i, currentX += deltaX) {
        normalValues.push_back(params.f(currentX));
        interpolatedValues.push_back(leastSquaresInterpolationValue(va, currentX));
    }
    computePlottingValues();
    displayPlots();

    std::cout << "Returning value from Least Squares Interpolation\n";
    return leastSquaresInterpolationValue(va, value);
}

void InterpolationSolver::resetPlottingValues() {
    normalPoints.clear();
    interpolatedPoints.clear();
    normalValues.clear();
    interpolatedValues.clear();

    minNormal = std::numeric_limits<double>::max();
    maxNormal = std::numeric_limits<double>::min();
    minInter = std::numeric_limits<double>::max();
    maxInter = std::numeric_limits<double>::min();

    numberOfPoints = C_WIDTH / 2 - 10;
    currentX = params.x0;
    deltaX = (params.xN - params.x0) / numberOfPoints;
}

void InterpolationSolver::computePlottingValues() {
    for (int i=0;i<numberOfPoints;++i) {
        minNormal = std::min(minNormal, normalValues[i]);
        maxNormal = std::max(maxNormal, normalValues[i]);
        minInter = std::min(minInter, interpolatedValues[i]);
        maxInter = std::max(maxInter, interpolatedValues[i]);
    }
    double normalMul = (1.0 * C_HEIGHT) / (maxNormal * 2.0); // - minNormal
    double interMul = (1.0 * C_HEIGHT) / (maxInter * 2.0); // - minInter
    for (int i=0;i<numberOfPoints;++i) {
        normalPoints.push_back(std::make_pair(i, normalValues[i] * normalMul));
        interpolatedPoints.push_back(std::make_pair(i, interpolatedValues[i] * interMul));
    }
}

void InterpolationSolver::resetPlot() {
    cleardevice();
    line(0, C_HEIGHT / 2, C_WIDTH, C_HEIGHT / 2);
    line(C_WIDTH / 2, 0, C_WIDTH / 2, C_HEIGHT);
}

void InterpolationSolver::drawPlot(const std::vector<std::pair<int, int>>& points) {
    resetPlot();
    int midX = C_WIDTH / 2;
    int midY = C_HEIGHT / 2;
    for (auto& p : points) {
        int x = midX + p.first;
        int y = midY - p.second;
        line(x, y, x+2, y+2);
    }
}

void InterpolationSolver::displayPlots() {
    std::cout << "Function no. " << params.functionIndex << ", normal plot\n";
    drawPlot(normalPoints);
    getch();
    std::cout << "Function no. " << params.functionIndex << ", interpolated plot\n";
    drawPlot(interpolatedPoints);
    getch();
}

InterpolationSolver::InterpolationSolver(InputFileParams& p) {
    params = p;
}

double InterpolationSolver::solve(double value) {
    std::vector<double> x;
    std::vector<double> y;
    generateXY(x, y);

    if (!params.interpolationType) {
        return leastSquaresInterpolation(x, y, value);
    }
    return splineFunctionInterpolation(x, y, value);
}

double InterpolationSolver::splineFunctionInterpolation(const std::vector<double>& x, const std::vector<double>& y, double value) {
    std::cout << "Entering Spline Function Interpolation\n";
    std::vector<double> A;
    std::vector<double> h;
    A.push_back(params.fd(x[0]));
    for (int i=1;i<params.interpolationPoints;++i) {
        h.push_back(x[i] - x[i-1]);
        A.push_back(-1 * A.back() + 2 * (y[i] - y[i-1]) / h[i-1]);
    }

    resetPlottingValues();
    for (int i=0;i<numberOfPoints;++i, currentX += deltaX) {
        normalValues.push_back(params.f(currentX));
        interpolatedValues.push_back(splineFunctionInterpolationValue(x, y, A, h, currentX));
    }
    computePlottingValues();
    displayPlots();

    std::cout << "Returning value from Spline Function Interpolation\n";
    return splineFunctionInterpolationValue(x, y, A, h, value);
}

double InterpolationSolver::splineFunctionInterpolationValue(const std::vector<double>& x, const std::vector<double>& y,
    const std::vector<double>& A, const std::vector<double>& h, double value) {
    // Se interpoleaza valoarea lui f(value)
    int i0 = -1;
    for (int i=0;i<params.interpolationPoints-1;++i) {
        if (x[i] <= value && value <= x[i+1]) {
            i0 = i;
            break;
        }
    }
    if (i0 == -1) {
        throw "Invalid state!\n";
    }
    double x_xi0 = (value - x[i0]);
    return (((A[i0 + 1] - A[i0]) * x_xi0)/(2 * h[i0])) * x_xi0 + A[i0] * x_xi0 + y[i0];
}
