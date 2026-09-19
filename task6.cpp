#include <charconv>
#include <cstdio>
#include <iostream>
#include <string_view>
#include <system_error>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " N\n";
        return 1;
    }

    const std::string_view argument(argv[1]);
    int n = 0;
    const auto result = std::from_chars(argument.data(),
                                        argument.data() + argument.size(), n);
    if (result.ec != std::errc{} ||
        result.ptr != argument.data() + argument.size() || n < 0) {
        std::cerr << "N must be a nonnegative integer in the int range.\n";
        return 1;
    }

    // Print the endpoint separately to avoid incrementing past INT_MAX.
    for (int i = 0; i < n; ++i) {
        std::printf("%d ", i);
    }
    std::printf("%d\n", n);

    for (int i = n; i > 0; --i) {
        std::cout << i << ' ';
    }
    std::cout << 0 << '\n';
    return 0;
}
