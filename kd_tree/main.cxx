#include "kd_tree.hxx"
#include <print>

int main() {
    Tree<2> t;
    t.insert({2,2});
    t.insert({3,3});
    t.insert({1,1});
    t.insert({4,4});
    t.insert({5,5});
    std::println("{}", t.search({3.4, 2.5})->elems);
}
