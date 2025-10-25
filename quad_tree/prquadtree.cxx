#include "prquadtree.hxx"
#include <utility>
#include <cassert>

std::array<Rect, 4> Rect::split() const {
    const auto mid_x = (left_top.x + right_bot.x) / 2;
    const auto mid_y = (left_top.y + right_bot.y) / 2;
    return {
        Rect{{mid_x, left_top.y}, {right_bot.x, mid_y}},
        Rect{left_top, {mid_x, mid_y}},
        Rect{{left_top.x, mid_y}, {mid_x, right_bot.y}},
        Rect{{mid_x, mid_y}, right_bot},
    };
}

std::array<Point, 4> Rect::get_cornerns() const {
    return {
        Point{right_bot.x, left_top.y},
        Point{left_top},
        Point{left_top.x, right_bot.y},
        Point{right_bot},
    };
}

bool Rect::contains(Point p) const {
    return left_top.x <= p.x and p.x <= right_bot.x and right_bot.y <= p.y and p.y <= left_top.y;
}

int Rect::get_index(Point p) const {
    const auto mid_x = (left_top.x + right_bot.x) / 2;
    const auto mid_y = (left_top.y + right_bot.y) / 2;
    if (p.x >= mid_x and p.y >= mid_y) {
        return 0;
    }
    if (p.x <= mid_x and p.y >= mid_y) {
        return 1;
    }
    if (p.x <= mid_x and p.y <= mid_y) {
        return 2;
    }
    if (p.x >= mid_x and p.y <= mid_y) {
        return 3;
    }
    std::unreachable();
}

void QuadTree::insert(Point p) { 
    if (!region.contains(p)) {
        return;
    } 
    if (!root) {
        root = new Node{nullptr, p};
        return;
    }
    root->insert(p, region);
}

void Node::insert(Point p, const Rect& region) {
    const auto splitted = region.split();
    if (is_leaf()) {
        const auto old = std::get<Point>(children);
        auto new_children = get_nullptr_array();
        const auto i = region.get_index(old);
        auto* new_node = new Node{this, old};
        new_children[i] = new_node;
        children = new_children;
    }
    const auto i = region.get_index(p);
    auto& subtree = std::get<LeafsT>(children);
    if (subtree[i] == nullptr) {
        subtree[i] = new Node{this, p};
        return;
    }
    subtree[i]->insert(p, region.split()[i]);
}

std::vector<Node*> QuadTree::search(const Rect& query) {
    if (!root) {
        return {};
    }
    std::vector<Node*> points;
    root->search(region, query, points);
    return points;
}

void Node::search(const Rect& region, const Rect& q, std::vector<Node*>& out) {
    if (this->is_leaf()) {
        const auto point = std::get<Point>(children);
        if (q.contains(point)) {
            out.push_back(this);
        }
        return;
    }
    if (region <= q) {
        out.push_back(this);
        return;
    }
    const auto& subtree = std::get<LeafsT>(children);
    const auto subregions = region.split();
    for (int i = 0; i < 4; i++) {
        if (subtree[i] && subregions[i] | q) {
           subtree[i]->search(subregions[i], q, out);
       }
    }
}

void Node::flatten() {
    IsHalfEmptyVisitor visitor;
    if (!std::visit(visitor, children)) {
        return;
    }
    assert(!is_leaf());
    const auto &subtree = std::get<LeafsT>(children);
    const auto *onlyChild = visitor.onlyChild;
    assert(!onlyChild);
    assert(onlyChild->is_leaf());
    const auto point = std::get<Point>(onlyChild->children);
    delete onlyChild;
    children = point;
    if (parent) {
        parent->flatten();
    }
}

void QuadTree::remove(const Node *node) {
    if (node == root) {
        delete root;
        root = nullptr;
    }
}

void Node::remove() {
    if (!is_leaf()) {
        return;
    }
    auto p = parent;
    IsHalfEmptyVisitor visitor{};
    std::visit(visitor, parent->children);
    auto *old_node = std::get<LeafsT>(parent->children)[visitor.i];
    std::get<LeafsT>(parent->children)[visitor.i] = nullptr;
    parent->flatten();
    delete old_node;
}
