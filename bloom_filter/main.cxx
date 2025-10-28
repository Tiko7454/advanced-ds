#include "bloom_filter.hxx"
#include <print>
#include <random>
#include <chrono>
#include <unordered_set>

// stolen from chatgpt
std::string random_string(const std::string& charset = "abcdefghijklmnopqrstuvwxyz") {
    // static so it's created once per program run
    static std::mt19937_64 rng(
        std::chrono::high_resolution_clock::now().time_since_epoch().count()
    );

    std::uniform_int_distribution<size_t> char_dist(0, charset.size() - 1);
    std::chi_squared_distribution<double> length_dist(7);

    auto length = length_dist(rng);

    std::string out;
    out.reserve(length);
    for (std::size_t i = 0; i < length; ++i) {
        out += charset[char_dist(rng)];
    }
    std::println("{}", out);
    return out;
}

int main() {
    BloomFilter bf(3, 1000);
    std::unordered_set<std::string> elements;
    for (int i{}; i < 10; i++) {
        auto a = random_string();
        bf.insert(a);
        elements.insert(a);
    }

    int b{};
    for (int i{}; i < 10; i++) {
        auto a = random_string();
        if (elements.contains(a)) {
            i--;
            continue;
        }
        b += bf.exists(a);
    }
    std::println("{}", bf.get_probability());
}
