#include "treap.hxx"
#include <deque>
#include <vector>
#include <algorithm>
#include <ranges>
#include <cassert>
#include <fstream>

Node::~Node() {
    delete left;
    delete right;
}

Node::Node(int x, int y, Node* parent) : Node{{x, y}, parent} { }
Node::Node(Point p, Node* parent) : point{p}, parent{parent} { }

Treap::Treap(std::initializer_list<Point> points) {
    auto a = points | std::views::transform(
        [](Point p){
            return new Node{p};
        }
    );
    std::vector<Node*> nodes(a.begin(), a.end());

    std::ranges::sort(nodes, [](Node* lhs, Node* rhs){return lhs->point.x < rhs->point.x; });

    std::deque<Node*> stack;
    Node* last = nullptr;
    for (auto *node : nodes) {
        while (!stack.empty()) {
            auto top = stack.back();
            if (top->point.y < node->point.y) {
                last = top;
                stack.pop_back();
                continue;
            }
            break;
        }
        if (stack.empty()) {
            stack.push_back(node);
            if (last) {
                *node << last;
            }
            continue;
        }
        auto top = stack.back();
        assert(top->point.y > node->point.y && "shat vata");
        if (top->right) {
            auto rest = top->right;
            *top >> node;
            *node << rest;
        } else {
            *top >> node;
        }
        stack.push_back(node);
    }
    assert(!stack.empty() && "shat vata 2");
    root = *stack.begin();
}

void Node::operator<<(Node* child) {
    left = child;
    child->parent = this;
}

void Node::operator>>(Node* child) {
    right = child;
    child->parent = this;
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

void Treap::print() const {
    std::ofstream t("tree.dot");
    t << "digraph G{\n";
    t << "rankdir=TB;\n";
    t << root->print();
    t << "}\n";
}

