#include <omp.h>

#include <cerrno>
#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <random>

#include "msort.h"

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
    std::size_t ts = 0;
    if (argc != 4 || !parse_positive(argv[1], n) ||
        !parse_positive(argv[2], t) || t > 20 ||
        !parse_positive(argv[3], ts)) {
        std::cerr << "Usage: " << argv[0]
                  << " n t ts  (n > 0, 1 <= t <= 20, ts > 0)\n";
        return 1;
    }
    omp_set_num_threads(static_cast<int>(t));

    std::random_device entropy_source;
    std::mt19937 generator(entropy_source());
    std::uniform_int_distribution<int> dist(-1000, 1000);

    int* arr = new int[n];
    for (std::size_t i = 0; i < n; ++i) {
        arr[i] = dist(generator);
    }

    const auto start = high_resolution_clock::now();
    msort(arr, n, ts);
    const auto end = high_resolution_clock::now();
    const duration<double, std::milli> elapsed = end - start;

    std::cout << arr[0] << '\n';
    std::cout << arr[n - 1] << '\n';
    std::cout << elapsed.count() << '\n';

    delete[] arr;
    return 0;
}
