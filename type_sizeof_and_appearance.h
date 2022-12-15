std::map<int, std::string> type_sizeof_and_appearance(const std::string& str) {
    std::string tmp = "";
    int type_byte = 0;
    std::map<int, std::string> mp;
    mp[0] = "(sizeof(";
    mp[2] = ") byte)";
    int flag = -1;
    for (int i = str.size()-1; i >= 0; --i) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            flag = 0;
            break;
        } else if (str[i] == '*') {
            if (i > 0 && str[i-1] == '*') {
                flag = 2;
                break;
            }
            flag = 1;
            break;
        } else if (str[i] == '&') {
            if (i > 0 && str[i - 1] == '&') {
                flag = 4;
                break;
            }
            flag = 3;
            break;
        }
    }
    std::map<std::string, int> type;
    std::map<std::string, int> type1;
    std::map<std::string, int> type2;
    std::map<std::string, int> type3;
    std::map<std::string, int> type4;

    if (flag == 0) {
        type["void"] = 1;
        type["int"] = sizeof(int);
        type["char"] = sizeof(char);
        type["bool"] = sizeof(bool);
        type["short"] = sizeof(short);
        type["double"] = sizeof(double);
        type["float"] = sizeof(float);
        type["long"] = sizeof(long);
        auto it = type.find(str);
        if (it != type.end()) {
            mp[1] = it->first;
            type_byte = it->second;
        }
    } else if (flag == 1) {
        type1["void*"] = sizeof(void*);
        type1["int*"] = sizeof(int*);
        type1["char*"] = sizeof(char*);
        type1["bool*"] = sizeof(bool*);
        type1["short*"] = sizeof(short*);
        type1["double*"] = sizeof(double*);
        type1["float*"] = sizeof(float*);
        type1["long*"] = sizeof(long*);
        auto it = type1.find(str);
        if (it != type1.end()) {
            mp[1] = it->first;
            type_byte = it->second;
        }
    } else if (flag == 2) {
        type2["int**"] = sizeof(int**);
        type2["char**"] = sizeof(char**);
        type2["bool**"] = sizeof(bool**);
        type2["short**"] = sizeof(short**);
        type2["double**"] = sizeof(double**);
        type2["float**"] = sizeof(float**);
        type2["long**"] = sizeof(long**);
        auto it = type2.find(str);
        if (it != type2.end()) {
            mp[1] = it->first;
            type_byte = it->second;
        }
    } else if (flag == 3) {
        type3["int&"] = sizeof(int&);
        type3["char&"] = sizeof(char&);
        type3["bool&"] = sizeof(bool&);
        type3["short&"] = sizeof(short&);
        type3["double&"] = sizeof(double&);
        type3["float&"] = sizeof(float&);
        type3["long&"] = sizeof(long&);
        auto it = type3.find(str);
        if (it != type3.end()) {
            mp[1] = it->first;
            type_byte = it->second;
        }
    } else if (flag == 4) {
        type4["int&&"] = sizeof(int&&);
        type4["char&&"] = sizeof(char&&);
        type4["bool&&"] = sizeof(bool&&);
        type4["short&&"] = sizeof(short&&);
        type4["double&&"] = sizeof(double&&);
        type4["float&&"] = sizeof(float&&);
        type4["long&&"] = sizeof(long&&);
        auto it = type4.find(str);
        if (it != type4.end()) {
            mp[1] = it->first;
            type_byte = it->second;
        }
    }
    //int*&


    for (auto& it : mp) {
        tmp += it.second;
    }
    mp.clear();
    mp[type_byte] = tmp;
    return mp;
}

/*
 std::map<int, std::string> mp = type_sizeof_and_appearance("char*");
    auto it = mp.begin();
    std::cout << it->first << " " << it->second << '\n';
}
*/

