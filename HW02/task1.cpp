#include <cerrno>
#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <random>

#include "scan.h"

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
    if (argc != 2 || !parse_positive(argv[1], n)) {
        std::cerr << "Usage: " << argv[0] << " n  (n is a positive integer)\n";
        return 1;
    }

    std::random_device entropy_source;
    std::mt19937 generator(entropy_source());
    std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

    float* arr = new float[n];
    for (std::size_t i = 0; i < n; ++i) {
        arr[i] = dist(generator);
    }
    // Zero-initialize the output so its first-touch page faults happen here
    // instead of being charged to the timed scan.
    float* output = new float[n]();

    const auto start = high_resolution_clock::now();
    scan(arr, output, n);
    const auto end = high_resolution_clock::now();
    const duration<double, std::milli> elapsed = end - start;

    std::cout << elapsed.count() << '\n';
    std::cout << output[0] << '\n';
    std::cout << output[n - 1] << '\n';

    delete[] arr;
    delete[] output;
    return 0;
}
