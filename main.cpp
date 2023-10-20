#include <iostream>
#include <string>
#include <filesystem>
#include <iomanip>
#include <cmath>

#include "hash.h"
#include "file.h"

using std::string;
namespace fs = std::filesystem;

void output(const string& path1, const string& path2, string verdict) {
    std::cout << "------------------------\n";
    std::cout << path1 << " and " << path2 << verdict << std::endl;
}

void output(const string& path1, string verdict) {
    std::cout << "------------------------\n";
    std::cout << path1 << verdict << std::endl;
}

int main() {
    std::cout << "Enter first directory" << std::endl;
    string pathToDirectory1;
    std::cin >> pathToDirectory1;
    if (!fs::is_directory(pathToDirectory1)) {
        std:: cout << "No such directory" << std::endl;
        return 1;
    }
    std::cout << "Enter second directory" << std::endl;
    string pathToDirectory2;
    std::cin >> pathToDirectory2;
    if (!fs::is_directory(pathToDirectory2)) {
        std:: cout << "No such directory" << std::endl;
        return 1;
    }
    
    std::vector<File> firstDir;
    std::vector<File> secondDir;
    for (const auto& entry1 : fs::directory_iterator(pathToDirectory1)) {
        firstDir.emplace_back(entry1.path());
    }
    for (const auto& entry2 : fs::directory_iterator(pathToDirectory2)) {
        secondDir.emplace_back(entry2.path());
    }

    std::cout << "Enter similarity threshold in double format [0, 1]" << std::endl;
    double similarityThreshold;
    std::cin >> similarityThreshold;
    if (similarityThreshold > 1 || similarityThreshold < 0) {
        std::cout << "Invalid threshold" << std::endl;
        return 1;
    }
    std::vector<bool> foundSimilarFirst(firstDir.size());
    std::vector<bool> foundSimilarSecond(secondDir.size());
    for (size_t i = 0; i < firstDir.size(); ++i) {
        const auto& file1 = firstDir[i];
        for (size_t j = 0; j < secondDir.size(); ++j) {
            const auto& file2 = secondDir[j];
            double similarity = file1.Compare(file2);
            if (similarity > 1.5) {
                output(file1.GetPath(), file2.GetPath(), " are identical");
                foundSimilarSecond[j] = true;
                foundSimilarFirst[i] = true;
            } else if (similarity >= similarityThreshold) {
                output(file1.GetPath(), file2.GetPath(), " similar by " + std::to_string(static_cast<int>(std::round(similarity * 100))) + "%");
                foundSimilarSecond[j] = true;
                foundSimilarFirst[i] = true;
            }
        }
    }
    for (size_t i = 0; i < firstDir.size(); ++i) {
        if (!foundSimilarFirst[i]) {
            output(firstDir[i].GetPath(), " only in first directory");
        }
    }
    for (size_t i = 0; i < secondDir.size(); ++i) {
        if (!foundSimilarSecond[i]) {
            output(secondDir[i].GetPath(), " only in second directory");
        }
    }
}