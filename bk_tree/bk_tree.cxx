#include "bk_tree.hxx"
#include <numeric>

size_t hamming_distance(std::string_view lhs, std::string_view rhs) {
    const size_t min_size = std::min(lhs.size(), rhs.size());
    size_t result = std::max(lhs.size(), rhs.size()) - min_size;
    for (size_t i = 0; i < min_size; i++){
        result += (lhs[i] != rhs[i]);
    }
    return result;
}

void BKTree::Node::insert(std::string_view word, const metric_t &metric) {
    const size_t dist = metric(this->value, word);
    const auto it = children.find(dist);
    if (it == children.end()) {
        children[dist] = new Node{word};
        return;
    }
    it->second->insert(word, metric);
}

std::ranges::iota_view<size_t, size_t> BKTree::Node::get_lookup_range(size_t dist, size_t tolerance) {
    size_t start = 0;
    if (dist >= tolerance) {
        start = dist - tolerance;
    }
    return std::views::iota(start, dist + tolerance + 1);
}

