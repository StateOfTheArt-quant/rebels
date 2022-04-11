#pragma once

#include <vector>
#include <fstream>
#include <string>

class CSV{
private:
    std::ifstream fs;

    void parseRow(std::string& line, std::vector<std::string>& cols);

public:
    CSV() = default;
    CSV(const std::string& file_path);

    //file control methods
    void openFile(const std::string& file_path);
    void closeFile();

    bool isOpen();
    bool hasData();

    void next(std::vector<std::string>& columns);
    void peak(std::vector<std::string>& columns);
    void skip(int n_lines=1);

};