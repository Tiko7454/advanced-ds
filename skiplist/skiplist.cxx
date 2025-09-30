#include "skiplist.hxx"
#include <algorithm>
#include <random>
#include <limits>
#include <iostream>
#include <print>

Node* Node::construct_node(int value, double probability) {
    static std::default_random_engine re{};
    std::uniform_real_distribution<double> dist(0, 1);
    int height = 1;
    for (; dist(re) < probability; height++);
    
    auto *node = construct_node_from_height(height);
    node->value = value;
    return node;
}

Node* Node::construct_node_from_height(int height) {
    auto *node = std::bit_cast<Node*>(new char[Node::size(height)]{});
    node->height = height;
    return node;
}

SkipList::SkipList(double probability) 
    : header{Node::construct_node_from_height(2)}
    , trailer{Node::construct_node_from_height(2)}
    , probability{probability} {
    header->value = std::numeric_limits<decltype(header->value)>::min();
    trailer->value = std::numeric_limits<decltype(trailer->value)>::max();
    header->nextElements[0] = trailer;
    header->nextElements[1] = trailer;
}

SkipList::~SkipList() {
    Node* old_node = header;
    Node* current_node = old_node->nextElements[0];
    while (old_node != trailer) {
        delete old_node;
        old_node = current_node;
        current_node = old_node->nextElements[0];
    }
    delete trailer;
}

void SkipList::remove(int value) {
    Node* node = nullptr;
    auto f = [&node](Node* current_node, int current_height){
        node = current_node->nextElements[current_height];
        current_node->nextElements[current_height] = node->nextElements[current_height];
        return false;
    };
    process(value, f);
    delete node;
}

void SkipList::process(int value, std::function<bool(Node*, int)> f) const {
    Node* current_node = header;
    int current_height = header->height - 1;
    while (current_height >= 0) {
        while (current_node->nextElements[current_height]->value < value) {
            current_node = current_node->nextElements[current_height];
        }
        if (current_node->nextElements[current_height]->value == value) {
            if (f(current_node, current_height)) {
                return;
            }
        }
        current_height--;
    }
}

SkipListIterator SkipList::find(int value) const {
    SkipListIterator si{end()};
    auto f = [&](Node* current_node, int current_height){
        si.node = current_node->nextElements[current_height];
        return true;
    };
    process(value, f);
    return si;
}

SkipList& SkipList::insert(int value) {
    Node* new_node = Node::construct_node(value, probability);
    resize_header_and_trailer(new_node->height + 1);

    Node* current_node = header;
    int current_height = header->height - 1;
    while (current_height >= 0) {
        while (current_node->nextElements[current_height]->value < value) {
            current_node = current_node->nextElements[current_height];
        }
        if (current_node->nextElements[current_height]->value == value) {
            delete new_node;
            return *this;
        }
        if (current_height < new_node->height) {
            new_node->nextElements[current_height] = current_node->nextElements[current_height];
            current_node->nextElements[current_height] = new_node;
        }
        current_height--;
    }
    return *this;
}

void SkipList::resize_header_and_trailer(int height) {
    if (header->height >= height) {
        return;
    }
    auto* new_header = Node::construct_node_from_height(height);
    new_header->value = header->value;
    std::copy(header->nextElements, header->nextElements + height, new_header->nextElements);
    for (int i = header->height; i < height; i++) {
        new_header->nextElements[i] = trailer;
    }
    delete header;
    header = new_header;
}

void SkipList::print() const {
    auto current = header->nextElements[0];
    while (current != trailer) {
        std::print("{} ({}), ", current->value, current->height);
        current = current->nextElements[0];
    }
    std::cout << std::endl;
}
