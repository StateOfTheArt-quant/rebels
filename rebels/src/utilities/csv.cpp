#include "rebels/utilities/csv.h"

#include <limits>
#include <iostream>

CSV::CSV(const std::string& file_path){
    openFile(file_path);
}

void CSV::openFile(const std::string& file_path){
    closeFile();

    fs.open(file_path);
    if (not fs.is_open()){
        std::cout << "[!] Failed to open file: " << file_path << std::endl;
        exit(1);
    }
}

void CSV::closeFile(){
    if(fs.is_open()){
        fs.close();
    }
}

bool CSV::isOpen(){ return fs.is_open(); }
bool CSV::hasData() { return !fs.eof(); }

void CSV::parseRow(std::string& line, std::vector<std::string>& cols){

    size_t pos = 0;
    while (true){
        size_t next = line.find(",", pos);

        if ( next == std::string::npos){
            cols.push_back(line.substr(pos, std::string::npos));
            break;
        } else {
            cols.push_back(line.substr(pos, next-pos));
            pos = next+1;
        }
    }
}

void CSV::next(std::vector<std::string>& cols)
{
    std::string line;
    getline(fs, line);
    parseRow(line, cols);
}

void CSV::peak(std::vector<std::string>& cols){
    std::string line;

    long pos = fs.tellg();
    getline(fs, line);
    parseRow(line, cols);

    fs.seekg(pos);
}

void CSV::skip(int n_lines){
    for (int n=0; n < n_lines; n++){
        fs.ignore(std::numeric_limits<std::streamsize>::max(), fs.widen('\n'));

        if (not hasData()){
            return;
        }
    }
}
