#ifndef SPANTREE_H
#define SPANTREE_H

#include <memory>
#include <string>
#include <iterator>

struct Interval {
    double a, b;

    bool operator<(Interval i) const { return a < i.a or a == i.a and b < i.b; }
    bool operator==(Interval i) const { return a == i.a and b == i.b; }

    std::string to_string() const { return std::format("[{}, {}]", a, b); }
    std::string to_line_string() const {
        return std::string(static_cast<unsigned>(a), ' ') +
               std::string(static_cast<unsigned>(b - a), '-');
    }
};

struct Node {
    Interval value;
    Node* left;
    Node* right;
    Node* parent;
    double span;

    Node* insert(Interval v);
    const Node* search(double q) const;

    std::string print() const;
    std::string to_string() const {
        return "\"" + value.to_string() + "(" + std::to_string(span) + ")\"";
    }
    void print_lines() const;

    Node(Node* parent, Interval value)
        : left{}, right{}, parent{parent}, value{value}, span{value.b} {}

    ~Node() {
        delete left;
        delete right;
    }

    void update();
    bool contains(double x) const { return value.a <= x && x <= value.b; }
    template<std::output_iterator<Point> It>
    void query(It gde);
};

class Spantree {
    std::unique_ptr<Node> root;

  public:
    Spantree(double a, double b) : Spantree{{a, b}} {}
    Spantree(std::initializer_list<Interval> il);
    void insert(Interval value) { root->insert(value); }
    const Node* find(int value) const;
    void print() const;
    void print_lines() const { root->print_lines(); }
    const Node* search(double q) const { return root->search(q); }
    template<std::output_iterator<Point> It>
    void query(It gde)
};

#endif // SPANTREE_H
