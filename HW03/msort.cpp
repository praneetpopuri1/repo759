#include "msort.h"

#include <cstddef>

namespace {

// Serial, non-recursive sort used once a subarray is below the threshold.
void insertion_sort(int* arr, std::size_t n) {
    for (std::size_t i = 1; i < n; ++i) {
        const int value = arr[i];
        std::size_t j = i;
        while (j > 0 && arr[j - 1] > value) {
            arr[j] = arr[j - 1];
            --j;
        }
        arr[j] = value;
    }
}

// Merges the sorted halves arr[0, mid) and arr[mid, n) through scratch.
void merge(int* arr, int* scratch, std::size_t mid, std::size_t n) {
    std::size_t left = 0;
    std::size_t right = mid;
    std::size_t out = 0;
    while (left < mid && right < n) {
        scratch[out++] = (arr[right] < arr[left]) ? arr[right++] : arr[left++];
    }
    while (left < mid) {
        scratch[out++] = arr[left++];
    }
    while (right < n) {
        scratch[out++] = arr[right++];
    }
    for (std::size_t i = 0; i < n; ++i) {
        arr[i] = scratch[i];
    }
}

// Sorts arr[0, n) using scratch[0, n) as merge space. The left half is
// handed to another thread as a task while this thread sorts the right half.
void merge_sort(int* arr, int* scratch, std::size_t n, std::size_t threshold) {
    if (n < 2) {
        return;
    }
    if (n < threshold) {
        insertion_sort(arr, n);
        return;
    }
    const std::size_t mid = n / 2;
#pragma omp task firstprivate(arr, scratch, mid, threshold)
    merge_sort(arr, scratch, mid, threshold);
    merge_sort(arr + mid, scratch + mid, n - mid, threshold);
#pragma omp taskwait
    merge(arr, scratch, mid, n);
}

}  // namespace

void msort(int* arr, const std::size_t n, const std::size_t threshold) {
    int* scratch = new int[n];
#pragma omp parallel
#pragma omp single
    merge_sort(arr, scratch, n, threshold);
    delete[] scratch;
}
