#include "fractional_cascading.hxx"
#include <print>
using list = std::initializer_list<int>;

int main() {
    auto merged = build_merged(list{2, 8, 9, 14, 18}, list{3, 4, 5, 11, 12}, list{1, 2, 7, 10, 15, 16});
    for (auto el : merged) {
        std::print("{} ", el.value);
    }
    std::println();
}

