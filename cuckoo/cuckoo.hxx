#ifndef CUCKOO_HXX
#define CUCKOO_HXX

#include <print>
#include <string>
#include <functional>
#include <optional>
#include <ranges>
#include <algorithm>

class HashBucket {
    using hash_t = std::function<size_t(std::string_view)>;
    static hash_t make_hash(size_t k);
    std::vector<std::optional<std::string>> data;
    static bool is_pow_2(size_t num) {
        return (num & (num - 1)) == 0;
    }
public:
    const hash_t hash;
    HashBucket(size_t pow_of_2, size_t hash_key) 
        : hash{make_hash(hash_key)}, data(1 << pow_of_2) {}
    std::optional<std::string>& operator[](size_t i) {
        return data[i & (data.size() - 1)];
    }
    std::optional<std::string>& operator[](std::string_view sv) {
        return (*this)[hash(sv)];
    }
    const std::optional<std::string>& operator[](size_t i) const {
        return data[i & (data.size() - 1)];
    }
    const std::optional<std::string>& operator[](std::string_view sv) const {
        return (*this)[hash(sv)];
    }
    void print() {
        std::println("{}", data | std::views::transform([](std::optional<std::string> opt){ return opt.value_or("***"); }));
    }
    auto make_view() const {
        return data 
            | std::views::filter([](std::optional<std::string> opt){ return opt.has_value(); })
            | std::views::transform([](std::optional<std::string> opt) { return *opt; });
    }
};

class CuckooSet {
public:
    CuckooSet(size_t k = 5) : buckets{{2, 2}, {2, 3}}, size{}, k{k} {};
    void insert(std::string_view sv);
    void print() {
        buckets[0].print();
        buckets[1].print();
    }
    // auto make_view() const {
    //     return std::views::concat()
    // }
private:
    bool shuffle(std::string_view sv, size_t bucket_number, size_t left);
    HashBucket buckets[2];
    size_t size;
    size_t k;
private:
};

#endif // CUCKOO_HXX
