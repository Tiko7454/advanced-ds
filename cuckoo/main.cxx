#include "cuckoo.hxx"

int main() {
    CuckooSet s;
    s.insert("aaa");
    s.insert("bbb");
    s.insert("ccc");
    s.insert("ddd");
    s.print();
    s.insert("eee");
}
