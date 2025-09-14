#include <iostream>
#include "spantree.hxx"
int main() {
    Spantree spantree{
        {12, 18},
        {5, 22},
        {16, 18},
        {22,35},
        {24,29},
        {13, 16},
        {9, 19},
        {2,6},
        {15,17},
        {4,5},
        {8,31},
    };
    std::cout << "v----v----v----v----v----v----v----v----v\n";
    spantree.print_lines();
    std::cout << "0----.----1----.----2----.----3----.----4\n";
    spantree.print();
    while (true) {
        std::cout << "q = ";
        double d;
        std::cin >> d;
        const auto *node = spantree.search(d);
        if (node) {
            std::cout << node->value.to_string() << "\n";
        } else {
            std::cout << "not found\n";
        }
    }
}
