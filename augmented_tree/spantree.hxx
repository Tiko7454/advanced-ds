#ifndef SPANTREE_H
#define SPANTREE_H

#include <memory>
#include <string>
#include <format>

struct Interval {
    double a, b;

    bool operator<(Interval i) const { return a < i.a or a == i.a and b < i.b; }
    bool operator==(Interval i) const { return a == i.a and b == i.b; }

    std::string to_string() const { return std::format("[{}, {}]", a, b); }
};

struct Node {
    Interval value;
    Node* left;
    Node* right;
    Node* parent;
    double span;

    Node* insert(Interval v);

    std::string print() const;
    std::string to_string() const {
        return "\"" + value.to_string() + "(" + std::to_string(span) + ")\"";
    }

    Node(Node* parent, Interval value)
        : left{}, right{}, parent{parent}, value{value}, span{value.b} {
        if (parent) {
            parent->update();
        }
    }

    ~Node() {
        delete left;
        delete right;
    }

    void update();
};

class Spantree {
    std::unique_ptr<Node> root;

  public:
    Spantree(double a, double b) : Spantree{{a, b}} {}
    Spantree(Interval value) : root{std::make_unique<Node>(nullptr, value)} {}
    void insert(Interval value) { root->insert(value); }
    const Node* find(int value) const;
    void print() const;
};

#endif // SPANTREE_H
