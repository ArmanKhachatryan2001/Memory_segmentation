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

int main() {
    int i = 0;
    std::ifstream file;
    file.open("User_code.cpp");
    std::string str = "";
    while (!file.eof()) {
        std::getline(file, str);
        if (str != "" && str[0] != '#' && str[0] != '/') {
            code[i++] = str;
        }
    }
    ran_code(code, i);
}
