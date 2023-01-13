STACK::STACK()
{
    style[i++] = "\033[4;31mRAM-----------------------------------------------------------------------------START\033[0m\n";
    style[i++] = "*/*";
    style[i++] = "        \033[4;32mSTACK--------------------------------------------------------START\033[0m\n\n";
}

void STACK::print()
{
    address[index++] = "\033[1;33m------------------------------END---------------------------------\033[0m\n";
    address[index++] = "\033[4;32mSTACK----------------------------------------------------------END\033[0m\n";
    RAM_print(style, address, name, value, area);
}

void STACK::function_area(std::string str)
{
    str = prabel + str;
    style[i++] = str;
}

void STACK::attribute(std::string str)
{
    std::vector<std::string> line;
    std::stringstream s(str);
    while (s >> str) {
        line.push_back(str);
    }
    int size = line.size();
    line[size-1].pop_back();
    if (line[0][0] == '*') {
        if (line[0][1] == '*') {
            line[0].erase(0, 2);
        } else {
            line[0].erase(0, 1);
        }
        line[0] = "&" + line[0];
        line[0] = find_address(return_address(line[0]));
        line[0] = return_name(line[0]);
        std::cout << line[0] << "A\n";
    }
    if (line[2][0] == 34) {
        line[2].erase(0, 1);
        line[2].pop_back();
        std::string ret_address = M._R->stack_call_char_pointer(line[0], line[2]);
        stack_change_value(line[0], ret_address, 1);
        return;
    }
    if (line[2] == "new") {
        M._H->heap_allocate_space(line[0] ,line[3]);
        stack_change_value(line[0], M._H->temprory_address, 1);
        return;
    } else if (line[2][0] == 39 || (line[2][0] >= '0' && line[2][0] <= '9')) {
        stack_change_value(line[0], install_string(line[2]));
        return;
    } else if (std::isalpha(line[2][0])) {

        if (line[2] == "nullptr" || line[2] == "NULL") {
            stack_change_value(line[0], install_string(line[2]), 1);
            return;
        }
        line[2] = return_address(line[2], 1);
        if (line[2] == tmp_nullptr || line[2] == tmp_null) {
            stack_change_value(line[0], line[2], 1);
            return;
        }
        if (line[2].find("0x") != -1) {
            stack_change_value(line[0], line[2], 1);
        } else {
            stack_change_value(line[0], install_string(line[2]));
        }
        return;
    } else if (line[2][0] == '&') {
        line[2].erase(0, 1);
        line[2] = return_address(line[2]);
        stack_change_value(line[0], line[2], 1);
    } else if (line[2][0] == '*') {
        if (line[2][1] == '*') {
            line[2].erase(0, 2);
        } else {
            line[2].erase(0, 1);
        }
        line[2] = return_address(line[2], 1);
        if (line[2].find("0x") == -1) {
            stack_change_value(line[0], line[2]);
        } else {
            line[2] = return_recursive_value(find_address(line[2]));
            stack_change_value(line[0], line[2]); 
        }
    }
}

std::string STACK::return_name(std::string str)
{
    for (auto& it : address) {
        if(it.second == str) {
            if (value[it.first].find("0x") == -1) {
                return name[it.first];
            } else {
                return return_name(find_address(value[it.first]));
            }
        }
    }
    std::string s = M._H->return_name(str);
    if (s != "") {
        return s;
    }
    s = M._D->return_name(str);
    if (s != "") {
        return s;
    }
    return "";
}

std::string STACK::return_recursive_value(std::string str)
{
    for (auto& it : address) {
        if(it.second == str) {
            if (value[it.first].find("0x") == -1) {
                return value[it.first];
            } else {
                return return_recursive_value(find_address(value[it.first]));
            }
        }
    }
    std::string s = M._H->return_recursive_value(str);
    if (s != "") {
        return s;
    }
    s = M._D->return_recursive_value(str);
    if (s != "") {
        return s;
    }
    return "";
}

std::string STACK::return_address(std::string str, int arg)
{
    char point = ' ';
    if (str[0] == '&') {
        point = '&';
        str.erase(0, 1);
    }
    std::string s = "[";
    if (static_array_address.find(str) != static_array_address.end()) {
        s += "\033[3;33m" + static_array_address[str] + "\033[0m" + "]";
        return s;
    }
    int tmp = 0;
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
             if (arg && s.find("0x") != -1) {
                s = return_value(find_address(s));
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
        s = M._R->return_address(str, arg);
        if (s != "[       ]") {
            return s;
        }
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

std::string STACK::return_value(const std::string& str)
{
    for (auto& it : address) {
        if (it.second == str) {
            return value[it.first];
        }
    }
    std::string ss = M._D->return_value(str);
    if (ss != "") {
        return ss;
    }
    ss = "[";

    return ss + "\033[3;33m" + str + "\033[0m" + "]"; ;
}

int STACK::stack_pointer(std::string str)
{
    static bool b = true;
    if (index == 100) {
        address[index-1] = "0x00000000";
    }
    std::vector<std::string> line;
    std::stringstream tmp(str);
    std::string t = "";
    while (tmp >> t) {
        line.push_back(t);
    }
    address[index] = address_adjuster("0x00000000", 0);
    int size = line.size();
    line[size-1].pop_back();
    std::map<int, std::string> type_size = type_sizeof_and_appearance(line[0]);
    auto it = type_size.begin();
    address[index] = address_adjuster(address[index - 1], it->first);
    if (size == 2) {
        value[index] = garbij;
    } else {
        if (line[3].find('(') != -1) {

            std::string stack_call = T.return_address(line[3]);
            std::vector<std::string> vec = argument_rezolve(stack_call);
            transfer(vec, str);
            std::string value_inc = F.function_call(stack_call, SP, code);
            M._F->clear_rezolve_function();

            int cat = value_inc.find('!');
            value[index] = value_inc.substr(0, cat);
            value_inc = value_inc.erase(0, cat + 1);
            M._H->add_pointer_allocate_space(line[1], value_inc);

        } else if (line[3] == "nullptr" || line[3] == "NULL") {
            value[index] = install_string(line[3]);
        } else if (line[3][0] == '"') {
            int i = 4;
            while (line.size() > i) {
                line[3] += " ";
                line[3] += line[i++];
            }
            line[3].pop_back();
            line[3].erase(0, 1);
            value[index] = M._R->stack_call_char_pointer(line[1], line[3]);
        } else if (line[3] == "new") {
            value[index] = M._H->heap_allocate_space(line[1] ,line[4]);
        } else {
            if (line[3][0] == '*') {
                line[3].erase(0, 1);
            }
            line[3] = return_address(line[3]);
            if (line[3][0] == '[') {
                value[index] = line[3];
            } else {
                t = "[";
                t += "\033[3;33m" + line[3] + "\033[0m" + "]";
                value[index] = t;
                line[3] = t;
            }
            M._H->add_pointer_allocate_space(line[1], line[3]);
        }
    }
    area[index] = it->second;
    name[index] = line[1];
    ++index;
    if (b) {
        address.erase(99);
        b = false;
    }
    function_arguments_rezolve.clear();
    return 0;
}

int STACK::stack_lvalue_referenc(std::string str)
{
    static bool b = false;
    if (index == 100) {
        b = true;
    }
    std::vector<std::string> line;
    std::stringstream tmp(str);
    std::string t = "";
    while (tmp >> t) {
        line.push_back(t);
    }
    int size = line.size();
    line[size-1].pop_back();
    std::map<int, std::string> type_size = type_sizeof_and_appearance(line[0]);
    auto it = type_size.begin();
    address[index] = address_adjuster(address[index-1], it->first);
    name[index] = line[1];

    if (line[3].find('(') != -1) {
        std::string stack_call = T.return_address(line[3]);
        std::vector<std::string> vec = argument_rezolve(stack_call);
        transfer(vec, str);
        value[index] = F.function_call(stack_call, SP, code);
        M._F->clear_rezolve_function();
    } else {
        t = return_address(line[3]);
        if (t[0] != '[') {
            line[3] = "[";
           value[index] = line[3] +  "\033[3;33m" + t + "\033[0m" + "]";
        } else {
            value[index] = return_address(line[3]);
        }
    }
    area[index] = it->second;
    function_arguments_rezolve.clear();
    ++index;
    if (b) {
        address.erase(99);
        b = false;
    }
    return 0;
}

int STACK::stack_give_value(std::string str)
{
    static bool b = true;
    std::vector<std::string> line;
    std::stringstream tmp(str);
    std::string t = "";
    while (tmp >> t) {
        if (t == "=") {
            int flag = std::count(line[line.size()-1].begin(), line[line.size()-1].end(), '[');
            if (flag == 1) {
                static_array(str);
                return 0;
            } else if (flag == 2) {
                static_matrix(str);
                return 0;
            }
        }
        line.push_back(t);
    }
    int size = line.size();
    line[size-1].pop_back();
    std::map<int, std::string> mp;
    if (size == 2) {
        for (int i = 0; i < size; ++i) {
            if (i == 0) {
                mp = type_sizeof_and_appearance(line[i]);
                auto it = mp.begin();
                address[index] = address_adjuster(address[index-1], it->first);
                area[index] = it->second;
            }
            if (i == 1) {
                name[index] = line[i];
            }
        }
        value[index] = garbij;
    } else {
         for (int i = 0; i < size; ++i) {
            std::string tm = "[          ]";
            if (i == 0) {
                mp = type_sizeof_and_appearance(line[i]);
                auto it = mp.begin();
                address[index] = address_adjuster(address[index-1], it->first);
                area[index] = it->second;
            }
            if (i == 1) {
                name[index] = line[i];
            }
            if (i == 2) {
                continue;
            }
            if (i == 3) {
                if (line[3].find('(') != -1) {
                    std::string stack_call = T.return_address(line[3]);
                    std::vector<std::string> vec = argument_rezolve(stack_call);
                    transfer(vec, str);
                    line[3] = F.function_call(stack_call, SP, code);
                    M._F->clear_rezolve_function();
                }
                if (line[3][0] == '*') {
                    if (line[3][1] == '*') {
                        line[3].erase(0, 2);
                    } else {
                        line[3].erase(0, 1);
                    }
                    line[3] = return_address(line[3], 1);
                    if (line[3].find("0x") == -1) {
                        stack_change_value(line[1], line[3]);
                    } else {
                        line[3] = return_recursive_value(find_address(line[3]));
                        stack_change_value(line[1], line[3]);
                        ++index;
                    }
                    return 0;
                }
                if (line[3][0] == 39 || (line[3][0] >= '0' && line[3][0] <= '9')) {
                    tm = install_string(line[3]);
                } else {
                    if (line[3][0] != '[') {
                        line[3] = return_address(line[3], 1);
                    }
                    if (line[3].size() > 12 && line[3][0] == '[') {
                        int x = line[3].find("0x");
                        if (x != -1) {
                            line[3] = line[3].substr(x, 10);
                            line[3] = return_value(line[3]);
                        }
                    }
                    tm = install_string(line[3]);
                }
             }
             if (tm != "[          ]") {
                value[index] = tm;
             }
         }
    }
    ++index;
    if (b) {
        address.erase(99);
        b = false;
    }
    function_arguments_rezolve.clear();
    return 0;
}

void STACK::transfer(std::vector<std::string>& vec, std::string str)
{
    int f1 = str.find('(');
    int f2 = str.find(')');
    std::string str1 = str.substr(f1+1, (f2-f1));
    if (str1 == ")") {
        return;
    }
    std::vector<std::string> arg;
    std::stringstream s(str1);
    while (s >> str1) {
        if (str1[str1.size()-1] == ',' || str1 == "," || str1[str1.size() - 1] == ')') {
            if (str1 != ",") {
                str1.pop_back();
                arg.push_back(str1);
            }
        }
    }
    int k = 0;
    for (int i = 0; i < arg.size(); ++i) {
        if (vec[i].find('*') != -1) {
            function_arguments_rezolve.push_back(return_address(arg[i]));
        } else if (vec[i].find('&') != -1 && vec[i].find("&&") == -1) {
            function_arguments_rezolve.push_back(return_address(arg[i]));
        } else if(vec[i].find('[') != -1){
            if (std::count(vec[i].begin(), vec[i].end(), '[') == 1) {
                std::string return_index = arg[i] + "[0]";
                function_arguments_rezolve.push_back(return_address(return_index));
            } else {
                std::string return_index = arg[i] + "[0][0]";
                function_arguments_rezolve.push_back(return_address(return_index));
            }
        } else {
            if (arg[i][0] == 39 || (arg[i][0] >= '0' && arg[i][0] <= '9')) {
                function_arguments_rezolve.push_back(install_string(arg[i]));
            } else {
                function_arguments_rezolve.push_back(return_address(arg[i], 1));
            }
        }
    }
    std::string tmp3;
    std::string tmp4;
    for (int i = arg.size(); i < vec.size(); ++i) {
        std::stringstream ss(vec[i]);
        while (ss >> tmp3);
        function_arguments_rezolve.push_back(install_string(tmp3));
    }
}

std::vector<std::string> STACK::argument_rezolve(std::string str)
{
    std::vector<std::string> vec;
    int f1 = str.find('(');
    int f2 = str.find(')');
    std::string str1 = str.substr(f1 + 1, (f2 - f1));
    std::string tmp = "";
    std::stringstream s(str1);
    while (s >> str1) {
        if (str1[str1.size()-1] == ',' || str1 == "," || str1[str1.size() - 1] == ')') {
            if (str1 != ",") {
                str1.pop_back();
                tmp += str1;
                vec.push_back(tmp);
                tmp = "";
                str1 = "";
            }
        }
        if (str1 != "" && str1 != ",") {
            tmp += str1;
            tmp += " ";
        }
    }
    std::string first1 = "";
    std::string second1 = "";
    for (int i = 0; i < vec.size(); ++i) {
        std::stringstream P(vec[i]);
        int k = 0;
        bool b = true;
        while (P >> first1) {
            if (b && k == 1) {
                M._F->rezolve_arguments[first1] = "";
                M._F->vector_rezolv_arguments.push_back(first1);
                second1 = first1;
                b = false;
            } else if (k == 3){
                if (first1[0] >= '0' && first1[0] <= '9' || first1[0] == 39) {
                    M._F->rezolve_arguments[second1] = first1;
                } else {
                    first1 = M._D->return_address(first1);
                    first1 = find_address(first1);
                    first1 = return_value(first1);
                    first1 = cut_from_string(first1);
                    M._F->rezolve_arguments[second1] = first1;
                }
            }
            ++k;
        }
    }
    return vec;
}

bool STACK::stack_change_address_for_value(std::string str_name, std::string str_value)
{
    static bool flag = true;
    for (auto& it : address) {
        if (it.second == str_name) {
            if (value[it.first].find("0x") == -1) {
                flag = false;
                value[it.first] = str_value;
            } else {
                str_name = find_address(value[it.first]);
                stack_change_address_for_value(str_name, str_value);
            }
            return 0;
        }
    }
    if (flag) {
        if (M._H->change_value(str_name, str_value)) {
            return 1;
        }
        if (M._D->data_change_value(str_name, str_value)) {
            return 1;
        }
    }
    flag = true;
    return 1;
}

bool STACK::stack_change_value(std::string str_name, std::string str_value, bool flag)
{
    for (auto& it : name) {
        if (it.second == str_name) {
            if (flag || value[it.first].find("0x") == -1) {
                value[it.first] = str_value;
            } else {
                str_name = find_address(value[it.first]);
                stack_change_address_for_value(str_name, str_value);
            }
            return 1;
        }
    }
    if (M._H->change_value(str_name, str_value)) {
        return 1;
    }
    if (M._D->data_change_value(str_name, str_value, flag)) {
        return 1;
    }
    return 0;
}

void STACK::static_matrix(std::string str)
{
    static bool b = false;
    if (index == 100) {
        b = true;
        address[index-1] = "0x00000000";
    }
    std::string tmp = "";
    std::stringstream s(str);
    std::vector<std::string> line;
    bool flag = false;
    while(s >> str) {
        if (flag || str[0] == '{') {
            flag = true;
            tmp += str;
        } else {
            line.push_back(str);
        }
    }
    line.push_back(tmp);
    int cut = line[1].find('[');
    int cut1 = 0;
    int name_cut = cut;
    int row = 0;
    int column = 0;
    int x = 2;
    do {
        cut1 = cut;
        while (line[1][cut] != ']') {
            ++cut;
        }
        tmp = line[1].substr(cut1 + 1, (cut - cut1) -1);
        if (tmp == "") {
            row = 0;
        } else {
            if (x == 2) {
                row = array_element_count(tmp);
            } else {
                column = array_element_count(tmp);
            }
        }
        ++cut;
        --x;
    } while (x > 0);
    std::vector<std::string> vec = return_matrix_elements(line[3], column, row);
    if (!row) {
        ++row;
        for (int i = 0; i < vec.size(); ++i) {
            if (vec[i] == " ") {
               ++row;
            }
        }
    }
    std::map<int, std::string> mp = type_sizeof_and_appearance(line[0]);
    line[1] = line[1].substr(0, name_cut);
    auto it = mp.begin();

    int tmp_row = 0;
    int j = 0;
    do {
    int temprory = index;
    address[index] = address[index-1];
    value[index] = "\033[3;33m=>\033[0m";
    name[index] = "\033[3;33m" + line[0] + "\033[0m";
    name[index] = "\033[3;33m(sizeof(" + name[index] + "\033[3;33m" + ") * " + std::to_string(column) + ")\033[0m";
    area[index] = "";
    ++index;
    bool flag_array = true;
    for (int i = 0; i < column; ++i) {
       if (vec[j] == " ") {
           ++j;
       }
       address[index] = address_adjuster(address[index-1], it->first);
       value[index] = install_string(vec[j++]);
       name[index] = line[1] + "[" + std::to_string(tmp_row) + "]" + "[" + std::to_string(i) + "]";
       if (flag_array) {
          static_array_address[line[1]] = address[index];
          flag_array = false;
       }
       area[index] = it->second;
       ++index;
    }
    address[temprory] = "\033[3;33m" + line[1] + "[" + std::to_string(tmp_row) + "]" + "\033[0m";
    ++tmp_row;
    } while (tmp_row < row);

    if (b) {
        address.erase(99);
        b = false;
    }
}

std::vector<std::string> STACK::return_matrix_elements(const std::string& str, int column, int row)
{
    std::vector<std::string> vec;
    std::string tmp = "";
    int size = str.size()-2;
    int num = column;
    int white_space = 1;
    for (int i = 1; i < size; ++i) {
        if (str[i] == '{') {
            if (num < column) {
                ++white_space;
                vec.push_back(" ");
                num = column;
            }
            ++i;
            for (; str[i] != '}'; ++i) {
                if (str[i] == ',') {
                    vec.push_back(tmp);
                    --num;
                    tmp = "";
                    continue;
                }
                tmp += str[i];
            }
            if (tmp != "") {
                vec.push_back(tmp);
                --num;
                tmp = "";
            }
            for (int k = 0; k < num; ++k) {
                vec.push_back("0");
            }
            num = column;
            ++i;
            while (i < size && (str[i] == ',' || str[i] == '}')) {
                ++i;
            }
            if (i < size) {
                ++white_space;
                vec.push_back(" ");
            }
            --i;
        } else {
            if (str[i] == ',') {
                vec.push_back(tmp);
                --num;
                if (!num) {
                    num = column;
                ++white_space;
                    vec.push_back(" ");
                }
                tmp = "";
                continue;
            }
            tmp += str[i];
        }
    }

    if (tmp != "") {
        --num;
       vec.push_back(tmp);
    }
    if (num && num < column) {
        for (int i = 0; i < num; ++i) {
            vec.push_back("0");
        }
    }
        while (white_space < row) {
        vec.push_back(" ");
        for (int i = 0; i < column; ++i) {
            vec.push_back("0");
        }
        ++white_space;
    }
    return vec;
}

int STACK::array_element_count(std::string tmp)
{
     if (tmp[0] >= '0' && tmp[0] <= '9') {
         return std::stoi(tmp);
     } else if (isalpha(tmp[0])) {
         tmp = return_address(tmp);
         tmp = find_address(tmp);
         tmp = return_value(tmp);
         tmp = cut_from_string(tmp);
         return std::stoi(tmp);
     }
     return 0;
}

void STACK::static_array(std::string str)
{
    static bool b = false;
    if (index == 100) {
        b = true;
        address[index-1] = "0x00000000";
    }
    std::string tmp = "";
    std::stringstream s(str);
    std::vector<std::string> line;
    bool flag = false;
    while(s >> str) {
        if (flag || str[0] == '{') {
            flag = true;
            tmp += str;
        } else {
            line.push_back(str);
        }
    }
    std::vector<std::string> vec = return_array_elements(tmp);
    std::map<int, std::string> mp = type_sizeof_and_appearance(line[0]);
    auto it = mp.begin();
    int x = 0;
    int y = 0;
    int count = 0;
    x = line[1].find('[');
    y = line[1].find(']');
    tmp = line[1].substr(x + 1, (y - x) -1);
    if (tmp == "") {
        count = vec.size();
    } else {
        count = array_element_count(tmp);
    }
    while (line[1].size() > x) {
        line[1].pop_back();
    }
    int temprory = index;
    address[index] = address[index-1];
    value[index] = "\033[3;33m=>\033[0m";
    name[index] = "\033[3;33m" + line[0] + "\033[0m";
    name[index] = "\033[3;33m(sizeof(" + name[index] + "\033[3;33m" + ") * " + std::to_string(count) + ")\033[0m";
    area[index] = "";
    ++index;
    int j = 0;
    int size = vec.size();
    bool flag_array = true;
    for (int i = 0; i < count; ++i) {
          address[index] = address_adjuster(address[index-1], it->first);
          if (j < size) {
             value[index] = install_string(vec[j++]);
          } else {
            value[index] = garbij;
          }
          name[index] = line[1] + "[" + std::to_string(i) + "]";
          if (flag_array) {
             static_array_address[line[1]] = address[index];
             flag_array = false;
          }
          area[index] = it->second;
          ++index;
    }
    address[temprory] = "\033[3;33m" + line[1] + "\033[0m";
    if (b) {
        address.erase(99);
        b = false;
    }
}

std::vector<std::string> STACK::return_array_elements(std::string& str)
{
    std::vector<std::string> vec;
    std::string tmp = "";
    str.pop_back();
    int size = str.size();
    for (int i = 0; i < size; ++i) {
        if (str[i] == '{' || str[i] == '}' || str[i] == ' ') {
            continue;
        }
        if (str[i] == ',') {
            vec.push_back(tmp);
            tmp = "";
            continue;
        }
        tmp += str[i];
     }
     if (tmp != "") {
         vec.push_back(tmp);
     }
     return vec;
}
