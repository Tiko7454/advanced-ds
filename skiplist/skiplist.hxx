#ifndef SKIPLIST_HXX
#define SKIPLIST_HXX

#include <cassert>
#include <cstddef>
class SkipList;
class SkipListIterator;

class Node {
    friend SkipList;
    friend SkipListIterator;
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

struct SkipListIterator {
    const Node* node;
    SkipListIterator operator++(int) {
        SkipListIterator tmp{node->nextElements[0]};
        node = node->nextElements[0];
        return tmp;
    }
    SkipListIterator& operator++() {
        node = node->nextElements[0];
        return *this;
    }
    const Node& operator*() { return *node; }
    const Node* operator->() { return node; }
    bool operator==(SkipListIterator& other) { return node == other.node; }
};

class SkipList {
    Node* header;
    Node* trailer;
    double probability;
public:
    SkipList(double probability = 0.5);
    SkipList& insert(int value);

    SkipListIterator begin() { return {header}; }
    SkipListIterator end() { return {trailer}; }
    void remove(int value);
    SkipListIterator find(int value) const;
    void resize_header_and_trailer(int height);
    void print() const;
    ~SkipList();

};

#endif
