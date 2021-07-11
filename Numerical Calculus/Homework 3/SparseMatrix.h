#ifndef SPARSE_MATRIX_H
#define SPARSE_MATRIX_H

#include <cstring>
#include <cstdio>
#include <string>
#include <vector>
#include <map>
#include <ostream>
#include <stdexcept>

#include <iostream>

#include "TridiagonalMatrix.h"

const int maxNumberOfElementsOnLine = 20;

#define vv std::vector
#define map std::map
#define pair std::pair

std::string errorMessage;

void err(const std::string& message) {
    errorMessage = message;
}

// cmp = comparison function
// read = read function
template <typename T, bool (*cmp)(const T&, const T&), int (*read)(FILE*, T&, int&, int&)>
class SparseMatrix {
    private:
        static const T zero;

        template <typename TT, bool (*mcmp)(const TT&, const TT&)>
        struct MatrixElement {
            int position;

            TT element;

            MatrixElement() {
                position = 0;
                memset(&element, 0, sizeof(element));
            }

            MatrixElement(int pos, TT el) {
                position = pos;
                element = el;
            }

            MatrixElement(const MatrixElement& el) {
                position = el.position;
                element = el.element;
            }

            template <typename TTri>
            MatrixElement(int pos, const TTri& el) {
                position = pos;
                element = el;
            }

            bool operator == (const MatrixElement<TT, mcmp>& oth) {
                return !(*this != oth);
            }

            MatrixElement<TT, mcmp>& operator = (const MatrixElement<TT, mcmp>& el) {
                position = el.position;
                element = el.element;
                return *this;
            }

            MatrixElement<TT, mcmp>& operator += (const MatrixElement<TT, mcmp>& el) {
                element += el.element;
                return *this;
            }

            friend MatrixElement<TT, mcmp> operator + (const MatrixElement<TT, mcmp>& el1, const MatrixElement<TT, mcmp>& el2) {
                return MatrixElement<TT, mcmp>(el1.position, el1.element + el2.element);
            }

            template <typename TTri>
            friend MatrixElement<TT, mcmp> operator + (const MatrixElement<TT, mcmp>& el1, const TTri& add) {
                return MatrixElement<TT, mcmp>(el1.position, el1.element + add);
            }

            friend bool operator != (const MatrixElement<TT, mcmp>& el1, const MatrixElement<TT, mcmp>& el2) {
                if (el1.position != el2.position) {
                    return false;
                }
                return !mcmp(el1.element, el2.element);
            }

            friend std::ostream& operator << (std::ostream& out, const MatrixElement<TT, mcmp>& el) {
                out << "{ column: " << el.position << ", value: " << el.element << " }";
                return out;
            }
        };

        int n;

        vv<vv<MatrixElement<T, cmp>>> mat;

        void resz(int sz) {
            n = sz;
            mat.resize(n, vv<MatrixElement<T, cmp>>());
        }
        
        template <typename TTri, bool (*cmpTri)(const TTri&, const TTri&), int (*readTri)(FILE*, TTri&)>
        void fromTridiagonalMatrix(const TridiagonalMatrix<TTri, cmpTri, readTri>& othMat) {
            n = othMat.n;
            mat.resize(n, vv<MatrixElement<T, cmp>>());
            int bIndex = 0, cIndex = 0;
            for (int i=0;i<n;++i) {
                vv<pair<int, TTri>> tridiagonalElements = othMat.getElementsOnLine(i, bIndex, cIndex);
                for (int j=0;j<tridiagonalElements.size();++j) {
                    addElement(i, MatrixElement<T, cmp>(tridiagonalElements[j].first, tridiagonalElements[j].second));
                }
            }
        }

    public:
        SparseMatrix()
        : n(0) {
        }

        SparseMatrix(const SparseMatrix<T, cmp, read>& othMat) {
            n =  othMat.n;
            mat = othMat.mat;
        }

        template <typename TTri, bool (*cmpTri)(const TTri&, const TTri&), int (*readTri)(FILE*, TTri&)>
        SparseMatrix(const TridiagonalMatrix<TTri, cmpTri, readTri>& mat) {
            fromTridiagonalMatrix(mat);
        }

        int size() const {
            return n;
        }

        bool init(const char* fileName, bool checkNumberOfElementsOnLine = false) {
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
                resz(n);
                vv<map<int,MatrixElement<T, cmp>>> elements(n);
                int fileLine = 0;
                while (++fileLine) {
                    T el;
                    int line, column;
                    ret = read(fileIn, el, line, column);
                    if (ret == EOF) {
                        break;
                    }
                    if (ret != 3) {
                        err("Invalid file format! Line number " + patch::to_string(fileLine) + " doesn't respect the specifications!\n");
                        goto invalid_file;
                    }
                    if ((line+1) > n) {
                        err("Invalid element on line " + patch::to_string(fileLine) + ", the line is greater than the matrix size!" +
                            patch::to_string(line+1) + " > " + patch::to_string(n) );
                        goto invalid_file;
                    }
                    if ((column+1) > n) {
                        err("Invalid element on line " + patch::to_string(fileLine) + ", the column is greater than the matrix size!" +
                            patch::to_string(column+1) + " > " + patch::to_string(n) );
                        goto invalid_file;
                    }
                    if (elements[line].count(column)) {
                        elements[line][column] += MatrixElement<T, cmp>(column, el);
                    } else {
                        elements[line][column] = MatrixElement<T, cmp>(column, el);
                    }
                }
                for (int i=0;i<elements.size();++i) {
                    if (checkNumberOfElementsOnLine) {
                        if (elements[i].size() > maxNumberOfElementsOnLine) {
                            err("Invalid file! (" + std::string(fileName) + ") There are more than " + patch::to_string(maxNumberOfElementsOnLine) + 
                                " elements on line " + patch::to_string(i) + " (" + 
                                patch::to_string(elements[i].size()) + ") elements!");
                            goto invalid_file;
                        }
                    }
                    
                    for (auto& el : elements[i]) {
                        mat[i].push_back(el.second);
                    }
                }
                return true;
            }
            invalid_file:
            fclose(fileIn);
            return false;
        }

        void addElement(int line, int position, T value) {
            addElement(MatrixElement<T, cmp>(position, value));
        }

        void addElement(int line, const MatrixElement<T, cmp>& el) {
            if (cmp(el.element, zero)) {
                return;
            }
            mat[line].push_back(el);
        }

        SparseMatrix<T, cmp, read>& operator = (const SparseMatrix<T, cmp, read>& othMat) {
            n = othMat.n;
            mat = othMat.mat;
            return *this;
        }

        template <typename TTri, bool (*cmpTri)(const TTri&, const TTri&), int (*readTri)(FILE*, TTri&)>
        SparseMatrix<T, cmp, read>& operator = (const SparseMatrix<T, cmp, read>& othMat) {
            fromTridiagonalMatrix(othMat);
            return *this;
        }

        template <typename TTri, bool (*cmpTri)(const TTri&, const TTri&), int (*readTri)(FILE*, TTri&)>
        friend SparseMatrix<T, cmp, read> operator + (const SparseMatrix<T, cmp, read>& mat1, const TridiagonalMatrix<TTri, cmpTri, readTri>& mat2) {
            if (mat1.n != mat2.n) {
                throw std::invalid_argument("Can't add matrices (sparse + tridiagonal) with different sizes! The sizes are: " + patch::to_string(mat1.n) + " and " +
                    patch::to_string(mat2.n) + "!");
            }

            SparseMatrix<T, cmp, read> res;
            res.resz(mat1.n);
            int bIndex = 0, cIndex = 0;
            for (int i=0;i<mat1.n;++i) {
                vv<pair<int, TTri>> tridiagonalElements = mat2.getElementsOnLine(i, bIndex, cIndex);

                int jj1 = 0, jj2 = 0;
                int n = mat1.mat[i].size(), m = tridiagonalElements.size();
                while (jj1 < n && jj2 < m) {
                    if (mat1.mat[i][jj1].position == tridiagonalElements[jj2].first) {
                        res.addElement(i, mat1.mat[i][jj1] + tridiagonalElements[jj2].second);
                        ++jj1, ++jj2;
                    } else if (mat1.mat[i][jj1].position < tridiagonalElements[jj2].first) {
                        res.addElement(i, mat1.mat[i][jj1]);
                        ++jj1;
                    } else {
                        res.addElement(i, MatrixElement<T, cmp>(tridiagonalElements[jj2].first, tridiagonalElements[jj2].second));
                        ++jj2;
                    }
                }
                while (jj1 < n) {
                    res.addElement(i, mat1.mat[i][jj1]);
                    ++jj1;
                }
                while (jj2 < m) {
                    res.addElement(i, MatrixElement<T, cmp>(tridiagonalElements[jj2].first, tridiagonalElements[jj2].second));
                    ++jj2;
                }
            }

            return res;
        }

        friend SparseMatrix<T, cmp, read> operator + (const SparseMatrix<T, cmp, read>& mat1, const SparseMatrix<T, cmp, read>& mat2) {
            if (mat1.n != mat2.n) {
                throw std::invalid_argument("Can't add matrices with different sizes! The sizes are: " + patch::to_string(mat1.n) + " and " +
                    patch::to_string(mat2.n) + "!");
            }

            SparseMatrix<T, cmp, read> res;
            res.resz(mat1.n);
            for (int i=0;i<mat1.n;++i) {
                int jj1 = 0, jj2 = 0;
                int n = mat1.mat[i].size(), m = mat2.mat[i].size();
                while (jj1 < n && jj2 < m) {
                    if (mat1.mat[i][jj1].position == mat2.mat[i][jj2].position) {
                        res.addElement(i, mat1.mat[i][jj1] + mat2.mat[i][jj2]);
                        ++jj1, ++jj2;
                    } else if (mat1.mat[i][jj1].position < mat2.mat[i][jj2].position) {
                        res.addElement(i, mat1.mat[i][jj1]);
                        ++jj1;
                    } else {
                        res.addElement(i, mat2.mat[i][jj2]);
                        ++jj2;
                    }
                }
                while (jj1 < n) {
                    res.addElement(i, mat1.mat[i][jj1]);
                    ++jj1;
                }
                while (jj2 < m) {
                    res.addElement(i, mat2.mat[i][jj2]);
                    ++jj2;
                }
            }

            return res;
        }

        template <typename TTri, bool (*cmpTri)(const TTri&, const TTri&), int (*readTri)(FILE*, TTri&)>
        friend SparseMatrix<T, cmp, read> operator * (const SparseMatrix<T, cmp, read>& mat1, const TridiagonalMatrix<TTri, cmpTri, readTri>& mat2) {
            if (mat1.n != mat2.n) {
                throw std::invalid_argument("Can't multiply matrices (sparse * tridiagonal) with different sizes! The sizes are: " + patch::to_string(mat1.n) + " and " +
                    patch::to_string(mat2.n) + "!");
            }
            SparseMatrix<T, cmp, read> res;
            TridiagonalMatrix<TTri, cmpTri, readTri> t = ~mat2;

            res.resz(mat1.size());
            for (int i=0;i<mat1.size();++i) {
                int bIndex = 0, cIndex = 0;
                for (int j=0;j<t.size();++j) {
                    vv<pair<int, TTri>> tridiagonalElements = t.getElementsOnLine(j, bIndex, cIndex);
                    T el = 0;
                    memset(&el, 0, sizeof(T));
                    int jj1 = 0, jj2 = 0;
                    int n = mat1.mat[i].size(), m = tridiagonalElements.size();

                    while (jj1 < n && jj2 < m) {
                        if (mat1.mat[i][jj1].position == tridiagonalElements[jj2].first) {
                            el += mat1.mat[i][jj1].element * tridiagonalElements[jj2].second;
                            ++jj1, ++jj2;
                        } else if (mat1.mat[i][jj1].position < tridiagonalElements[jj2].first) {
                            ++jj1;
                        } else {
                            ++jj2;
                        }
                    }

                    if (cmp(el, zero)) {
                        continue;
                    }
                    res.addElement(i, MatrixElement<T, cmp>(j, el));
                }
            }

            return res;
        }

        friend SparseMatrix<T, cmp, read> operator * (const SparseMatrix<T, cmp, read>& mat1, const SparseMatrix<T, cmp, read>& mat2) {
            if (mat1.n != mat2.n) {
                throw std::invalid_argument("Can't multiply matrices with different sizes! The sizes are: " + patch::to_string(mat1.n) + " and " +
                    patch::to_string(mat2.n) + "!");
            }
            SparseMatrix<T, cmp, read> res;
            SparseMatrix<T, cmp, read> t = ~mat2;

            res.resz(mat1.size());
            for (int i=0;i<mat1.size();++i) {
                for (int j=0;j<t.size();++j) {
                    T el = 0;
                    memset(&el, 0, sizeof(T));
                    int jj1 = 0, jj2 = 0;
                    int n = mat1.mat[i].size(), m = t.mat[j].size();

                    while (jj1 < n && jj2 < m) {
                        if (mat1.mat[i][jj1].position == t.mat[j][jj2].position) {
                            el += mat1.mat[i][jj1].element * t.mat[j][jj2].element;
                            ++jj1, ++jj2;
                        } else if (mat1.mat[i][jj1].position < t.mat[j][jj2].position) {
                            ++jj1;
                        } else {
                            ++jj2;
                        }
                    }

                    if (cmp(el, zero)) {
                        continue;
                    }
                    res.addElement(i, MatrixElement<T, cmp>(j, el));
                }
            }

            return res;
        }

        friend SparseMatrix<T, cmp, read> operator ~ (const SparseMatrix<T, cmp, read>& mat) {
            SparseMatrix<T, cmp, read> res;

            res.resz(mat.size());
            vv<map<int,MatrixElement<T, cmp>>> elements(mat.n);

            for (int i=0;i<mat.mat.size();++i) {
                for (auto& el : mat.mat[i]) {
                    elements[el.position][i] = MatrixElement<T, cmp>(i, el.element);
                }
            }
            for (int i=0;i<elements.size();++i) {
                for (auto& el : elements[i]) {
                    res.mat[i].push_back(el.second);
                }
            }

            return res;
        }

        friend bool operator == (const SparseMatrix<T, cmp, read>& mat1, const SparseMatrix<T, cmp, read>& mat2) {
            if (mat1.size() != mat2.size()) {
                return false;
            }
            for (int i=0;i<mat1.size();++i) {
                if (mat1.mat[i].size() != mat2.mat[i].size()) {
                    std::cout << "The two matrices have different number of elements on line " << i << '\n';
                    return false;
                }
                for (int j=0;j<mat1.mat[i].size();++j) {
                    if (mat1.mat[i][j] != mat2.mat[i][j]) {
                        std::cout << "The " << j << "'th elements on " << i << "'th line are different: \n";
                        std::cout << mat1.mat[i][j] << '\n';
                        std::cout << mat2.mat[i][j] << '\n';
                        return false;
                    }
                }
            }
            return true;
        }

        friend std::ostream& operator << (std::ostream& out, const SparseMatrix<T, cmp, read>& mat) {
            out << "[ \n";
            for (auto& line : mat.mat) {
                if (line.size() == 0) {
                    out << "\t[ ]\n";
                    continue;
                }
                out << "\t[";
                for (int i=0;i<((int)line.size())-1;++i) {
                    out << line[i] << ", ";
                }
                out << line.back() << "]\n";
            }
            out << "]\n";
            return out;
        }
};

template <typename T, bool (*cmp)(const T&, const T&), int (*read)(FILE*, T&, int&, int&)>
const T SparseMatrix<T, cmp, read>::zero = 0;

#endif
