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

ProcessReader::ProcessReader(const std::string& path, int32_t skip_rows, const char& delimiter)
    : _delimiter(delimiter), _stop(false) {
    openFile(path);

    // get first line as output column
    skip_rows--;
    getline(__fs, _header);

    // skip row when needed
    if (skip_rows > 0) skip(skip_rows);
}

ProcessReader::~ProcessReader() { closeFile(); }

bool ProcessReader::is_readable() noexcept { return (hasData() && !_stop); }

std::string& ProcessReader::head() { return _header; }

void ProcessReader::next(std::string& out) {
    if (!is_readable()) return;

    std::string temp;
    getline(__fs, temp);

    if (temp.empty()) {
        _stop = true;
        return;
    }

    // 每次都查找一次
    size_t pos_end = temp.find(_delimiter);
    date           = temp.substr(0UL, pos_end);
    out            = temp.substr(pos_end);
}

void ProcessReader::skip(int n) {
    while (n--) {
        if (is_readable()) {
            break;
        }

        __fs.ignore(std::numeric_limits<std::streamsize>::max(), __fs.widen('\n'));
    }
}

ProcessWriter::ProcessWriter(const std::string& path) {
    // open file with stream
    __ofs.open(path);

    if (!__ofs.is_open()) {
        std::cerr << "[!] Failed to open file: " << path << std::endl;
        exit(-1);
    }
}

ProcessWriter::~ProcessWriter() { __ofs.close(); }

void ProcessWriter::write(const std::string& data) { __ofs << data; }

void ProcessWriter::write_cache(const std::string& date_, const std::string end) {
    __ofs << date_ << cache << end;
}

AligmentProcess::AligmentProcess(std::vector<std::string> files,
                                 const std::string& out_dir,
                                 const std::string& date_slash,
                                 int32_t skip_rows,
                                 const char& delimiter) {
    // 初始化所有文件及对应的输出
    for (auto file_path : files) {
        // 获取文件名
        std::string filename = _get_file_name(file_path);
        reader_ptr rp        = std::make_shared<ProcessReader>(file_path, skip_rows, delimiter);
        writer_ptr wp        = std::make_shared<ProcessWriter>(out_dir + "/" + filename);
        // 写入头文件并进入下一行
        wp->write(rp->head() + "\n");
        rp->next(wp->cache);
        // 存入读写字典
        __read_map[filename]  = rp;
        __write_map[filename] = wp;
    }
    // 初始日期
    __init_date = "2999" + date_slash + "12" + date_slash + "31";
}

std::string AligmentProcess::_get_file_name(const std::string& path) {
    size_t slash_pos = path.find_last_of('/') + 1;
    return path.substr(slash_pos);
}

void AligmentProcess::process() {
    while (1) {
        // 终止条件
        if (__read_map.empty()) break;

        // 初始化最小日期
        std::string min_date = __init_date;
        // 查询读取字典
        for (auto& item : __read_map) {
            std::string key   = item.first;
            reader_ptr reader = item.second;

            min_date = std::min(min_date, reader->date);

            __write_map[key]->date = reader->date;
        }

        // 查询写入字典
        for (auto& item : __write_map) {
            std::string key   = item.first;
            writer_ptr writer = item.second;

            // 写入
            writer->write_cache(min_date);

            if (min_date >= writer->date) {
                // 查找
                auto iter = __read_map.find(key);

                if (iter != __read_map.end()) {
                    iter->second->next(writer->cache);

                    // 判断是否可读，不可读则从读取字典中删除
                    if (!iter->second->is_readable()) {
                        __read_map.erase(key);
                        // TODO 用于填充默认数据时使用
                        // writer->cache = "";
                    }
                }
            }
        }
    }
}
