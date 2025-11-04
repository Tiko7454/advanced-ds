#include "cuckoo.hxx"

HashBucket::hash_t HashBucket::make_hash(size_t k) {
    return [k](std::string_view sv) {
        size_t pow{k};
        size_t res{};
        for (const char ch : sv) {
            res += pow * ch;
            pow *= k;
        }
        return res;
    };
}

bool CuckooSet::shuffle(std::string_view sv, size_t bucket_number, size_t left) {
    if (left == 0) {
        return false;
    }
    auto& el_to_displace = buckets[1-bucket_number][sv];
    if (!el_to_displace) {
        *el_to_displace = sv;
        return true;
    }
    auto res = shuffle(*el_to_displace, 1-bucket_number, left-1);
    if (res) {
        el_to_displace = sv;
        return true;
    }
    return false;
}

void CuckooSet::insert(std::string_view sv) {
    size++;
    auto i = size % 2;
    std::optional<std::string>* positions[2];
    positions[i] = &buckets[i][sv];
    if (!positions[i]->has_value()) {
        *positions[i] = sv;
        return;
    }
    if (*positions[i] == sv) {
        size--;
        return;
    }
    positions[1-i] = &buckets[1-i][sv];
    if (!positions[1-i]->has_value()) {
        *positions[1-i] = sv;
        return;
    }
    if (*positions[1-i] == sv) {
        size--;
        return;
    }
    if (shuffle(**positions[i], i, k)) {
        *positions[i] = sv;
        return;
    }
    throw "cannot insert";
}
