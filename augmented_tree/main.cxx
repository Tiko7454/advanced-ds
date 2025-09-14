#include <iostream>
#include "spantree.h"
int main() {
    Spantree tree{1, 5};
    tree.insert({2,3});
    tree.insert({0,12});
    tree.insert({-4,13});
    tree.print();
}
