#include "hash.h"

PolynomialHash::PolynomialHash(const vector<uint8_t>& str, size_t powerLen) {
    FillPowerMod(powerLen);
    PrefixHash_.resize(str.size());
    PrefixHash_[0] = {str[0], str[0]};
    for (size_t i = 1; i < str.size(); ++i) {
        PrefixHash_[i].A = PrefixHash_[i - 1].A + static_cast<uint64_t>(str[i]) * PowerMod_[i].A;
        PrefixHash_[i].A %= MOD1;
        PrefixHash_[i].B = PrefixHash_[i - 1].B + static_cast<uint64_t>(str[i]) * PowerMod_[i].B;
        PrefixHash_[i].B %= MOD2;
    }
}

void PolynomialHash::FillPowerMod(size_t len) {
    PowerMod_.resize(len);
    PowerMod_[0].A = 1;
    PowerMod_[0].B = 1;
    for (size_t i = 1; i < len; ++i) {
        PowerMod_[i].A = PowerMod_[i - 1].A * BASE1;
        PowerMod_[i].A %= MOD1;
        PowerMod_[i].B = PowerMod_[i - 1].B * BASE2;
        PowerMod_[i].B %= MOD2;
    }
}

PolynomialHash::Point PolynomialHash::SubHashPowered(size_t l, size_t r, uint64_t p) {
    return (PrefixHash_[r] - (l > 0? PrefixHash_[l - 1] : Point{0, 0})) * PowerMod_[p];
}