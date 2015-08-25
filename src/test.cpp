#include "timing_utils.hpp"

#include <Eigen/Eigen>
#include <Eigen/Sparse>

#include <iostream>
#include <cstdlib>
#include <ctime>

typedef double Scalar;
typedef Eigen::Array<Scalar, Eigen::Dynamic, 1> V;
typedef Eigen::DiagonalMatrix<Scalar, Eigen::Dynamic> D;
typedef Eigen::SparseMatrix<Scalar> M;

#define COMPILER_BARRIER() asm volatile("" ::: "memory")

static const Scalar rand_max = static_cast<Scalar>(RAND_MAX);

extern "C" {
    int getValue(void* ptr);
}


inline V generateV(int nx) {
    V retval = V::Zero(nx);

    for (int i=0; i<nx; ++i) {
        retval[i] = rand() / rand_max;
    }

    return retval;
}

inline M generateM(int nx) {
    M retval(nx, nx);
    retval.reserve(Eigen::ArrayXi::Ones(nx, 1));

    for (M::Index i = 0; i < nx; ++i) {
        retval.insert(i, i) = rand() / rand_max;
    }

    return retval;
}

inline D generateD(int nx) {
    D retval = generateV(nx).matrix().asDiagonal();
    return retval;
}



template <typename T, typename U, typename V>
void benchmarkMult(T& a, U& b) {
    Scalar checksum = 0.0;
    long long i=0;

    time_point start = getCurrentTime();
    time_point end = start + boost::chrono::seconds(1);

    //while (end > getCurrentTime()) {
        for (int j=0; j<1000; ++j) {
            V c = a*b;
            checksum += (getValue(&a) + getValue(&b) + getValue(&c) > 0);
            ++i;
            COMPILER_BARRIER();
        }
    //}

    end = getCurrentTime();

    std::cout << "Duration:   " << toSeconds(end - start) << " s" << std::endl;
    std::cout << "Iterations: " << i << std::endl;
    std::cout << "Iters/sec:  " << i / toSeconds(end-start) << std::endl;

    if (checksum != i) {
        std::cout << "Checksum FAILED: " << checksum << "!=" << i << std::endl;
    }
}



int main(int, char**) {
    time_point start = getCurrentTime();

    srand(time(NULL));

    const int nx = 300000;
/*
    {
        std::cout << "=V*V===============================" << std::endl;
        V a = generateV(nx);
        V b = generateV(nx);
        benchmarkMult<V, V, V>(a, b);
        std::cout << "===================================" << std::endl << std::endl;
    }

    {
        std::cout << "=M*M===============================" << std::endl;
        M a = generateM(nx);
        M b = generateM(nx);
        benchmarkMult<M, M, M>(a, b);
        std::cout << "===================================" << std::endl << std::endl;
    }

    {
        std::cout << "=M*D===============================" << std::endl;
        M a = generateM(nx);
        D b = generateD(nx);
        benchmarkMult<M, D, M>(a, b);
        std::cout << "===================================" << std::endl << std::endl;
    }

    {
        std::cout << "=D*M===============================" << std::endl;
        D a = generateD(nx);
        M b = generateM(nx);
        benchmarkMult<D, M, M>(a, b);
        std::cout << "===================================" << std::endl << std::endl;
    }
*/
    {
        std::cout << "===D*M=============================" << std::endl;
        Scalar checksum = 0.0;
        long long i=0;
        D a = generateD(nx);
        M b = generateM(nx);

        time_point start = getCurrentTime();
        time_point end = start + boost::chrono::seconds(1);

        //while (end > getCurrentTime()) {
            M c;
            for (int j=0; j<1000; ++j) {
                c = a*b;
                checksum += (getValue(&a) + getValue(&b) + getValue(&c) > 0);
                ++i;
                COMPILER_BARRIER();
            }
        //}
        end = getCurrentTime();

        std::cout << "Duration:   " << toSeconds(end - start) << " s" << std::endl;
        std::cout << "Iterations: " << i << std::endl;
        std::cout << "Iters/sec:  " << i / toSeconds(end-start) << std::endl;

        if (checksum != i) {
            std::cout << "Checksum FAILED: " << checksum << "!=" << i << std::endl;
        }
    }

    {
        std::cout << "=M=D*M=============================" << std::endl;
        Scalar checksum = 0.0;
        long long i=0;
        D a = generateD(nx);
        M b = generateM(nx);

        time_point start = getCurrentTime();
        time_point end = start + boost::chrono::seconds(1);

        //while (end > getCurrentTime()) {
            for (int j=0; j<1000; ++j) {
                b = a*b;
                checksum += (getValue(&a) + getValue(&b) > 0);
                ++i;
                COMPILER_BARRIER();
            }
        //}
        end = getCurrentTime();

        std::cout << "Duration:   " << toSeconds(end - start) << " s" << std::endl;
        std::cout << "Iterations: " << i << std::endl;
        std::cout << "Iters/sec:  " << i / toSeconds(end-start) << std::endl;

        if (checksum != i) {
            std::cout << "Checksum FAILED: " << checksum << "!=" << i << std::endl;
        }
    }

    {
        std::cout << "=M=D*M=============================" << std::endl;
        Scalar checksum = 0.0;
        long long i=0;
        D a = generateD(nx);
        M b = generateM(nx);

        time_point start = getCurrentTime();
        time_point end = start + boost::chrono::seconds(1);

        //while (end > getCurrentTime()) {
            for (int j=0; j<1000; ++j) {
                M tmp = a*b;
                b = tmp;
                checksum += (getValue(&a) + getValue(&b) > 0);
                ++i;
                COMPILER_BARRIER();
            }
        //}
        end = getCurrentTime();

        std::cout << "Duration:   " << toSeconds(end - start) << " s" << std::endl;
        std::cout << "Iterations: " << i << std::endl;
        std::cout << "Iters/sec:  " << i / toSeconds(end-start) << std::endl;

        if (checksum != i) {
            std::cout << "Checksum FAILED: " << checksum << "!=" << i << std::endl;
        }
    }

    {
        std::cout << "=M=D*M=============================" << std::endl;
        Scalar checksum = 0.0;
        long long i=0;
        D a = generateD(nx);
        M b = generateM(nx);

        time_point start = getCurrentTime();
        time_point end = start + boost::chrono::seconds(1);

        //while (end > getCurrentTime()) {
            for (int j=0; j<1000; ++j) {
                M tmp = a*b;
                b.swap(tmp);
                checksum += (getValue(&a) + getValue(&b) > 0);
                ++i;
                COMPILER_BARRIER();
            }
        //}
        end = getCurrentTime();

        std::cout << "Duration:   " << toSeconds(end - start) << " s" << std::endl;
        std::cout << "Iterations: " << i << std::endl;
        std::cout << "Iters/sec:  " << i / toSeconds(end-start) << std::endl;

        if (checksum != i) {
            std::cout << "Checksum FAILED: " << checksum << "!=" << i << std::endl;
        }
    }

    {
        std::cout << "=M=D*M=============================" << std::endl;
        Scalar checksum = 0.0;
        long long i=0;
        D a = generateD(nx);
        M b = generateM(nx);

        time_point start = getCurrentTime();
        time_point end = start + boost::chrono::seconds(1);

        //while (end > getCurrentTime()) {
            for (int j=0; j<1000; ++j) {
                M tmp(a*b);
                b.swap(tmp);
                checksum += (getValue(&a) + getValue(&b) > 0);
                ++i;
                COMPILER_BARRIER();
            }
        //}
        end = getCurrentTime();

        std::cout << "Duration:   " << toSeconds(end - start) << " s" << std::endl;
        std::cout << "Iterations: " << i << std::endl;
        std::cout << "Iters/sec:  " << i / toSeconds(end-start) << std::endl;

        if (checksum != i) {
            std::cout << "Checksum FAILED: " << checksum << "!=" << i << std::endl;
        }
    }




    time_point end = getCurrentTime();

    std::cout << "Total time: " << toSeconds(end-start) << " s" << std::endl;
    return 0;
}
