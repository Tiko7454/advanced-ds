#include "interval_tree.hxx"

std::ostream& operator<<(std::ostream& os, Interval v) {
    return os << static_cast<std::string>(v);
}

IntervalTree& IntervalTree::insert(Interval v) {
    if (!root) {
        root = std::make_unique<Node>(v.get_pivot());
    }
    root->insert(v);
    return *this;
}

void IntervalTree::Node::insert(Interval v) {
    if (v < pivot) {
        if (!left) {
            left = new Node{v.get_pivot()};
        }
        return left->insert(v);
    }
    if (v > pivot) {
        if (!right) {
            right = new Node{v.get_pivot()};
        }
        return right->insert(v);
    }
    const auto [src, dest] = v;
    src2dest.emplace(src, dest);
    dest2src.emplace(dest, src);
}
