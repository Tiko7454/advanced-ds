#ifndef TREAP_HXX
#include <initializer_list>
#include <format>
#include <string>

struct Point {
    int x;
    int y;
    std::string to_string() const { return std::format("[{}, {}]", x, y); }
};

struct Node {
    Node(int x, int y, Node* parent = nullptr);
    explicit Node(Point p, Node* parent = nullptr);
    Point point;
    Node* parent;
    Node* left = nullptr;
    Node* right = nullptr;
    void operator<<(Node* child);
    void operator>>(Node* child);
    ~Node();
    std::string print() const;
    std::string to_string() const {
        return std::format("\"{}\"", point.to_string());
    }
    template<std::output_iterator<Point> It>
    void query(int x_min, int x_max, int y_min, It& gde) const;
};

class Treap {
    Node* root;
    public:
    Treap(std::initializer_list<Point> points);
    ~Treap() { delete root; }
    void print() const;
    template<std::output_iterator<Point> It>
    It query(int x_min, int x_max, int y_min, It gde) const {
        if (root) {
            root->query(x_min, x_max, y_min, gde);
        }
        return gde;
    }
};

template<std::output_iterator<Point> It>
void Node::query(int x_min, int x_max, int y_min, It& gde) const {

}

#endif // TREAP_HXX
