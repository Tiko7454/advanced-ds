#include "bsp.hxx"

Line Segment::expand() const {
    auto det = start.real() * end.imag() - start.imag() * end.real();
    if (det == 0) {
        auto x = start.real();
        auto y = start.imag();
        if (x == 0) {
            x = end.real();
            y = end.imag();
        }
        auto hypot = std::hypot(x,y);
        auto B = x / hypot;
        auto A = -y / hypot; 
        return {A, B, 0};
    }
    auto AB = (start - end) / det;
    auto len = std::abs(AB);
    auto normal_AB = AB / len;
    return {normal_AB.real(), normal_AB.imag(), -1./len};
}

bool Segment::contains(std::complex<double> p) const {
    return std::min(start.real(), end.real()) <= p.real()
        && std::min(start.imag(), end.imag()) <= p.imag()
        && std::max(start.real(), end.real()) >= p.real()
        && std::max(start.imag(), end.imag()) >= p.imag();
}

std::optional<std::complex<double>> Segment::operator|(Line l) {
    return (expand() | l)
        .and_then([this](auto c){ return contains(c) ? std::optional(c) : std::nullopt; });
}

std::optional<std::complex<double>> operator|(Line l, Line p) {
    auto det = l.a * p.b - l.b * p.a;
    if (det == 0) {
        return std::nullopt;
    }
    auto x = (-l.c * p.b + p.c * l.b) / det;
    auto y = (-p.c * l.a + l.c * p.a) / det;
    return {{x, y}};
}

std::optional<std::complex<double>> operator|(Line l, Segment s) {
    return l | s;
}

std::array<Segment, 2> Segment::partition(Line l) const {
    auto intersection = (l|*this).value();
    return {Segment{start, intersection}, Segment{intersection, end}};
}

auto Line::get_direction(std::complex<double> point) -> Direction {
    if ((*this)(point) > 0) {
        return Direction::Front;
    }
    return Direction::Back;
}

double Line::operator()(std::complex<double> point) {
    return a*point.real() + b*point.imag() + c;
}

