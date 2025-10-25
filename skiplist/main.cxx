#include "skiplist.hxx"
#include <print>

int main() {
    SkipList sl{};
    sl.insert(12).insert(32).insert(26);
    sl.insert(13).insert(33).insert(36);
    sl.insert(14).insert(34).insert(46);
    sl.insert(15).insert(35).insert(12);
    sl.remove(14);
    std::println("{}", sl.find(46)->get_value());
    std::println("{}", sl.find(100) == sl.end());
    std::println("{}", sl.find(0) == sl.end());
    std::println("{}", sl.find(38) == sl.end());
    sl.print();
}
