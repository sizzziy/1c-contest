#pragma once

#include <vector>
#include <cstdint>

using std::size_t;
using std::vector;

/* Double polynomial hash algorithm implemented */

// All numbers are prime
const uint64_t MOD1 = 967430887;
const uint64_t MOD2 = 298223729;
const uint64_t BASE1 = 23;
const uint64_t BASE2 = 13;


class PolynomialHash {
public:
    struct Point {
        uint64_t A;
        uint64_t B;
        Point operator+(const Point& other) {
            return {A + other.A, B + other.B};
        }
        Point operator-(const Point& other) {
            return {A - other.A, B - other.B};
        }
        Point operator*(const Point& other) {
            return {A * other.A % MOD1, B * other.B % MOD2};
        }
        bool operator<(const Point& other) const {
            return (A == other.A && B < other.B) || (A < other.A);
        }
    };
    PolynomialHash(const vector<uint8_t>& str, size_t powerLen);
    Point SubHashPowered(size_t l, size_t r, uint64_t p);
private:
    vector<Point> PrefixHash_;
    vector<Point> PowerMod_;
private:
    void FillPowerMod(size_t len);
};