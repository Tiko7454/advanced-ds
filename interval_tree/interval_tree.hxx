#ifndef INTERVAL_TREE_HXX
#define INTERVAL_TREE_HXX

#include <format>
#include <map>
#include <memory>
#include <ranges>
#include <string>

struct Interval {
    double src, dest;
    double get_pivot() const {
        return (src + dest) / 2;
    }
    bool operator<(double x) const {
        return dest < x;
    }
    bool operator>(double x) const {
        return src > x;
    }
    operator std::string() const {
        return std::format("[{}, {}]", src, dest);
    }
};

std::ostream& operator<<(std::ostream& os, Interval v);

class IntervalTree {
private:
    enum class Direction { LEFT, RIGHT };
    struct Node {
        std::multimap<double, double> src2dest;
        std::multimap<double, double> dest2src;
        Node* left;
        Node* right;
        double pivot;
        Node(double pivot) : left{}, right{}, pivot{pivot} {}
        ~Node() { delete left; delete right; }
        void insert(Interval v);
        template<std::output_iterator<Interval> It> 
        void point_query(double x, It& gde) const;
        template<std::output_iterator<Interval> It> 
        void side_query(double x, Direction direction, It& gde) const;
    };
    std::unique_ptr<Node> root;
public:
    IntervalTree& insert(Interval v);
    IntervalTree& insert(double src, double dest) {
        return insert({src, dest});
    }
    template<std::output_iterator<Interval> It> 
    It point_query(double x, It gde) const {
        if (root) {
            root->point_query(x, gde);
        }
        return gde;
    }
    template<std::output_iterator<Interval> It> 
    It left_query(double x, It gde) const {
        if (root) {
            root->side_query(x, Direction::LEFT, gde);
        }
        return gde;
    }
};

template<std::output_iterator<Interval> It> 
void IntervalTree::Node::point_query(double x, It& gde) const {
    if (x < pivot) {
        for (const auto [src, dest] : src2dest) {
            if (x < src) {
                break;
            }
            *gde = {src, dest};
            gde++;
        }
        if (left) {
            left->point_query(x, gde);
        }
        return;
    }
    if (pivot < x) {
        for (const auto [dest, src] : dest2src | std::views::reverse) {
            if (dest < x) {
                break;
            }
            *gde = {src, dest};
            gde++;
        }
        if (right) {
            right->point_query(x, gde);
        }
        return;
    }
    for (const auto [src, dest] : src2dest) {
        *gde = {src, dest};
        gde++;
    }
}

template<std::output_iterator<Interval> It> 
void IntervalTree::Node::side_query(double x, Direction direction, It& gde) const {
    bool is_left = direction == Direction::LEFT;
    if (x < pivot) {
        for (const auto [src, dest] : src2dest) {
            if (x < src) {
                break;
            }
            *gde = {src, dest};
            gde++;
        }
        if (left) {
            left->point_query(x, gde);
        }
        return;
    }
    if (pivot < x) {
        for (const auto [dest, src] : dest2src | std::views::reverse) {
            if (dest < x) {
                break;
            }
            *gde = {src, dest};
            gde++;
        }
        if (right) {
            right->point_query(x, gde);
        }
        return;
    }
    for (const auto [src, dest] : src2dest) {
        *gde = {src, dest};
        gde++;
    }
}

#endif // INTERVAL_TREE_HXX
