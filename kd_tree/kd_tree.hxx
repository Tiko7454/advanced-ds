#ifndef KD_TREE
#define KD_TREE

#include <array>
#include <cstddef>
#include <memory>
#include <ranges>
#include <numeric>
#include <functional>

template<size_t N>
struct Point {
    std::array<double, N> elems;
    double operator[](size_t i) const {
        return elems[i];
    }
    double operator^(const Point<N>& other) const {
        auto a = std::views::zip(elems, other.elems) 
            | std::views::transform([](auto x){ auto [a,b] = x; return (a-b)*(a-b); });
        return std::accumulate(a.begin(), a.end(), 0, std::plus<double>{});
    }
};

template<size_t N>
struct Dimention {
    size_t n;
    Dimention<N> operator++() {
        n++;
        if (n == N) {
            return {0};
        }
        return {n};
    }
};

template<size_t N>
class Tree {
public:
    void insert(const Point<N> &value) { 
        if (!root) {
            root = std::make_unique<Node>(value);
            return;
        }
        root->insert(value); 
    }
    std::optional<Point<N>> search(const Point<N> &value) const {
        if (!root) {
            return {};
        }
        return std::optional{Visitor(value).visit(*root.get()).get()};
    }

private:
    struct Node {
        Point<N> value;
        const Dimention<N> dim;
        Node* left;
        Node* right;

        explicit Node(const Point<N> &value, Dimention<N> parent_dimention = {0}) 
            : value{value}
            , dim{++parent_dimention} {}
        ~Node() { delete left; delete right; }

        void insert(const Point<N> &current);
    };
    std::unique_ptr<Node> root;
    struct Visitor {
        Visitor(const Point<N>& search_point) 
            : search_point{search_point}
            , found_point{nullptr}
            , current_min{std::numeric_limits<double>::max()} {}
        const Point<N>& search_point;
        const Point<N>* found_point;
        double current_min;

        const Visitor& visit(const Node& current);
        const Point<N>& get() const { return *found_point; }
    };
};


template<size_t N>
void Tree<N>::Node::insert(const Point<N> &current) {
    if(value[dim.n] <= current[dim.n]) {
        if (!right) {
            right = new Node{current, dim};
            return;
        }
        right->insert(current);
    } else {
        if (!left) {
            left = new Node{current, dim};
            return;
        }
        left->insert(current);
    }
}

template<size_t N>
const Tree<N>::Visitor& Tree<N>::Visitor::visit(const Node& current) {
    auto current_dist = current.value ^ search_point;
    if (current_min > current_dist) {
        current_min = current_dist;
        found_point = &current.value;
    }
    if(search_point[current.dim.n] <= current.value[current.dim.n]) {
        if (current.left) {
            visit(*current.left);
        }
        if (current.right) {
            if (current_min >= (current.value[current.dim.n] - search_point[current.dim.n])) {
                visit(*current.right);
            }
        }
    } else {
        if (current.right) {
            visit(*current.right);
        }
        if (current.left) {
            if (current_min >= (search_point[current.dim.n] - current.value[current.dim.n])) {
                visit(*current.left);
            }
        }
    }

    return *this;
}

#endif // KD_TREE
