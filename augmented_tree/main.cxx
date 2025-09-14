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
    spantree.print();
}
