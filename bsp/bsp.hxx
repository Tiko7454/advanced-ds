#include <vector>
#ifndef BSP

#include <complex>
#include <optional>
#include <array>

struct Segment;

struct Line {
    double a, b, c;
    enum class Direction { Front, Back };
    Direction get_direction(std::complex<double> point);
    double operator()(std::complex<double> point);
    template<std::output_iterator<Segment> It>
    std::array<It, 2> partition(const std::vector<Segment>& vec, It front, It back);
};


struct Segment {
    std::complex<double> start, end;
    Line expand() const;
    std::optional<std::complex<double>> operator|(Line l);
    std::array<Segment, 2> partition(Line) const;
private:
    bool contains(std::complex<double>) const;
};

template<std::output_iterator<Segment> It>
std::array<It, 2> Line::partition(const std::vector<Segment>& vec, It front, It back) {
    for (auto seg: vec) {
        auto intersection = seg | *this;
        if (intersection) {
        } else {
            switch (get_direction(seg.start)) {
                case Direction::Front:
                    *front = seg;
                    front++;
                    break;
                case Direction::Back:
                    *back = seg;
                    back++;
                    break;
            }
        }
    }
}


class BSP {
private:
    struct Node{
        Segment value;
        Node* left;
        Node* right;
        ~Node() {
            delete left;
            delete right;
        }
    };
public:
    Node* root;
    ~BSP() {
        delete root;
    }
};


std::optional<std::complex<double>> operator|(Line l, Line p);
std::optional<std::complex<double>> operator|(Line l, Segment s);

#endif // BSP
