#include <rebels/utilities/csv.h>
#include <string>
#include <vector>
#include <iostream>

void print_vector(std::vector<std::string> v) {
    std::vector<std::string>::iterator vit;
    for (vit = v.begin(); vit != v.end(); vit++) {
        std::cout << *vit << " ";
    }
    std::cout << std::endl;
}

int main() {
    std::string md_path = "./data/train/md/000661.SZ/md_20200525.csv";
    std::vector<std::string> row;

    CSV csv{md_path};
    // skip the first header
    csv.skip(1);

    // read the first row
    csv.next(row);

    print_vector(row);
    std::cout << "-----------------------------------------------" << std::endl;

    // read the second row
    row.clear();
    csv.next(row);
    print_vector(row);
    std::cout << "-----------------------------------------------" << std::endl;

    // read the third row
    row.clear();
    csv.next(row);
    print_vector(row);
    std::cout << "-----------------------------------------------" << std::endl;

    // read the third row
    row.clear();
    csv.next(row);
    print_vector(row);

    csv.closeFile();

    return 0;
}
