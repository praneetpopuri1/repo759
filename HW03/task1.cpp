#include <omp.h>

#include <cerrno>
#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <random>

#include "matmul.h"

using std::chrono::duration;
using std::chrono::high_resolution_clock;

namespace {

// Parses the whole argument as a positive decimal integer.
bool parse_positive(const char* text, std::size_t& value) {
    // strtoull would otherwise accept leading whitespace and a sign.
    if (text[0] < '0' || text[0] > '9') {
        return false;
    }
    char* end = nullptr;
    errno = 0;
    const unsigned long long parsed = std::strtoull(text, &end, 10);
    if (*end != '\0' || errno == ERANGE || parsed == 0) {
        return false;
    }
    value = static_cast<std::size_t>(parsed);
    return true;
}

}  // namespace

int main(int argc, char* argv[]) {
    std::size_t n = 0;
    std::size_t t = 0;
    if (argc != 3 || !parse_positive(argv[1], n) ||
        !parse_positive(argv[2], t) || t > 20) {
        std::cerr << "Usage: " << argv[0] << " n t  (n > 0, 1 <= t <= 20)\n";
        return 1;
    }
    omp_set_num_threads(static_cast<int>(t));

    std::random_device entropy_source;
    std::mt19937 generator(entropy_source());
    std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

    const std::size_t size = n * n;
    float* A = new float[size];
    float* B = new float[size];
    for (std::size_t i = 0; i < size; ++i) {
        A[i] = dist(generator);
        B[i] = dist(generator);
    }
    // Zero-initialize C so its first-touch page faults happen here instead of
    // being charged to the timed multiplication.
    float* C = new float[size]();

    const auto start = high_resolution_clock::now();
    mmul(A, B, C, n);
    const auto end = high_resolution_clock::now();
    const duration<double, std::milli> elapsed = end - start;

    std::cout << C[0] << '\n';
    std::cout << C[size - 1] << '\n';
    std::cout << elapsed.count() << '\n';

    delete[] A;
    delete[] B;
    delete[] C;
    return 0;
}
