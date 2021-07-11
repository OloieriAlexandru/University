#ifndef TRIDIAGONAL_MATRIX_H
#define TRIDIAGONAL_MATRIX_H

#include <vector>
#include <ostream>
#include <cstdio>
#include <string>
#include <stdexcept>
#include <utility>

#define vv std::vector
#define mp std::make_pair
#define pair std::pair

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

extern std::string errorMessage;
extern void err(const std::string& message);

template <typename T, bool (*cmp)(const T&, const T&), int (*read)(FILE*, T&, int&, int&)>
class SparseMatrix;

template <typename T, bool (*cmp)(const T&, const T&), int (*read)(FILE*, T&)>
class TridiagonalMatrix {
    private:
        static const T zero;

        bool readArray(FILE *fileIn, int elCount, vv<T>& arr, std::string arrName) {
            int ret;
            T el;
            for (int i=1;i<=elCount;++i) {
                ret = read(fileIn, el);
                if (ret != 1) {
                    err("Invalid file! Failed to read the " + patch::to_string(i) + "'th element of the array \"" + arrName + "\"!"); 
                    return false;
                }
                arr.push_back(el);
            }
            return true;
        }

        void updateDiagonal(int& sz, vv<T>& arr, int positionsCount) {
            sz += positionsCount;
            for (int i=0;i<positionsCount;++i) {
                arr.pop_back();
            }
        }
    public:
        int n;
        int p;
        int q;

        vv<T> a;
        vv<T> b;
        vv<T> c;

        bool init(const char* fileName) {
            FILE *fileIn = fopen(fileName, "r");
            while (true) {
                if (!fileIn) {
                    err("Failed to open \"" + std::string(fileName) + "\" file!");
                    goto invalid_file;
                }

                int ret = fscanf(fileIn, "%d\n%d\n%d\n", &n, &p, &q);
                if (ret != 3) {
                    err("Invalid file format! (\"" + std::string(fileName) + "\") Expected three numbers on the first three lines!");
                    goto invalid_file;
                }

                if (!readArray(fileIn, n, a, "a") || !readArray(fileIn, n - q, c, "c") || !readArray(fileIn, n - p, b, "b") ) {
                    goto invalid_file;
                }

		fclose(fileIn);
                return true;
            }
            invalid_file:
            fclose(fileIn);
            return false;
        }

        vv<pair<int, T>> getElementsOnLine(int line, int& cIndex, int& bIndex) const {
            vv<pair<int, T>> tridiagonalElements;
            if (line - q >= 0) {
                tridiagonalElements.push_back(mp(line - q, c[cIndex++]));
            }
            tridiagonalElements.push_back(mp(line, a[line]));
            if (line + p < n) {
                tridiagonalElements.push_back(mp(line + p, b[bIndex++]));
            }
            return tridiagonalElements;
        }

        void getElementsOnLine(int line, int& cIndex, int& bIndex, pair<int, T> els[3], int& size) {
            size = 0;
            if (line - q >= 0) {
                els[size++] = c[cIndex++];
            }
            els[size++] = a[line];
            if (line + p < n) {
                els[size++] = b[bIndex++];
            }
        }

        friend TridiagonalMatrix<T, cmp, read> operator ~ (const TridiagonalMatrix<T, cmp, read>& mat) {
            TridiagonalMatrix<T, cmp, read> res;

            res.n = mat.n;
            res.p = mat.q;
            res.q = mat.p;

            res.a = mat.a;
            res.b = mat.c;
            res.c = mat.b;

            return res;
        }

        template <typename TSparse, bool (*cmpTSparse)(const TSparse&, const TSparse&), int (*readTSparse)(FILE*, TSparse&)>
        friend SparseMatrix<TSparse, cmpTSparse, readTSparse> operator * (const TridiagonalMatrix<T, cmp, read>& mat1, const TridiagonalMatrix<T, cmp, read>& mat2) {
            if (mat1.n != mat2.n) {
                throw std::invalid_argument("Can't multiply matrices (tridiagonal * tridiagonal) with different sizes! The sizes are: " + patch::to_string(mat1.n) + " and " +
                    patch::to_string(mat2.n) + "!");
            }
            SparseMatrix<TSparse, cmpTSparse, readTSparse> res;
            TridiagonalMatrix<T, cmp, read> t = ~mat2;

            int n;
            int m;
            pair<int, T> tridiagonalElements1[3];
            pair<int, T> tridiagonalElements2[3];

            res.resz(mat1.size());
            for (int i=0;i<mat1.size();++i) {
                int bIndex1 = 0, cIndex1 = 0;
                int bIndex2 = 0, cIndex2 = 0;
                for (int j=0;j<t.size();++j) {
                    getElementsOnLine(j, bIndex1, cIndex1, tridiagonalElements1, n);
                    getElementsOnLine(j, bIndex2, cIndex2, tridiagonalElements2, m);
                    
                    T el = 0;
                    memset(&el, 0, sizeof(T));
                    int jj1 = 0, jj2 = 0;

                    while (jj1 < n && jj2 < m) {
                        int idx1 = tridiagonalElements1[jj1].first;
                        int idx2 = tridiagonalElements1[jj2].first;
                        if (idx1 == idx2) {
                            el += tridiagonalElements1[jj1].second * tridiagonalElements2[jj2].second;
                            ++jj1, ++jj2;
                        } else if (idx1 < idx2) {
                            ++idx1;
                        } else {
                            ++idx2;
                        }
                    }

                    if (cmp(el, zero)) {
                        continue;
                    }
                    res.addElement(i, j, el);
                }
            }

            return res;
        }

        friend std::ostream& operator << (std::ostream& out, const TridiagonalMatrix<T, cmp, read>& mat) {
            out << "[ \n";
            int bIndex = 0, cIndex = 0;
            for (int i=0;i<mat.n;++i) {
                out << "\t[";
                for (int j=0;j<mat.n;++j) {
                    if (i == j) {
                        out << mat.a[i] << ", ";
                    } else if (j - i == mat.p) {
                        out << mat.b[bIndex++] << ", ";
                    } else if (i - j == mat.q) {
                        out << mat.c[cIndex++] << ", ";
                    } else {
                        out << "0, ";
                    }
                }
                out << "]\n";
            }
            return out;
        }

        int size() const {
            return n;
        }

        void moveB(int positionsCount) {
            updateDiagonal(p, b, positionsCount);
        }

        void moveC(int positionsCount) {
            updateDiagonal(q, c, positionsCount);
        }
};

template <typename T, bool (*cmp)(const T&, const T&), int (*read)(FILE*, T&)>
const T TridiagonalMatrix<T, cmp, read>::zero = 0;

#endif
