#include "prquadtree.hxx"

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

bool Rect::contains(Point p) const {
    return left_top.x <= p.x and p.x <= right_bot.x and
        right_bot.y <= p.y and p.y <= left_top.y;
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
    if (p.x >= mid_x and p.y <= mid_y) {
        return 2;
    }
    if (p.x >= mid_x and p.y >= mid_y) {
        return 3;
    }
    return -1;
}

void Node::insert(Point p, const Rect& region) {
    const auto splitted = region.split();
    if (is_leaf()) {
        const auto old = std::get<Point>(children);
        auto new_children = get_nullptr_array();
        int i = region.get_index(old);
        auto* new_node = new Node{this, old};
        new_children[i] = new_node;
        children = new_children;
    }
    ...
}
