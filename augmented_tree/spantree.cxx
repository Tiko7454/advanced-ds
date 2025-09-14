#include "spantree.hxx"
#include <fstream>
#include <iostream>

Node* Node::insert(Interval v) {
    if (v < value) {
        if (!left) {
            left = new Node{this, v};
            update();
            return left;
        }
        return left->insert(v);
    }
    if (value < v) {
        if (!right) {
            right = new Node{this, v};
            update();
            return right;
        }
        return right->insert(v);
    }
    return this;
}

void Node::update() {
    auto old_span = span;
    span = value.b;
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
        dot += to_string() + " -> " + left->to_string() + " [color=blue];\n";
    }
    if (right) {
        dot += right->print();
        dot += to_string() + " -> " + right->to_string() + " [color=green];\n";
    }
    if (left and right) {
        dot += "{rank=same; " + left->to_string() + "; " + right->to_string() + ";}\n";
        dot += left->to_string() + " -> " + right->to_string() + "[style=invis];\n";
    }
    return dot;
}

void Spantree::print() const {
    std::ofstream t("tree.dot");
    t << "digraph G{\n";
    t << "rankdir=TB;\n";
    t << root->print();
    t << "}\n";
}

Spantree::Spantree(std::initializer_list<Interval> il) {
    if (il.size() == 0) {
        throw std::runtime_error("spantree cannot be made from empty");
    }
    root = std::make_unique<Node>(nullptr, *il.begin());
    for (auto&& el: il) {
        insert(el);
    }
}

const Node* Node::search(double q) const {
    if (contains(q)) {
        return this;
    }
    if (left and q < left->span) {
        return left->search(q);
    }
    if (right and q > value.a) {
        return right->search(q);
    }
    return nullptr;
}

void Node::print_lines() const {
    std::cout << value.to_line_string() << "\n";
    if (left) {
        left->print_lines();
    }
    if (right) {
        right->print_lines();
    }
}
