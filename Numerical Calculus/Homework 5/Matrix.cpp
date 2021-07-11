#include "Matrix.h"

void err(const std::string& message) {
    errorMessage = message;
}

double myAbs(double x) {
    if (x < 0) {
        return -x;
    }
    return x;
}

bool Matrix::init(const char *fileName) {
    FILE *fileIn = fopen(fileName, "r");
    while (true) {
        if (!fileIn) {
            err("Failed to open \"" + std::string(fileName) + "\" file!");
            goto invalid_file;
        }

        int ret = fscanf(fileIn, "%d %d\n", &p, &n);
        if (ret != 2) {
            err("Invalid file format! (\"" + std::string(fileName) + "\") Expected two numbers on the first three lines! (n and p)");
            goto invalid_file;
        }

        a.resize(p, vv<double>(n, 0.0));
        for (int i=0;i<p;++i) {
            for (int j=0;j<n;++j) {
                fscanf(fileIn, "%lf", &a[i][j]);
            }
        }

        fclose(fileIn);
        return true;
    }
    invalid_file:
    fclose(fileIn);
    return false;
}

pair<int,int> Matrix::computePQ(const vv<vv<double>>& a) {
    double maxx = -99999999.0;
    int p = -1, q = -1;
    for (int i=0;i<a.size();++i) {
        for (int j=0;j<i;++j) {
            if (myAbs(a[i][j]) > maxx && myAbs(a[i][j]) > EPS) {
                maxx = myAbs(a[i][j]);
                p = i;
                q = j;
            }
        }
    }
    return mp(p, q);
}

pair<double, pair<double,double>> Matrix::computeTCS(const vv<vv<double>>& a, pair<int,int> pq) {
    if (pq.first == -1) {
        return mp(0.0, mp(0.0, 0.0));
    }
    int p = pq.first;
    int q = pq.second;
    double alpha = (1.0 * (a[p][p] - a[q][q])) / (2.0 * a[p][q]);
    double sign = alpha >= 0 ? 1.0 : -1.0;
    double t = -alpha + sign * sqrt((alpha * alpha) + 1);
    double c = 1.0 / sqrt(1 + t * t);
    double s = t / sqrt(1.0 + t * t);
    return mp(t, mp(c, s));
}

vv<vv<double>> Matrix::generateIn(int n) {
    vv<vv<double>> in;
    in.resize(n, vv<double>(n, 0.0));
    for (int i=0;i<n;++i) {
        in[i][i] = 1.0;
    }
    return in;
}

void Matrix::task1(std::ofstream& out) {
    MatrixXd aInit(p, n);
    for (int i=0;i<p;++i) {
        for (int j=0;j<n;++j) {
            aInit(i, j) = a[i][j];
        }
    }

    int iter = maxNumberOfIterations;
    pair<int, int> pq = computePQ(a);
    pair<double, pair<double,double>> tcs = computeTCS(a, pq);
    vv<vv<double>> u = generateIn(n);
    while (iter-- && pq.first != -1) {
        vv<vv<double>> newA;
        double t = tcs.first;
        double c = tcs.second.first;
        double s = tcs.second.second;
        newA = a;

        for (int j=0;j<n;++j) {
            if (j != pq.first && j != pq.second) {
                newA[pq.first][j] = c * a[pq.first][j] + s * a[pq.second][j];
                newA[pq.second][j] = -s * a[j][pq.first] + c * a[pq.second][j];
                newA[j][pq.second] = -s * a[j][pq.first] + c * a[pq.second][j];
                newA[j][pq.first] = a[pq.first][j];
            }
        }
        newA[pq.first][pq.first] = a[pq.first][pq.first] + t * a[pq.first][pq.second];
        newA[pq.second][pq.second] = a[pq.second][pq.second] - t * a[pq.first][pq.second];
        newA[pq.first][pq.second] = newA[pq.second][pq.first] = 0.0;

        for (int i=0;i<n;++i) {
            double old = u[i][pq.first];
            u[i][pq.first] = c * u[i][pq.first] + s * u[i][pq.second];
            u[i][pq.second] = -s * old + c * u[i][pq.second];
        }

        a = newA;
        pq = computePQ(a);
        tcs = computeTCS(a, pq);
    }
    out << "-----------------------------------\n";
    out << "Valorile proprii:\n\n";
    for (int i=0;i<n;++i) {
        out << a[i][i] << ", ";
    }
    out << "\n\n-----------------------------------\n";
    out << "Vectorii proprii:\n\n";
    for (int j=0;j<n;++j) {
        for (int i=0;i<n;++i) {
            out << u[i][j] << ", ";
        }
        out << '\n';
    }

    MatrixXd U(p, n);
    MatrixXd lambda(p, n);
    for (int i=0;i<p;++i) {
        for (int j=0;j<n;++j) {
            U(i, j) = u[i][j];
            
            if (i == j) {
                lambda(i, j) = a[i][i];
            } else {
                lambda(i, j) = 0.0;
            }
        }
    }

    MatrixXd prod1 = aInit * U;
    MatrixXd prod2 = U * lambda;

    out << "\n-----------------------------------\n";
    out << "A^(init) * U:\n" << prod1 << '\n';
    out << "U * Λ:\n" << prod2 << '\n';
    out << "|| A^(init) * U - U * Λ ||:\n" << (prod1 - prod2).norm() << '\n';
}

void Matrix::task2(std::ofstream& out) {
    MatrixXd eigenA(p, n);
    MatrixXd lastEigenA(p, n);
    MatrixXd lastL;
    for (int i=0;i<p;++i) {
        for (int j=0;j<n;++j) {
            lastEigenA(i, j) = a[i][j];
        }
    }
    double diff = 100.0;
    int iter = maxNumberOfIterations;
    while (iter-- && myAbs(diff) >= EPS) {
        lastL = lastEigenA.llt().matrixL();
        eigenA = lastL.transpose() * lastL;
        diff = (eigenA - lastEigenA).norm();
        lastEigenA = eigenA;
    }
    out << "\n-----------------------------------\n";
    out << "A^k:\n" << lastEigenA << '\n';
}

void Matrix::task3(std::ofstream& out) {
    MatrixXd eigenA(p, n);
    for (int i=0;i<p;++i) {
        for (int j=0;j<n;++j) {
            eigenA(i, j) = a[i][j];
        }
    }
    out << "-------------------------\n";
    out << "Task 3 - using eigen C++ library:\n";

    Eigen::JacobiSVD<MatrixXd> svd(eigenA, Eigen::ComputeFullV | Eigen::ComputeFullU);
    VectorXd singularValues = svd.singularValues();
    out << "\nSingular values:\n" << singularValues << '\n';
    MatrixXd U = svd.matrixU(); 
    out << "\nMatrix U:\n" << U << '\n';
    MatrixXd VT = svd.matrixV().transpose();
    out << "\nMatrix V^T:\n" << VT << '\n';

    int matrixRank = 0;
    double sigmaMax = -99999999999.0;
    double sigmaMin = 99999999999.0;
    MatrixXd SI(p, n);
    for (int i=0;i<singularValues.size();++i) {
        if (myAbs(singularValues(i)) > EPS) {
            ++matrixRank;
            if (singularValues(i) > sigmaMax) {
                sigmaMax = singularValues(i);
            }
            if (singularValues(i) < sigmaMin) {
                sigmaMin = singularValues(i);
            }
            SI(i, i) = 1.0 / singularValues(i);
        }
    }
    out << "\nMatrix rank:\n" << matrixRank << '\n';
    out << "\nMatrix rank:\n" << svd.rank() << " (eigen)\n";
    out << "\nCondition number of matrix:\n" << sigmaMax / sigmaMin << '\n';
    // https://stackoverflow.com/questions/33575478/how-can-you-find-the-condition-number-in-eigen/33577450
    // double cond = 0.0; // svd.singularValues()(0) / svd.singularValues()(svd.singularValues().size()-1);
    out << "\nCondition number of matrix:\n" << "??" << " (eigen)\n";

    MatrixXd AI = VT.transpose() * SI * U.transpose();
    // https://comnuan.com/cmnn0100f/cmnn0100f.php
    out << "\nMoore-Penrose pseudoinverse of matrix:\n" << AI << '\n';

    MatrixXd AJ = ((eigenA.transpose() * eigenA).inverse()) * eigenA.transpose();
    out << "\nPseudoinverse least squares of matrix:\n" << AJ << '\n';

    double diff = (AI - AJ).norm();
    out << "\n||A^I - A^J|| = " << diff << '\n';

    out << "\n";
}

bool Matrix::solve(std::ofstream& out) {
    if (!n) {
        err("You should initialize the matrix first! (You can initialize the matrix by calling Matrix::init() )");
        return false;
    }
    if (n == p) {
        task3(out);
        task1(out);
    } else {
        task3(out);
    }
    task2(out);
    return true;
}

std::ostream& operator << (std::ostream& out, const Matrix& mat) {
    out << "[ \n";
    int bIndex = 0, cIndex = 0;
    for (int i=0;i<mat.p;++i) {
        out << "\t[";
        for (int j=0;j<mat.n;++j) {
            out << mat.a[i][j] << ", ";
        }
        out << "]\n";
    }
    out << "]\n";
    return out;
}
