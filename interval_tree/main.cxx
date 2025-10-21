#include "interval_tree.hxx"
#include <iostream>
#include <iterator>

int main() {
    IntervalTree tree{};
    tree.insert(1, 7)
        .insert(8, 10)
        .insert(-9, -6)
        .insert(-7, -5)
        .insert(-6, -5)
        .insert(-8, -6.5)
        .insert(-10, 0);



    std::ostream_iterator<Interval> gde(std::cout, "\n");
    tree.point_query(-5, gde);
}
