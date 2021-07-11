#include "Matrix.h"

std::string errorMessage;

// https://stackoverflow.com/questions/2704521/generate-random-double-numbers-in-c/9324796
double fRand(double fMin, double fMax) {
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

void err(const std::string message) {
    errorMessage = message;
}

void Matrix::initFrom(const Matrix& m) {
    n = m.n;
    mat = m.mat;
    b = m.b;
}

bool Matrix::validMatrix() {
    for (int i=0;i<n;++i) {
        if (abs(mat[i][i]) < EPS) {
            err("Invalid file! The matrix is not positive definite!\n");
            return false;
        }
        d.push_back(mat[i][i]);
        for (int j=0;j<n;++j) {
            if (mat[i][j] != mat[j][i]) {
                err("Invalid file! The matrix is not symmetric!");
                return false;
            }
        }
    }
    return true;
}

Matrix::Matrix(const Matrix& m) {
    initFrom(m);
}

void Matrix::transpose() {
    int nn = mat.size();
    if (nn <= 0) {
        return;
    }
    int mm = mat[0].size();
    vv<vv<double>> trans(mm, vv<double>(nn, 0.0));
    for (int i=0;i<nn;++i){
        for (int j=0;j<mm;++j) {
            trans[j][i] = mat[i][j];
        }
    }
    n = mm;
    mat = trans;
}

int Matrix::size() {
    return n;
}

double Matrix::get(int i, int j) {
    if (!(i >= 0 && i < mat.size() && j >= 0 && mat.size() && j < mat[0].size())) {
        return 0.0;
    }
    if (j > i) {
        return mat[i][j];
    } else if (j < i) {
        return mat[j][i];
    } else {
        return d[i];
    }
}

int Matrix::bsize() {
    return (int)b.size();
}

double Matrix::getb(int i) {
    if (!(i >=0 && i < b.size())) {
        return 0.0;
    }
    return b[i];
}

bool Matrix::init(const char* fileName, bool readEquation) {
    FILE *fileIn = fopen(fileName, "r");
    while (true) {
        if (!fileIn) {
            err("Failed to open \"" + std::string(fileName) + "\" file!");
            goto invalid_file;
        }
        int ret = fscanf(fileIn, "%d\n", &n);
        if (ret != 1) {
            err("Invalid file format! (\"" + std::string(fileName) + "\") Expected a number on the first line!");
            goto invalid_file;
        }
        double el;
        for (int i=0;i<n;++i) {
            mat.push_back(vv<double>());
            for (int j=0;j<n;++j) {
                if (j+1 == n) {
                    ret = fscanf(fileIn, "%lf\n", &el);
                } else {
                    ret = fscanf(fileIn, "%lf ", &el);
                }
                if (ret != 1) {
                    err("Invalid file! Matrix line: " + patch::to_string(i+1) + ", column: " + patch::to_string(j+1));
                    goto invalid_file;
                }
                mat.back().push_back(el);
            }
        }
        if (!validMatrix()) {
            goto invalid_file;
        }
        if (readEquation) {
            for (int i=0;i<n;++i) {
                if (1 != fscanf(fileIn, "%lf", &el)) {
                    err("Invalid file! Couldn't read the right side of the equation - a vector with " + 
                        patch::to_string(n) + " elements!");
                    goto invalid_file;
                }
                b.push_back(el);
            }
        }
        return true;
    }
    invalid_file:
    fclose(fileIn);
    return false;
}

void Matrix::initRandom(int size, double minRandValue, double maxRandValue) {
    n = size;
    mat.resize(n, vv<double>(n, 0.0));
    b.resize(n, 0.0);

    MatrixXd randomEigen(n, n);
    for (int i=0;i<n;++i) {
        for (int j=0;j<n;++j) {
            randomEigen(i, j) = fRand(minRandValue, maxRandValue);
        }
    }
    randomEigen = randomEigen * randomEigen.transpose();
    for (int i=0;i<n;++i) {
        for (int j=0;j<n;++j) {
            mat[i][j] = randomEigen(i, j);
            if (i == j) {
                d.push_back(mat[i][j]);
            }
        }
        b[i] = fRand(minRandValue * n, maxRandValue * n);
    }
}

bool Matrix::initConsole() {
    std::cout << "Matrix size: ";
    std::cin >> n;
    mat.resize(n, vv<double>(n, 0.0));
    b.resize(n, 0.0);
    for (int i=0;i<n;++i) {
        for (int j=0;j<n;++j) {
            std::cout << "Element at position (" << i << ", " << j << ") = ";
            std::cin >> mat[i][j];
        }
    }
    if (!validMatrix()) {
        return false;
    }
    for (int i=0;i<n;++i) {
        std::cout << "b[" << i << "] = ";
        std::cin >> b[i];
    }
    return true;
}

bool Matrix::choleskyFactorization() {
    if (cholesky != 0) {
        err("You have already computed the Cholesky factorization!");
        return false;
    }
    for (int p=0;p<n;++p) {
        if (abs(mat[p][p]) < EPS) {
            err("Cannot compute the Cholesky factorization for the input matrix!");
            return false;
        }
        // Se calculeaza coloana p
        for (int i=p;i<n;++i) {
            double sum = 0;
            if (i == p) {
                for (int j=0;j<p;++j) {
                    sum += mat[p][j] * mat[p][j];
                }
                mat[p][p] = sqrt(mat[p][p] - sum);
            } else {
                for (int j=0;j<p;++j) {
                    sum += mat[p][j] * mat[i][j];
                }
                mat[i][p] = (mat[i][p] - sum) / mat[p][p];
            }
        }
    }
    ++cholesky;
    return true;
}

void Matrix::printCholeskyFactorization(std::ostream& out) {
    out << "L\n";
    for (int i=0;i<n;++i) {
        for (int j=0;j<n;++j) {
            if (j <= i) {
                out << mat[i][j] << ' ';
            } else {
                out << 0.0 << ' ';
            }
        }
        out << '\n';
    }
    out << "L^T\n";
    for (int i=0;i<n;++i) {
        for (int j=0;j<n;++j) {
            if (i <= j) {
                out << mat[j][i] << ' ';
            } else {
                out << 0.0 << ' ';
            }
        }
        out << '\n';
    }
}

double Matrix::computeDeterminant() {
    if (cholesky < 1) {
        err("You haven't computed the Cholesky factorization!");
        return 1e6;
    }
    double ans = mat[0][0];
    for (int i=1;i<n;++i) {
        ans *= mat[i][i];
    }
    return ans * ans;
}

double Matrix::getNorm(const Matrix& x) {
    double err = 0.0;
    for (int i=0;i<n;++i) {
        double sum = 0;
        for (int j=0;j<n;++j) {
            if (j > i) {
                sum += mat[i][j] * x.mat[j][0];
            } else if (j < i) {
                sum += mat[j][i] * x.mat[j][0];
            } else {
                sum += d[i] * x.mat[j][0];
            }
        }
        sum -= b[i];
        err += sum * sum;
    }
    if (abs(err) >= EPS) {
        err = sqrt(err);
    }
    return err;
}

Matrix Matrix::solveEquation() {
    if (n != b.size()) {
        throw new std::logic_error("Cannot compute the solution of the equation! The values of the vector \"b\" are unknown!");
    }
    Matrix res;
    res.n = n;

    // direct substitution method
    for (int i=0;i<n;++i) {
        res.mat.push_back(vv<double>());
        double sum = 0.0;
        for (int j=0;j<i;++j) {
            sum += mat[i][j] * res.mat[j][0];
        }
        res.mat.back().push_back((b[i]-sum) / mat[i][i]);
    }
    // reverse substitution method
    for (int i=n-1;i>=0;--i) {
        double sum = 0.0;
        for (int j=i+1;j<n;++j) {
            sum += mat[j][i] * res.mat[j][0];
        }
        res.mat[i][0] = (res.mat[i][0] - sum) / mat[i][i];
    }

    return res;
}

Matrix Matrix::getInverse() {
    if (cholesky < 1) {
        throw std::logic_error("Cannot compute the determinant! The Cholesky factorization wasn't computed!");
    }
    Matrix res;
    res.n = n;
    res.mat.resize(n);

    vv<double> mi(n, 0.0);
    for (int i=0;i<n;++i) {
        mi[i] = 1.0;
        Matrix cp = *this;
        cp.setB(mi);
        Matrix coli = cp.solveEquation();
        for (int j=0;j<n;++j) {
            res.mat[j].push_back(coli.mat[j][0]);
        }
        mi[i] = 0.0;
    }

    return res;
}

Matrix Matrix::getB() {
    Matrix res;
    res.n = b.size();
    res.mat.resize(1);
    res.mat[0] = b;
    return res;
}

void Matrix::setB(const vv<double>& sb) {
    b = sb;
}

double Matrix::getNorm2(const VectorXd& eigenVector) {
    double norm = 0.0, mul;
    for (int i=0;i<mat.size();++i) {
        mul = mat[i][0] - eigenVector(i);
        norm += mul * mul;
    }
    return sqrt(norm);
}

double Matrix::getNorm1(const MatrixXd& eigenInverse) {
    double norm = 0.0;
    for (int i=0;i<mat.size();++i) {
        double sum = 0.0;
        for (int j=0;j<eigenInverse.cols();++j) {
            sum += abs(mat[i][j] - eigenInverse(i, j));
        }
        norm = std::max(norm, sum);
    }
    return norm;
}

Matrix& Matrix::operator = (const Matrix& m) {
    initFrom(m);
}

Matrix operator * (const Matrix& mat1, const Matrix& mat2) {
    if (!mat1.mat.size() || !mat2.mat.size() || !mat2.mat[0].size()
            || !mat1.mat[0].size() || mat1.mat[0].size() != mat2.mat.size()) {
        return Matrix();
    }
    Matrix res;

    res.mat.resize(mat1.mat.size(), vv<double>(mat2.mat[0].size(), 0.0));
    for (int i=0;i<mat1.mat.size();++i) {
        for (int j=0;j<mat2.mat[0].size();++j) {
            for (int k=0;k<mat1.mat[0].size();++k) {
                if (k > i) {
                    res.mat[i][j] += mat1.mat[i][k] * mat2.mat[k][j];
                } else if (k < i) {
                    res.mat[i][j] += mat1.mat[k][i] * mat2.mat[k][j];
                } else {
                    res.mat[i][j] += mat1.d[i] * mat2.mat[k][j];
                }
            }
        }
    }

    return res;
}

std::ostream& operator << (std::ostream& out, const Matrix& mat) {
    for (auto& line : mat.mat) {
        for (auto& el : line) {
            out << el << ' ';
        }
        out << '\n';
    }
    return out;
}

void MatrixBonus::initFrom(const Matrix& m) {
    for (int i=0;i<m.n;++i) {
        for (int j=0;j<m.n;++j) {
            if (j > i) {
                continue;
            }
            if (i == j) {
                A.push_back(m.d[i]);
            } else {
                A.push_back(m.mat[j][i]);
            }
            L.push_back(0.0);
        }
    }
    b = m.b;
    n = m.n;
}

void MatrixBonus::initFrom(const MatrixBonus& m) {
    n = m.n;
    b = m.b;
    A = m.A;
    L = m.L;
    cholesky = m.cholesky;
}

MatrixBonus::MatrixBonus(const Matrix& m) {
    initFrom(m);
    cholesky = false;
}

MatrixBonus::MatrixBonus(const MatrixBonus& m) {
    initFrom(m);
}

#define INDEX_TRANSLATION_L(i, j) ((i*(i+1)/2)+j)

bool MatrixBonus::choleskyFactorization() {
    if (cholesky) {
        return true;
    }
    for (int p=0;p<n;++p) {
        if (abs(A[INDEX_TRANSLATION_L(p, p)]) < EPS) {
            err("Cannot compute the Cholesky factorization for the input matrix!");
            return false;
        }
        for (int i=p;i<n;++i) {
            double sum = 0;
            if (i == p) {
                for (int j=0;j<p;++j) {
                    sum += L[INDEX_TRANSLATION_L(p, j)] * L[INDEX_TRANSLATION_L(p, j)];
                }
                L[INDEX_TRANSLATION_L(p, p)] = sqrt(A[INDEX_TRANSLATION_L(p, p)] - sum);
            } else {
                for (int j=0;j<p;++j) {
                    sum += L[INDEX_TRANSLATION_L(p, j)] * L[INDEX_TRANSLATION_L(i, j)];
                }
                L[INDEX_TRANSLATION_L(i, p)] = (A[INDEX_TRANSLATION_L(i, p)] - sum) / L[INDEX_TRANSLATION_L(p, p)];
            }
        }
    }
    cholesky = true;
    return true;
}

void MatrixBonus::printCholeskyFactorization(std::ostream& out) {
    if (!cholesky) {
        return;
    }
    out << "L\n";
    int next = 0;
    for (int i=0;i<n;++i) {
        for (int j=0;j<n;++j) {
            if (j <= i) {
                out << L[next++] << ' ';
            } else {
                out << 0.0 << ' ';
            }
        }
        out << '\n';
    }
    out << "L^T\n";
    for (int i=0;i<n;++i) {
        for (int j=0;j<n;++j) {
            if (j >= i) {
                out << L[INDEX_TRANSLATION_L(j, i)] << ' ';
            } else {
                out << 0.0 << ' ';
            }
        }
        out << '\n';
    }
}

Matrix MatrixBonus::solveEquation() {
    Matrix res;
    res.n = n;

    // direct substitution method
    for (int i=0;i<res.n;++i) {
        res.mat.push_back(vv<double>());
        double sum = 0.0;
        for (int j=0;j<i;++j) {
            sum += L[INDEX_TRANSLATION_L(i, j)] * res.mat[j][0];
        }
        res.mat.back().push_back((b[i]-sum) / L[INDEX_TRANSLATION_L(i, i)]);
    }
    // reverse substitution method
    for (int i=res.n-1;i>=0;--i) {
        double sum = 0.0;
        for (int j=i+1;j<res.n;++j) {
            sum += L[INDEX_TRANSLATION_L(j, i)] * res.mat[j][0];
        }
        res.mat[i][0] = (res.mat[i][0] - sum) / L[INDEX_TRANSLATION_L(i, i)];
    }

    return res;
}

MatrixBonus& MatrixBonus::operator = (const MatrixBonus& m) {
    initFrom(m);
}

std::ostream& operator << (std::ostream& out, const MatrixBonus& mb) {
    for (int i=0;i<mb.n;++i) {
        for (int j=0;j<mb.n;++j) {
            if (j <= i) {
                out << mb.A[INDEX_TRANSLATION_L(i, j)] << ' ';
            } else {
                out << mb.A[INDEX_TRANSLATION_L(j, i)] << ' ';
            }
        }
        out << '\n';
    }
    return out;
}
