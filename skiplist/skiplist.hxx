#ifndef SKIPLIST_HXX
#define SKIPLIST_HXX

#include <cassert>
#include <cstddef>
class SkipList;

class Node {
    friend SkipList;
    int value;
    int height;
    Node* nextElements[1];

    Node() = delete;
    static size_t size(size_t height) {
        assert(height != 0) ;
        return sizeof(Node) + (height - 1) * sizeof(*nextElements);
    }
    static Node* construct_node(int value, double probability);
    static Node* construct_node_from_height(int height);
};

class SkipList {
    Node* header;
    Node* trailer;
    double probability;
public:
    SkipList(double probability = 0.5);
    SkipList& insert(int value);
    void resize_header_and_trailer(int height);
    void print() const;
    ~SkipList() {
        delete header;
        // TODO...
    }

};

#endif
