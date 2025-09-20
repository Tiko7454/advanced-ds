#include "prquadtree.hxx"
#include <iostream>
#include <ranges>
#include <algorithm>

const auto map = std::views::transform;

void println(const std::string &x) {
    std::cerr << x << "\n";
}

int main() {
    QuadTree qt{0.0, 100.0, 100.0, 0.0};
    qt.insert({20, 10});
    qt.insert({75, 15});
    qt.insert({45, 90});
    qt.insert({65, 90});
    qt.insert({55, 52});
    qt.insert({60, 65});
    qt.insert({67, 70});
    qt.insert({70, 60});
    qt.insert({90, 70});
    for (auto& node : qt.search({{53, 71}, {76, 12}})) {
        std::ranges::for_each(node->collectPoints() | map(to_string), println);
    }
    std::cout << "-----------\n";
    for (auto& node : qt.search({{50, 60}, {60, 50}})) {
        std::ranges::for_each(node->collectPoints() | map(to_string), println);
    }
    std::cout << "-----------\n";
    auto *node = qt.search({{50, 60}, {60, 50}})[0];
    std::cout << "removing " << to_string(std::get<Point>(node->children)) << "\n";
    node->remove();
    for (auto& node : qt.search({{53, 71}, {76, 12}})) {
        std::ranges::for_each(node->collectPoints() | map(to_string), println);
    }

}
