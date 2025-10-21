#include "bloom_filter.hxx"

int main() {
    BloomFilter bf(3, 1000);
    bf.insert("aaaaaaaaaaaaaaaa");
    bf.exists("bbbbbbbbbbbbbbbb");
}
