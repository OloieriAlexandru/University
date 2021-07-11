#include "TridiagonalMatrix.h"

void err(const std::string& message) {
    errorMessage = message;
}

double myAbs(double x) {
    if (x < 0) {
        return -x;
    }
    return x;
}

bool TridiagonalMatrix::readArray(FILE *fileIn, int elCount, vv<double>& arr, std::string arrName) {
    int ret;
    double el;
    for (int i=1;i<=elCount;++i) {
        ret = fscanf(fileIn, "%lf", &el);
        if (ret != 1) {
            err("Invalid file! Failed to read the " + patch::to_string(i) + "'th element of the array \"" + arrName + "\"!"); 
            return false;
        }
        arr.push_back(el);
    }
    return true;
}

vv<pair<int, double>> TridiagonalMatrix::getElementsOnLine(int line, int& cIndex, int& bIndex) const {
    vv<pair<int, double>> tridiagonalElements;
    if (line - q >= 0) {
        tridiagonalElements.push_back(mp(line - q, c[cIndex++]));
    }
    tridiagonalElements.push_back(mp(line, a[line]));
    if (line + p < n) {
        tridiagonalElements.push_back(mp(line + p, b[bIndex++]));
    }
    return tridiagonalElements;
}

int TridiagonalMatrix::size() {
    return n;
}

bool TridiagonalMatrix::init(const char* aFileName, const char* fFileName) {
    FILE *afileIn = fopen(aFileName, "r");
    FILE *fFileIn = fopen(fFileName, "r");
    while (true) {
        if (!afileIn) {
            err("Failed to open \"" + std::string(aFileName) + "\" file!");
            goto invalid_file;
        }
        if (!fFileIn) {
            err("Failed to open \"" + std::string(fFileName) + "\" file!");
            goto invalid_file;
        }

        int ret = fscanf(afileIn, "%d\n%d\n%d\n", &n, &p, &q);
        if (ret != 3) {
            err("Invalid file format! (\"" + std::string(aFileName) + "\") Expected three numbers on the first three lines!");
            goto invalid_file;
        }

        if (!readArray(afileIn, n, a, "a") || !readArray(afileIn, n - q, c, "b") || !readArray(afileIn, n - p, b, "c") ) {
            goto invalid_file;
        }
        int fn;
        ret = fscanf(fFileIn, "%d", &fn);
        if (ret != 1 || fn != n) {
            err("Invalid file format! (\"" + std::string(fFileName) + "\") Expected a number on the first line, equal to the first number in the "
                +std::string(aFileName) + "\" file!");
            goto invalid_file;
        }
        if (!readArray(fFileIn, n, f, "f")) {
            goto invalid_file;
        }

        fclose(afileIn);
        fclose(fFileIn);
        return true;
    }
    invalid_file:
    fclose(afileIn);
    fclose(fFileIn);
    return false;
}

bool TridiagonalMatrix::isDiagonallyDominant() {
    int bIndex = 0, cIndex = 0;
    for (int i=0;i<f.size();++i) {
        vv<pair<int, double>> elements = getElementsOnLine(i, bIndex, cIndex);
        double sum1 = 0.0;
        double sum2 = 0.0;
        for (int j=0;j<elements.size();++j) {
            if (i == elements[j].first) {
                sum1 += myAbs(elements[j].second);
            } else {
                sum2 += myAbs(elements[j].second);
            }
        }
        if (sum1 < sum2) {
            return false;
        }
    }
    return true;
}

vv<double> TridiagonalMatrix::solveEquation() {
    vv<double> res(size(), 0.0);

    int remainingIterations = maxNumberOfIterations;
    double diff = 1e11;
    while (remainingIterations-- && diff > EPS) {
        diff = 0.0;
        int bIndex = 0, cIndex = 0;
        for (int i=0;i<f.size();++i) {
            vv<pair<int, double>> elements = getElementsOnLine(i, bIndex, cIndex);
            double sum = 0.0;
            double diag = 0.0;
            for (auto el : elements) {
                if (el.first == i) {
                    diag = el.second;
                } else {
                    sum += res[el.first] * el.second;
                }
            }
            if (std::abs(diag) < EPS) {
                err("Invalid matrix! A zero element on the diagonal on line " + patch::to_string(i) + "!");
                throw std::invalid_argument("");
            }
            double value = (f[i] - sum) / (1.0 * diag);
            double mul = res[i] - value;
            diff += mul * mul;
            res[i] = value;
        }
        diff = sqrt(diff);
    }
    if (diff > EPS) {
        res = vv<double>(size(), 0.0);
    }
    sol = res;
    return res;
}

vv<double> TridiagonalMatrix::solveEquationEigen() {
    vv<double> res(size(), 0.0);

    // https://eigen.tuxfamily.org/dox/group__TutorialSparse.html
    Eigen::SparseMatrix<double> A(size(), size());
    Eigen::VectorXd b(size());

    std::vector<Eigen::Triplet<double>> coeff;
    coeff.reserve(a.size() + b.size() + c.size());

    int bIndex = 0, cIndex = 0;
    for (int i=0;i<a.size();++i) {
        coeff.push_back(Eigen::Triplet<double>(i, i, a[i]));
        if (i + p < a.size()) {
            coeff.push_back(Eigen::Triplet<double>(i, i+p, this->b[bIndex++]));
        }
        if (i - q >= 0) {
            coeff.push_back(Eigen::Triplet<double>(i, i-q, c[cIndex++]));
        }
        b(i) = f[i];
    }
    A.setFromTriplets(coeff.begin(), coeff.end());

    Eigen::BiCGSTAB<Eigen::SparseMatrix<double>> chol(A);
    Eigen::VectorXd x = chol.solve(b);

    for (int i=0;i<x.size();++i) {
        res[i] = x(i);
    }
    return res;
}

double TridiagonalMatrix::getNorm() {
    if (!sol.size()) {
        err("TridiagonalMatrix::solveEquation() must be called before computing the norm!");
        throw std::logic_error("");
    }
    double err = 0.0;
    int bIndex = 0, cIndex = 0;
    for (int i=0;i<a.size();++i) {
        vv<pair<int, double>> elements = getElementsOnLine(i, bIndex, cIndex);
        double sum = 0.0;
        for (auto el : elements) {
            sum += 1.0 * sol[el.first] * el.second;
        }
        sum -= f[i];
        sum = myAbs(sum);
        err = std::max(err, sum);
    }
    return err;
}

std::ostream& operator << (std::ostream& out, const TridiagonalMatrix& tm) {
    out << "[ \n";
    int bIndex = 0, cIndex = 0;
    for (int i=0;i<tm.n;++i) {
        out << "\t[";
        for (int j=0;j<tm.n;++j) {
            if (i == j) {
                out << tm.a[i] << ", ";
            } else if (j - i == tm.p) {
                out << tm.b[bIndex++] << ", ";
            } else if (i - j == tm.q) {
                out << tm.c[cIndex++] << ", ";
            } else {
                out << "0, ";
            }
        }
        out << "] " << tm.f[i] << '\n';
    }
    out << "]\n";
    return out;
}
