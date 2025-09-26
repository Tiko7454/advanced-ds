#include "skiplist.hxx"

int main() {
    SkipList sl{};
    sl.insert(12).insert(32).insert(26);
    sl.print();
}
