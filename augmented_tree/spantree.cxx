#include "spantree.hxx"
#include <fstream>

Node* Node::insert(Interval v) {
    if (value < v) {
        if (!left) {
            left = new Node{this, v};
            return left;
        }
        return left->insert(v);
    }
    if (v < value) {
        if (!right) {
            right = new Node{this, v};
            return right;
        }
        return right->insert(v);
    }
    return this;
}

void Node::update() {
    auto old_span = span;
    if (left) {
        span = std::max(span, left->span);
    }
    if (right) {
        span = std::max(span, right->span);
    }
        if (parent) {
            parent->update();
    }
}

std::string Node::print() const {
    std::string dot;
    dot += to_string() + ";\n";
    if (left) {
        dot += left->print();
        dot +=  to_string() + " -> " + left->to_string() + ";\n";
    }
    if (right) {
        dot += right->print();
        dot +=  to_string() + " -> " + right->to_string() + ";\n";
    }
    return dot;
}

void Spantree::print() const {
    std::ofstream t("tree.dot");
    t << "digraph G{\n";
    t << root->print();
    t << "}\n";
}
