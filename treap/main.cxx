#include "treap.hxx"

int main() {
    Treap treap{
        {15, 83},
        {7, 71},
        {19, 49},
        {2, 50},
        {10, 62},
        {24, 42},
        {9, 41},
        {12, 24},
        {22, 8}
    };
    treap.print();
}
