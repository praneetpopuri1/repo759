#include "convolution.h"

#include <cstddef>

namespace {

// Value of f[i, j] for the padded image: the image itself inside the n x n
// grid, 1 along the edges outside it, and 0 at the corners outside it.
float padded_pixel(const float* image, std::size_t n, std::ptrdiff_t i,
                   std::ptrdiff_t j) {
    const std::ptrdiff_t size = static_cast<std::ptrdiff_t>(n);
    const bool row_inside = 0 <= i && i < size;
    const bool col_inside = 0 <= j && j < size;
    if (row_inside && col_inside) {
        return image[i * size + j];
    }
    return (row_inside || col_inside) ? 1.0f : 0.0f;
}

}  // namespace

void convolve(const float* image, float* output, std::size_t n,
              const float* mask, std::size_t m) {
    const std::ptrdiff_t offset = static_cast<std::ptrdiff_t>((m - 1) / 2);

    for (std::size_t x = 0; x < n; ++x) {
        for (std::size_t y = 0; y < n; ++y) {
            float sum = 0.0f;
            for (std::size_t i = 0; i < m; ++i) {
                const std::ptrdiff_t row =
                    static_cast<std::ptrdiff_t>(x + i) - offset;
                for (std::size_t j = 0; j < m; ++j) {
                    const std::ptrdiff_t col =
                        static_cast<std::ptrdiff_t>(y + j) - offset;
                    sum += mask[i * m + j] * padded_pixel(image, n, row, col);
                }
            }
            output[x * n + y] = sum;
        }
    }
}
