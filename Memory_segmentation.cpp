#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
#include <unordered_map>
#include "type_sizeof_and_appearance.h" // return map<int,string> //int-> տիպի չապսը // ստրինգ-> տեսքը
#include "address_adjuster.h" // return string
#include "Memory_prototype.h"

int main() {
    int i = 0;
    std::map<int, std::string> code;
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
    //for (auto& it : code) {
      //  std:cout << it.second << '\n';
    //prints
    foo(&S);
    foo(&H);
    foo(&T);
    foo(&B);
    foo(&R);
    foo(&D);
    std::cout <<"\033[4;31mRAM-------------------------------------------------------------------------------END\033[0m\n";
    std::cout << std::endl;
}

