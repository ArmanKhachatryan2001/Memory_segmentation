READ_ONLY::READ_ONLY()
{
    style[i++] = "*/*";
    style[i++] = "        \033[4;32mREAD_ONLY----------------------------------------------------START\033[0m\n";
}
void READ_ONLY::print()
{
    address[index++] = "\033[4;32mREAD_ONLY------------------------------------------------------END\033[0m\n";
    RAM_print(style, address, name, value, area);
}

std::string READ_ONLY::stack_call_char_pointer(std::string n, std::string str)
{
    std::string tmp_address = "[";
    static bool fl = true;
    static bool b = true;
    if (b) {
        address[index-1] = "0xA0000000";
        b = false;
    }
    bool flag = true;
    int in = index;
    address[index] = address[index-1];
    value[index] = "\033[3;33m=>\033[0m";
    name[index] = "\033[3;33m(sizeof(char) * " + std::to_string(str.size() + 1) + ")\033[0m";
    area[index] = "";
    ++index;
    int i = 0;
    for (; i < str.size(); ++i) {
        address[index] = address_adjuster(address[index-1], 1);
        if (flag) {
            tmp_address += "\033[3;33m" + address[index] + "\033[0m" + "]";
            flag = false;
        }
        std::string ss = "'";
        ss += str[i];
        ss += "'";
        value[index] = install_string(ss);
        name[index] = n + "[" + std::to_string(i) + "]";
        area[index] = "(sizeof(char) byte)";
        ++index;
    }
    address[index] = address_adjuster(address[index-1], 1);
    value[index] = R"('\0')";
    value[index] = install_string(value[index]);
    name[index] = n + "[" + std::to_string(i) + "]";
    area[index] = "(sizeof(char) byte)\n";
    ++index;
    if (fl) {
        address.erase(99);
        fl = false;
    }
    address[in] = "\033[3;33m" + n + "\033[0m";
    return tmp_address;
}

std::string READ_ONLY::return_address(std::string str, int arg)
{
    char point = ' ';
    if (str[0] == '&') {
        point = '&';
        str.erase(0, 1);
    }
    int tmp = 0;
    std::string s = "[";
    auto i_ref = area.begin();
    auto i_ret = value.begin();
    for (auto& it : name) {
        if (it.second == str) {
             if((i_ref->second.find('&') != -1 && i_ref->second.find("&&") == -1) || (i_ref->second.find('*') != -1) && point != '&') {
                s += i_ret->second;
                s.erase(1,1);
             } else {
                 if (arg == 0) {
                    s += "\033[3;33m" + address[it.first] + "\033[0m" + "]";
                 } else {
                    return i_ret -> second;
                 }
             }
            return s;
        }
        if (i_ref != area.end()) {
            ++i_ref;
        }
        if (i_ret != value.end()) {
            ++i_ret;
        }
    }
    if (s == "[") {
        s = M._H->return_address(str, arg);
        if (s != "[       ]") {
            return s;
        }
        s = M._B->return_address(str, arg);
        if (s != "[       ]") {
            return s;
        }
        s = M._D->return_address(str, arg);
        if (s != "[       ]") {
            return s;
        }
    }
    return s + prabel + "]";
}
