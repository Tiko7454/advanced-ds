#include <bk_tree.hxx>
#include <iostream>
#include <iterator>

int main() {
    BKTree tree{hamming_distance};
    tree.insert("nest")
        .insert("best")
        .insert("neck")
        .insert("guess")
        .insert("back")
        .insert("west")
        .insert("mess")
        .insert("guest")
        .insert("bed")
        .insert("book")
        .insert("break")
        .insert("not")
        .insert("must")
        .insert("kick");
    tree.search("lest", 2, std::ostream_iterator<std::string>{std::cout, "\n"});
}
