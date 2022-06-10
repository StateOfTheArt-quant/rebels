#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <memory>

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

// 用于处理数据对齐时使用的读取文本类
class ProcessReader : public FileReader {
protected:
    bool _stop;             // 停止标识
    const char _delimiter;  // 分隔符号
    std::string _header;    // 写入csv或文件的头

public:
    std::string date;

    ProcessReader(const std::string& path, int32_t skip_rows = 1, const char& delimiter = ',');
    ~ProcessReader();

    // 自定义可读操作标识
    bool is_readable() noexcept;
    std::string& head();

    // 获取下一条数据
    void next(std::string& out);
    void skip(int n);
};

// 用于处理数据对齐时使用的写文本类
class ProcessWriter {
private:
    std::ofstream __ofs;

public:
    std::string cache;
    std::string date;

    ProcessWriter(const std::string& path);
    ~ProcessWriter();

    bool is_writeable() noexcept;

    void write(const std::string& data);
    void write_cache(const std::string& date_, const std::string end = "\n");
};

// 数据对齐类
class AligmentProcess {
    using reader_ptr = std::shared_ptr<ProcessReader>;
    using writer_ptr = std::shared_ptr<ProcessWriter>;

private:
    std::string __init_date;
    std::map<std::string, reader_ptr> __read_map;
    std::map<std::string, writer_ptr> __write_map;

    std::string _get_file_name(const std::string& path);

public:
    AligmentProcess(std::vector<std::string> files,
                    const std::string& out_dir,
                    const std::string& date_slash = "-",
                    int32_t skip_rows             = 1,
                    const char& delimiter         = ',');
    ~AligmentProcess() {};

    void process();
};
