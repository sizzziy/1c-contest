#pragma once

#include <vector>
#include <string>

using std::vector;
using std::string;

class File {
public:
    File(const string& path);
    double Compare(const File& other) const;
    string GetPath() const;
private:
    void ReadBuffer();
    vector<uint8_t> Buffer_;
    string Path_;
};