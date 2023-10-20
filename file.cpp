#include "file.h"
#include "hash.h"

#include <fstream>
#include <iterator>
#include <set>
#include <exception>

using std::set;

const size_t LCS_THRESHOLD = 100'000;

int LongestCommonSubsequence(const vector<uint8_t>& first, const vector<uint8_t>& second) {
    vector<vector<int>> LCSPrefix(first.size() + 1, vector<int>(second.size() + 1));
    int result = 0;
    for (size_t i = 1; i <= first.size(); ++i) {
        for (size_t j = 1; j <= second.size(); ++j) {
            LCSPrefix[i][j] = std::max(LCSPrefix[i - 1][j], LCSPrefix[i][j - 1]);
            if (first[i - 1] == second[j - 1]) {
                LCSPrefix[i][j] = std::max(LCSPrefix[i][j], LCSPrefix[i - 1][j - 1] + 1);
                result = std::max(result, LCSPrefix[i][j]);
            }
        }
    }
    return result;
}


/*
O(n log n)
finding maximal substring with polynomial hashes
*/
int MaximalSubstr(const vector<uint8_t>& first, const vector<uint8_t>& second) {
    size_t n = first.size();
    size_t m = second.size();
    PolynomialHash hash1(first, n + m + 2);
    PolynomialHash hash2(second, n + m + 2);
    int l = 0;
    int r = std::min(n, m) + 1;
    while (r - l > 1) {
        int substrLen = (l + r) / 2;

        // using set because in g++ set<int> usually works faster than unordered_set<int> when less than 100'000 elements
        set<PolynomialHash::Point> substrHashesInSecond;
        for (size_t leftIdx = 0; leftIdx < n; ++leftIdx) {
            size_t rightIdx = leftIdx + substrLen - 1;
            if (rightIdx >= m) {
                break;
            }
            substrHashesInSecond.insert(hash2.SubHashPowered(leftIdx, rightIdx, n + m - leftIdx));
        }

        bool foundSubstr = false;
        for (size_t leftIdx = 0; leftIdx < n; ++leftIdx) {
            size_t rightIdx = leftIdx + substrLen - 1;
            if (rightIdx >= n) {
                break;
            }
            auto subHash = hash1.SubHashPowered(leftIdx, rightIdx, n + m - leftIdx);
            if (substrHashesInSecond.find(subHash) != substrHashesInSecond.end()) {
                foundSubstr = true;
                break;
            }
        }
        if (foundSubstr) {
            l = substrLen;
        } else {
            r = substrLen;
        }
    }
    return l;
}

File::File(const string& path) {
    Path_ = path;
    ReadBuffer();
}

string File::GetPath() const {
    return Path_;
}

void File::ReadBuffer() {
    std::ifstream input(Path_, std::ios::binary);
    if (!input.good()) {
        throw "Bad file";
    }
    Buffer_ = std::vector<uint8_t>(std::istreambuf_iterator<char>(input), {});
}

double File::Compare(const File& other) const {
    if (Buffer_ == other.Buffer_) {
        return true;
    }
    int maxFile = std::max(Buffer_.size(), other.Buffer_.size());
    uint64_t similarity;
    if (maxFile > LCS_THRESHOLD) {
        similarity = MaximalSubstr(Buffer_, other.Buffer_);
    } else {
        similarity = LongestCommonSubsequence(Buffer_, other.Buffer_);
    }
    if (similarity == maxFile) {
        return 2;
    }
    return similarity * 1.0 / maxFile;
}