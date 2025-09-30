#include "fenwick_tree.hxx"
#include <print>

int main() {
    FenwickTree ft(sum, {1, 2, 3, 5, 6, 7, 9});
    ft.print();
    std::println("0 2 = {}", ft.query(0, 2));
    std::println("1 5 = {}", ft.query(1, 5));
    std::println("0 7 = {}", ft.query(0, 7));
    std::println("0 6 = {}", ft.query(0, 6));
    // std::println("{}", ft);
}
