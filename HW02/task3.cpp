#include <chrono>
#include <cstddef>
#include <iostream>
#include <random>
#include <vector>

#include "matmul.h"

using std::chrono::duration;
using std::chrono::high_resolution_clock;

namespace {

// Times one call of multiply, then prints the elapsed time in milliseconds
// and the last element of the product C.
template <typename Multiply>
void time_and_report(Multiply multiply, const double* C, std::size_t size) {
    const auto start = high_resolution_clock::now();
    multiply();
    const auto end = high_resolution_clock::now();
    const duration<double, std::milli> elapsed = end - start;

    std::cout << elapsed.count() << '\n';
    std::cout << C[size - 1] << '\n';
}

}  // namespace

int main() {
    const unsigned int n = 1024;
    const std::size_t size = static_cast<std::size_t>(n) * n;

    std::random_device entropy_source;
    std::mt19937 generator(entropy_source());
    std::uniform_real_distribution<double> dist(-1.0, 1.0);

    double* A = new double[size];
    double* B = new double[size];
    for (std::size_t i = 0; i < size; ++i) {
        A[i] = dist(generator);
        B[i] = dist(generator);
    }
    // mmul4 takes its inputs as vectors holding the same values.
    const std::vector<double> A_vec(A, A + size);
    const std::vector<double> B_vec(B, B + size);
    double* C = new double[size]();

    std::cout << n << '\n';
    time_and_report([&] { mmul1(A, B, C, n); }, C, size);
    time_and_report([&] { mmul2(A, B, C, n); }, C, size);
    time_and_report([&] { mmul3(A, B, C, n); }, C, size);
    time_and_report([&] { mmul4(A_vec, B_vec, C, n); }, C, size);

    delete[] A;
    delete[] B;
    delete[] C;
    return 0;
}
