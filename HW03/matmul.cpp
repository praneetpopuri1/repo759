#include "matmul.h"

#include <cstddef>

// Parallel version of mmul2 from HW02: the i-k-j loop order keeps the
// innermost accesses to B and C contiguous. Each thread owns whole rows of C,
// so no two threads ever write the same element.
void mmul(const float* A, const float* B, float* C, const std::size_t n) {
#pragma omp parallel for schedule(static)
    for (std::size_t i = 0; i < n; ++i) {
        float* C_row = C + i * n;
        for (std::size_t j = 0; j < n; ++j) {
            C_row[j] = 0.0f;
        }
        for (std::size_t k = 0; k < n; ++k) {
            const float a = A[i * n + k];
            const float* B_row = B + k * n;
            for (std::size_t j = 0; j < n; ++j) {
                C_row[j] += a * B_row[j];
            }
        }
    }
}
