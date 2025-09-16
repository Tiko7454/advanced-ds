#ifndef PRQUADTREE_HXX
#define PRQUADTREE_HXX

#include <array>
#include <memory>
#include <variant>
struct Point { double x, y; };

struct Rect {
    Point left_top, right_bot;
    std::array<Rect, 4> split() const;
    bool contains(Point p) const;
    int get_index(Point p) const;
    Rect(Point lt, Point rb) : left_top{lt}, right_bot{rb} {}
    Rect(double x1, double y1, double x2, double y2) : Rect{{x1, y1}, {x2, y2}} {}
};

struct Node {
    using LeafsT = std::array<Node*, 4>;
    std::variant<LeafsT, Point> children;
    Node* parent;

    Node(Node* parent, Point point) : parent{parent}, children{point} {}

    bool is_leaf() const { return std::holds_alternative<Point>(children); }

    void insert(Point p, const Rect& region);
    
    ~Node() {
        std::visit(DeleteVisitor{}, children);
    }
private:
    struct DeleteVisitor {
        void operator()(LeafsT children) {
            for (auto *child : children) {
                delete child;
            }
        }
        void operator()(Point p) {}
    };
    static LeafsT get_nullptr_array() {
        return {};
    }
};

class QuadTree {
    Rect region;
    std::unique_ptr<Node> root;

    QuadTree(const Rect& region, Point point) : region{region}, root{std::make_unique<Node>(nullptr, point)} {}
    void insert(Point p) { root->insert(p, region); }
};

#endif // PRQUADTREE_HXX
