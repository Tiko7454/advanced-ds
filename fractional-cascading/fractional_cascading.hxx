#ifndef FRACTIONAL_CASCADING_HXX
#define FRACTIONAL_CASCADING_HXX

#include <cstddef>
#include <vector>
#include <algorithm>
#include <array>
#include <ranges>

struct Item {
    size_t this_index;
    size_t prev_index;
    int value;
    bool is_brought;
    bool operator<(const Item& other) const {
        return value < other.value;
    }
    Item(int value) : this_index{}, prev_index{}, value{value}, is_brought{false} {};
};
inline Item make_item(int value) {
    return {value};
}

template<size_t Jump=1>
struct Iterator {
    const Item* content;
    const Item& operator*() const {
        return *content;
    }
    Iterator<Jump> operator++() {
        return Iterator{content += Jump};
    }
};

template<size_t Jump=1>
bool operator==(const Iterator<Jump>& it, const Iterator<Jump>& end) {
    return it.content >= end.content;
}

template<size_t K>
class Merged {
    std::vector<Item> elems;
    std::vector<std::array<size_t, 2>> indices;
    Merged<K-1> previous_layer;
public:
    Merged(std::initializer_list<int> current, Merged<K-1> previous_layer) 
        : elems{}, previous_layer{previous_layer} {
        auto items = current | std::views::transform(make_item);
        std::merge(items.begin(), items.end(), previous_layer.begin_2(), previous_layer.end_2(), std::back_inserter(elems));
        for (auto& el : elems) {
            if (el.is_brought) {
                el.prev_index += 2;
            } else {
                el.this_index++;
            }
        }
    }
    const Iterator<1> begin() const {
        return Iterator<1>{elems.begin().base()};
    }
    const Iterator<2> begin_2() const {
        return Iterator<2>{elems.begin().base()};
    }
    const Iterator<1> end() const {
        return Iterator<1>{elems.end().base()};
    }
    const Iterator<2> end_2() const {
        return Iterator<2>{elems.end().base()};
    }
};

template<>
class Merged<0> {
public:
    const Iterator<1> begin() const {
        return Iterator<1>{nullptr};
    }
    const Iterator<2> begin_2() const {
        return Iterator<2>{nullptr};
    }
    const Iterator<1> end() const {
        return Iterator<1>{nullptr};
    }
    const Iterator<2> end_2() const {
        return Iterator<2>{nullptr};
    }
};


inline auto build_merged() {
    return Merged<0>{};
}

template<typename... Arrays>
inline auto build_merged(std::initializer_list<int> head, Arrays... tail) {
    return Merged<sizeof...(Arrays)+1>{head, build_merged(tail...)};
}


#endif // FRACTIONAL_CASCADING_HXX
