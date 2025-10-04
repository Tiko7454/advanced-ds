#ifndef FENWICK_TREE_HXX
#define FENWICK_TREE_HXX
#include <functional>
#include <initializer_list>
#include <vector>

struct Monoid {
    std::function<int(int,int)> f;
    int neutral;
};

extern Monoid sum;
extern Monoid max;

class FenwickTree {
public:
    FenwickTree(Monoid m, std::initializer_list<int> il);
    int operator[](size_t idx) { return tree[0][idx]; };
    std::vector<int>::iterator begin() { return tree[0].begin(); }
    std::vector<int>::iterator end() { return tree[0].end(); }
    std::vector<int>::const_iterator begin() const { return tree[0].begin(); }
    std::vector<int>::const_iterator end() const { return tree[0].end(); }
    void update(size_t idx, int value);
    void print() const;
    int query(size_t src, size_t dest) const;
    // int push_back(int value);
    void pop_back();
    int size() const { return end() - begin(); }
    bool empty() const { return size() == 0; }
private:
    std::vector<std::vector<int>> tree;
    Monoid m;
    int apply(int level, int idx) const;
    int query(size_t src, size_t dest, int level, int res) const;
};

#endif // FENWICK_TREE_HXX
