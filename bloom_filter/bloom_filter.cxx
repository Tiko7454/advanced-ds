#include "bloom_filter.hxx"
#include <algorithm>
#include <cmath>
#include <ranges>
#include <concepts>
#include <utility>
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

BloomFilter::BloomFilter(size_t k, size_t n) : content(n*k*2), count{} {
    hashes.reserve(k);
    for (size_t i{}; i < k; i++){
        hashes.emplace_back(generate_hash_function());
    }
}

template<typename T>
concept arithmetic = std::is_arithmetic_v<T>;

template<arithmetic T>
static inline T int_pow_(T base, size_t power, T result) {
    if (power == 0) {
        return result;
    }
    return int_pow_(base, power - 1, result * base);
}

template<arithmetic T>
static inline T int_pow(T base, size_t power) {
    return int_pow_(base, power, static_cast<T>(1));
}

double BloomFilter::get_probability() const {
    const auto m = content.size();
    const auto k = hashes.size();
    const auto n = count;
    return int_pow(1 - std::exp(-static_cast<double>(k * n) / m), k);
}

BloomFilter& BloomFilter::insert(std::string_view str) {
    for (const auto &hash : hashes) {
        push(hash(str));
    }
    ++count;
    return *this;
}

bool BloomFilter::exists(std::string_view str) const {
    auto indices = hashes | std::views::transform([=, *this](auto hash){ return get(hash(str)); });
    return std::all_of(indices.begin(), indices.end(), std::identity{});
}
