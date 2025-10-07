#ifndef BK_TREE_HXX
#define BK_TREE_HXX
#include <functional>
#include <string>
#include <string_view>
#include <ranges>


size_t hamming_distance(std::string_view lhs, std::string_view rhs);

class BKTree {
private:
    using metric_t = std::function<size_t(std::string_view, std::string_view)>;
    struct Node {
        std::string value;
        std::unordered_map<int, Node*> children;
        ~Node() { for (const auto &[_, child] : children) { delete child; } }
        void insert(std::string_view word, const metric_t& metric);
        Node(std::string_view value) : value{value} {}
        template<std::output_iterator<std::string> It>
        void search(std::string_view word, size_t tolerance, const metric_t& metric, It& gde) const;
        static std::ranges::iota_view<size_t, size_t> get_lookup_range(size_t dist, size_t tolerance);
    };
    const metric_t metric;
    Node* root;
public:
    BKTree(metric_t metric) : metric{metric}, root{} {}
    BKTree& insert(std::string_view word) {
        if (!root) {
            root = new Node{word};
            return *this;
        }
        root->insert(word, metric);
        return *this;
    }
    template<std::output_iterator<std::string> It>
    It search(std::string_view word, size_t tolerance, It gde) const;
};

template<std::output_iterator<std::string> It>
It BKTree::search(std::string_view word, size_t tolerance, It gde) const {
    if (root) {
        root->search(word, tolerance, metric, gde);
    }
    return gde;
}

template<std::output_iterator<std::string> It>
void BKTree::Node::search(std::string_view word, size_t tolerance, const metric_t& metric, It& gde) const {
    const size_t dist = metric(this->value, word);
    if (dist <= tolerance) {
        *gde = this->value;
        gde++;
    }
    for (size_t i : get_lookup_range(dist, tolerance)) {
        const auto it = children.find(i);
        if (it == children.end()) {
            continue;
        }
        it->second->search(word, tolerance, metric, gde);
    }
}

#endif // BK_TREE_HXX
