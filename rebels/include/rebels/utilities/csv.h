#pragma once

#include <vector>
#include <string>
#include <fstream>

/// base reader
class FileReader {
protected:
    std::ifstream __fs;

public:
    // constructor
    FileReader() = default;

    // file control methods
    void openFile(const std::string& file_path);
    void closeFile();

    // file state methods
    bool isOpen() noexcept;
    bool hasData() noexcept;
};

/// csv reader
class CSV : public FileReader {
private:
    void parseRow(std::string& line, std::vector<std::string>& cols);

public:
    // constructor
    CSV() = default;
    CSV(const std::string& file_path);

    // csv control methods
    void skip(int n_lines = 1);
    void next(std::vector<std::string>& columns);
    void peak(std::vector<std::string>& columns);
};
