#include "fenwick_tree.hxx"
#include <initializer_list>
#include <cmath>
#include <print>

Monoid sum{std::plus<int>{}, 0};
Monoid max{[](int a, int b){ return std::max(a, b); }, std::numeric_limits<int>::min()};

FenwickTree::FenwickTree(Monoid m, std::initializer_list<int> il) 
    : m{m} {
    tree.reserve(static_cast<int>(std::log2(il.size())) + 1);
    tree.emplace_back(il);
    auto size = il.size();
    while (size > 1) {
        size++;
        size /= 2;
        std::vector<int> new_vec(size);
        for (int i = 0; i < size; i++) {
            new_vec[i] = apply(tree.size(), i);
        }
        tree.emplace_back(std::move(new_vec));
    }
}

int FenwickTree::apply(int level, int idx) const {
    if (tree[level-1].size() <= idx*2+1) {
        return tree[level-1][idx*2]; 
    }
    return m.f(tree[level-1][idx*2], tree[level-1][idx*2+1]);
}

void FenwickTree::update(size_t idx, int value) {
    tree[0][idx] = value;
    for (int level = 1; level < tree.size(); level++) {
        idx /= 2;
        auto old = tree[level][idx];
        tree[level][idx] = apply(level, idx);
        if (old == tree[level][idx]) {
            return;
        }
    }
}

int FenwickTree::query(size_t src, size_t dest, int level, int res) const {
    if (src >= dest) {
        return res;
    }
    if (src % 2 != 0) {
        res = m.f(res, tree[level][src]);
        src++;
    }
    if (dest % 2 != 0) {
        res = m.f(res, tree[level][dest-1]);
        dest--;
    }
    return query(src/2, dest/2, level+1, res);
}

int FenwickTree::query(size_t src, size_t dest) const {
    return query(src, dest, 0, m.neutral);
}

void FenwickTree::print() const {
    for (const auto &level : tree) {
        std::println("{}", level);
    }
}

void FenwickTree::pop_back() {
    update(size() - 1, m.neutral);
    tree[0].pop_back();
    auto size = this->size();
    int i = 0;
    while (size > 1) {
        i++;
        size++;
        size /= 2;
        if (tree[i].size() == size) {
            break;
        }
        tree[i].pop_back();
    }
    if (size == 1) {
        tree.pop_back();
    }
}
