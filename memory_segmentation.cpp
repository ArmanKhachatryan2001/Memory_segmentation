#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
#include <unordered_map>
#include "type_sizeof_and_appearance.h"
#include "address_adjuster.h"
#include "memory_prototype.h"

int main(int argc, char** argv) {

    if (argc == 2) {
        int i = 0;
        std::ifstream file;
        file.open(argv[1]);
        if (file.is_open()) {
            std::string str = "";
            while (!file.eof()) {
                std::getline(file, str);
                if (str != "" && str[0] != '#' && str[0] != '/') {
                    code[i++] = str;
                }
            }
            ran_code(code, i);
        } else {
            std::cout << "Error opening file" << '\n';
        }
    } else {
        std::cout << "\n\n";
        std::cout << "Try this way" << '\n';
        std::cout << "Compile your program." << '\n';
        std::cout << "g++ user_file_name.cpp" << '\n';
        std::cout << "g++ memory_segmentation.cpp" << '\n';
        std::cout << "./a.out user_file_name.cpp" << "\n\n\n";
    }
    return 0;
}
