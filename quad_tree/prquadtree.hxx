#ifndef PRQUADTREE_HXX
#define PRQUADTREE_HXX

#include <array>
#include <format>
#include <variant>
#include <vector>

struct Point { 
    double x, y;
};

inline std::string to_string(Point p) {
    return std::format("({}, {})", p.x, p.y);
}

auto get_address(auto object) {
    return &object;
}

struct Rect {
    Point left_top, right_bot;
    std::array<Rect, 4> split() const;
    std::array<Point, 4> get_cornerns() const;
    bool contains(Point p) const;
    int get_index(Point p) const;
    Rect(Point lt, Point rb) : left_top{lt}, right_bot{rb} {}
    Rect(double x1, double y1, double x2, double y2) : Rect{{x1, y1}, {x2, y2}} {}
};

inline bool operator<=(const Rect& sub, const Rect& sup) {
    return (sup.left_top.x <= sub.left_top.x and sup.right_bot.x >= sub.right_bot.x) and
      (sup.left_top.y <= sub.left_top.y and sup.right_bot.y >= sub.right_bot.y);
}

inline bool operator>=(const Rect& sup, const Rect& sub) {
    return sub <= sup;
}

inline bool operator|(const Rect &lhs, const Rect &rhs) {
    // powered by ChatGPT
    if (lhs.right_bot.x < rhs.left_top.x || rhs.right_bot.x < lhs.left_top.x)
        return false;
    if (lhs.right_bot.y > rhs.left_top.y || rhs.right_bot.y > lhs.left_top.y)
        return false;
    return true;
}

struct Node {
    using LeafsT = std::array<Node*, 4>;
    std::variant<LeafsT, Point> children;
    Node* parent;
    std::string name{};

    Node(Node* parent, Point point) : parent{parent}, children{point} {}

    bool is_leaf() const { return std::holds_alternative<Point>(children); }

    void insert(Point p, const Rect& region);

    void search(const Rect &region, const Rect &q, std::vector<Node*>& out);

    std::vector<Point> collectPoints() const {
        CollectPointsVisitor visitor{};
        std::visit(visitor, this->children);
        return visitor.points;
    }

    std::vector<const Node*> collectLeafNodes() const {
        if (this->is_leaf()) {
            return {this};
        }
        CollectLeafNodesVisitor visitor{};
        std::visit(visitor, this->children);
        return visitor.leafs;
    }

    ~Node() {
        std::visit(DeleteVisitor{}, children);
    }

    void remove();
private:
    void flatten();

    struct DeleteVisitor {
        void operator()(LeafsT children) {
            for (auto *child : children) {
                delete child;
            }
        }
        void operator()(Point p) {}
    };
    struct IsHalfEmptyVisitor {
        Node* onlyChild = nullptr;
        int i = -1;
        bool operator()(LeafsT children) {
            for (auto *child : children) {
                i++;
                if (!child) {
                    continue;
                }
                if (onlyChild) {
                    return false;
                }
                onlyChild = child;
            }
            return true;
        }
        bool operator()(Point p) {
            return false;
        }
    };
    struct CollectLeafNodesVisitor {
        std::vector<const Node*> leafs;
        void operator()(LeafsT children) {
            for (auto *child : children) {
                if (!child) {
                    continue;
                }
                if (child->is_leaf()) {
                    leafs.push_back(child);
                }
                std::visit(*this, child->children);
            }
        }
        void operator()(Point p) { }
    };
    struct CollectPointsVisitor {
        std::vector<Point> points;
        void operator()(LeafsT children) {
            for (auto *child : children) {
                if (!child) {
                    continue;
                }
                std::visit(*this, child->children);
            }
        }
        void operator()(Point p) {
            points.push_back(p);
        }
    };

    static LeafsT get_nullptr_array() { return {}; }
};

class QuadTree {
    Rect region;

public:
    Node* root;
    QuadTree(auto&&... region) : region{region...}, root{nullptr} {}
    void insert(Point p);
    std::vector<Node*> search(const Rect& query);
    void remove(const Node *node);
    ~QuadTree() {
        delete root;
    }
};

#endif // PRQUADTREE_HXX
