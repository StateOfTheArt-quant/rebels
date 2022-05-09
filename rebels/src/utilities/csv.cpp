#include <limits>
#include <iostream>

#include "rebels/utilities/csv.h"

void FileReader::openFile(const std::string& file_path) {
    // close file if is opened
    closeFile();
    // open file with stream
    __fs.open(file_path);

    if (!isOpen()) {
        std::cerr << "[!] Failed to open file: " << file_path << std::endl;
        exit(-1);
    }
}

void FileReader::closeFile() {
    if (isOpen()) {
        __fs.close();
    }
}

bool FileReader::isOpen() noexcept { return __fs.is_open(); }

bool FileReader::hasData() noexcept { return !__fs.eof(); }

CSV::CSV(const std::string& file_path) { openFile(file_path); }

void CSV::parseRow(std::string& line, std::vector<std::string>& cols) {
    // local variables
    size_t next, pos = 0;

    while (1) {
        // find comma in current line
        next = line.find(",", pos);

        // move file ptr
        if (next == std::string::npos) {
            cols.push_back(line.substr(pos, std::string::npos));
            break;
        } else {
            cols.push_back(line.substr(pos, next - pos));
            pos = next + 1;
        }
    }
}

void CSV::next(std::vector<std::string>& columns) {
    // copy data to rows
    std::string line;
    // get fs stream to line
    getline(__fs, line);
    // format csv data
    parseRow(line, columns);
}

void CSV::peak(std::vector<std::string>& columns) {
    // local variables
    std::string line;
    std::streampos pos;
    // line length
    pos = __fs.tellg();
    // get fs stream to line
    getline(__fs, line);
    // format csv data
    parseRow(line, columns);
    // move ptr
    __fs.seekg(pos);
}

void CSV::skip(int n_lines) {
    for (int n = 0; n < n_lines; n++) {
        // ignore lines endwith '\n'
        __fs.ignore(std::numeric_limits<std::streamsize>::max(), __fs.widen('\n'));

        if (!hasData()) {
            return;
        }
    }
}
