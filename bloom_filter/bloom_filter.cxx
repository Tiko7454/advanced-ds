#include "bloom_filter.hxx"
#include <algorithm>
#include <ranges>
#include <vector>

unsigned long long generate_prime() {
    static std::vector<char> is_prime{0, 0, 1};
    int old_size = is_prime.size();
    
    unsigned long long i = old_size;
    for (int j = 2; j < old_size; j++) {
        if (!is_prime[j]) {
            continue;
        }
        if (i % j == 0) {
            is_prime.push_back(0);
            i++;
            j = 1;
            continue;
        }
    }
    is_prime.push_back(1);
    return i;
}

hash_t generate_hash_function() {
    auto key = generate_prime();
    return [key](std::string_view str){
        unsigned long long pow{key};
        unsigned long long res{};
        for (const char ch : str) {
            res += pow * ch;
            pow *= key;
        }
        return res;
    };
}

BloomFilter::BloomFilter(int k, int n) : content(n*k*2) {
    hashes.reserve(k);
    for (int i = 0; i < k; i++){
        hashes.emplace_back(generate_hash_function());
    }
}

BloomFilter& BloomFilter::insert(std::string_view str) {
    for (const auto &hash : hashes) {
        push(hash(str));
    }
    return *this;
}

bool BloomFilter::exists(std::string_view str) const {
    auto indices = hashes | std::views::transform([=, *this](auto hash){ return get(hash(str)); });
    return std::all_of(indices.begin(), indices.end(), std::identity{});
}

double BloomFilter::get_probability() const {
    
}
