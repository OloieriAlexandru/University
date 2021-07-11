#ifndef INTERPOLATION_SOLVER_H
#define INTERPOLATION_SOLVER_H

#include <cmath>
#include <vector>
#include <random>
#include <algorithm>
#include <iostream>
#include <utility>
#include <limits>

#include <graphics.h>

#include <Eigen/Dense>
using Eigen::MatrixXd;
using Eigen::VectorXd;

extern const int C_WIDTH;
extern const int C_HEIGHT;
extern const double PI;

extern double ssin(double x);
extern double ccos(double x);

struct InputFileParams {
	double x0;
	double xN;
	int interpolationType;
    int interpolationPoints;
    int functionIndex;
	double (*f)(double);
	double (*fd)(double);
};

class InterpolationSolver {
    private:
        InputFileParams params;

        void generateXY(std::vector<double>& x, std::vector<double>& y);

        double leastSquaresInterpolationValue(const std::vector<double>& a, double value);

        double leastSquaresInterpolation(const std::vector<double>& x, const std::vector<double>& y, double value);

        double splineFunctionInterpolation(const std::vector<double>& x, const std::vector<double>& y, double value);

        double splineFunctionInterpolationValue(const std::vector<double>& x, const std::vector<double>& y,
            const std::vector<double>& A, const std::vector<double>& h, double value);

        // Function plot

        int numberOfPoints;
        double currentX;
        double deltaX;
        std::vector<std::pair<int,int>> normalPoints;
        std::vector<std::pair<int,int>> interpolatedPoints;
        std::vector<double> normalValues;
        std::vector<double> interpolatedValues;
        double minNormal;
        double maxNormal;
        double minInter;
        double maxInter;

        void resetPlottingValues();

        void computePlottingValues();

        void resetPlot();

        void drawPlot(const std::vector<std::pair<int, int>>& points);

        void displayPlots();
    public:
        InterpolationSolver(InputFileParams& p);

        double solve(double value);
};

#endif // INTERPOLATION_SOLVER_H

