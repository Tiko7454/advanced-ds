#ifndef BLOOM_FILTER_HXX
#define BLOOM_FILTER_HXX

#include <functional>
#include <string_view>

using hash_t = std::function<unsigned long long(std::string_view)>;

unsigned long long generate_prime();

hash_t generate_hash_function();

class BloomFilter {
public:
    BloomFilter(size_t k, size_t n);
    BloomFilter& insert(std::string_view str);
    bool exists(std::string_view str) const;
    double get_probability() const;
private:
    std::vector<bool> content;
    std::vector<hash_t> hashes;
    size_t count;
    void push(unsigned long long x) {
        content[x % content.size()] = 1;
    }
    bool get(unsigned long long x) const {
        return content[x % content.size()];
    }
};

#endif
