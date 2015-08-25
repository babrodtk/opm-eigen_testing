/**
 * Example showing performance of Eigen for sparse/diagonal matrix operations
 * Compile with
 * g++-4.9 -O3 -DNDEBUG -fPIC -I<eigen-path> -std=c++11 -o eigen_slow_sparse <this-file>
 */


#include <Eigen/Eigen>
#include <Eigen/Sparse>

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>

typedef double Scalar;
typedef Eigen::Array<Scalar, Eigen::Dynamic, 1> Array;
typedef Eigen::DiagonalMatrix<Scalar, Eigen::Dynamic> DiagonalMatrix;
typedef Eigen::SparseMatrix<Scalar> SparseMatrix;

typedef std::chrono::high_resolution_clock::time_point time_point;
typedef std::chrono::high_resolution_clock::duration duration;

#define COMPILER_BARRIER() asm volatile("" ::: "memory")

static const Scalar rand_max = static_cast<Scalar>(RAND_MAX);


inline time_point getCurrentTime() {
    return std::chrono::high_resolution_clock::now();
}

unsigned long long toMicroSeconds(const duration& tp) {
    return std::chrono::duration_cast<std::chrono::microseconds>(tp).count();
}

double toSeconds(const duration& tp) {
    return toMicroSeconds(tp)/1000000.0;
}

inline SparseMatrix generateSparseMatrix(int nx) {
    SparseMatrix retval(nx, nx);
    retval.reserve(Eigen::ArrayXi::Ones(nx, 1));

    for (SparseMatrix::Index i = 0; i < nx; ++i) {
        retval.insert(i, i) = rand() / rand_max;
    }

    return retval;
}

inline DiagonalMatrix generateDiagonalMatrix(int nx) {

    Array vals = Array::Zero(nx);

    for (int i=0; i<nx; ++i) {
        vals[i] = rand() / rand_max;
    }

    return vals.matrix().asDiagonal();
}

int main(int, char**) {
        const int nx = 300000;

    {
        DiagonalMatrix a = generateDiagonalMatrix(nx);
        SparseMatrix b = generateSparseMatrix(nx);

        time_point start = getCurrentTime();
        for (int j=0; j<1000; ++j) {
            SparseMatrix c = a*b;
            COMPILER_BARRIER();
        }
        time_point end = getCurrentTime();

        std::cout << "SparseMatrix c = a*b:   " << toSeconds(end - start) << " s" << std::endl;
    }


    {
        DiagonalMatrix a = generateDiagonalMatrix(nx);
        SparseMatrix b = generateSparseMatrix(nx);
        SparseMatrix c;

        time_point start = getCurrentTime();
        for (int j=0; j<1000; ++j) {
            c = a*b;
            COMPILER_BARRIER();
        }
        time_point end = getCurrentTime();

        std::cout << "c = a*b:   " << toSeconds(end - start) << " s" << std::endl;
    }


    {
        DiagonalMatrix a = generateDiagonalMatrix(nx);
        SparseMatrix b = generateSparseMatrix(nx);

        time_point start = getCurrentTime();
        for (int j=0; j<1000; ++j) {
            b = a*b;
            COMPILER_BARRIER();
        }
        time_point end = getCurrentTime();

        std::cout << "b = a*b:   " << toSeconds(end - start) << " s" << std::endl;
    }


    {
        DiagonalMatrix a = generateDiagonalMatrix(nx);
        SparseMatrix b = generateSparseMatrix(nx);
        SparseMatrix tmp;

        time_point start = getCurrentTime();
        for (int j=0; j<1000; ++j) {
            tmp = a*b;
            b = tmp;
            COMPILER_BARRIER();
        }
        time_point end = getCurrentTime();

        std::cout << "tmp = a*b; b=tmp:   " << toSeconds(end - start) << " s" << std::endl;
    }


    {
        DiagonalMatrix a = generateDiagonalMatrix(nx);
        SparseMatrix b = generateSparseMatrix(nx);
        SparseMatrix tmp;

        time_point start = getCurrentTime();
        for (int j=0; j<1000; ++j) {
            tmp = a*b;
            b.swap(tmp);
            COMPILER_BARRIER();
        }
        time_point end = getCurrentTime();

        std::cout << "tmp = a*b; b.swap(tmp):   " << toSeconds(end - start) << " s" << std::endl;
    }


    {
        SparseMatrix a = generateSparseMatrix(nx);
        SparseMatrix b = generateSparseMatrix(nx);
        SparseMatrix c;

        time_point start = getCurrentTime();
        for (int j=0; j<1000; ++j) {
            c = a*b;
            COMPILER_BARRIER();
        }
        time_point end = getCurrentTime();

        std::cout << "c = a*b:   " << toSeconds(end - start) << " s" << std::endl;
    }
}
