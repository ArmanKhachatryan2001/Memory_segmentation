std::string FUNCTION::return_address(std::string str, int arg)
{
    char point = ' ';
    std::string str4 = str;
    if (str[0] == '&') {
        point = '&';
        arg = 0;
        str.erase(0, 1);
    }
    int tmp = 0;
    int search = 0;
    for (auto& ref : function_area) {
        if (ref.second == str) {
            search = ref.first;
            break;
        }
    }
    std::string s = "[";
    std::string ssr = "";
    if (search) {
        if((area[search].find('&') != -1 && area[search].find("&&") == -1) || (area[search].find('*') != -1) && point != '&') {
            s += value[search];
            s.erase(1,1);
            if (arg == 2 && s.find("0x") != -1) {
                s = find_address(s);
                s = return_recursive_value(s);
            }
        } else {
            if (arg == 0) {
               s += "\033[3;33m" + address[search] + "\033[0m" + "]";
            } else {
               return value[search];
            }
        }
        return s;
    } else if (point == '&') {
        if (temprory_address.find(str) != temprory_address.end()) {
            ssr = temprory_address[str];
        }
        if (ssr != "") {
            ssr.erase(0,1);
            ssr.pop_back();
            std::string ssr2 = "[";
            ssr2 += "\033[3;33m" + ssr + "\033[0m" + "]";
            return ssr2;
        }
    } else if (rezolve_arguments.find(str) != rezolve_arguments.end()) {
        ssr = rezolve_arguments[str];
        return ssr;
    } else if (std::count(str.begin(), str.end(), '[') == 2) {
        for (auto& it : name) {
            if (it.second == str) {
                return value[it.first];
                break;
            }
        }
    }
    if (s == "[") {
        s = M._H->return_address(str4, arg);
        if (s != "[       ]") {
            return s;
        }
        s = M._B->return_address(str4, arg);
        if (s != "[       ]") {
            return s;
        }
        s = M._D->return_address(str4, arg);
        if (s != "[       ]") {
            return s;
        }
    }
    return s + prabel + "]";
}

void FUNCTION::function_stack_lvalue_referenc(std::string str)
{
    static bool b = false;
    if (index == 100) {
        b = true;
        address[index-1] = "0x00000000";
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
    value[index] = return_address(line[3]);
    area[index] = it->second;
    function_area[index] = name[index];
    ++index;
    if (b) {
        address.erase(99);
    }
    return;
}

void FUNCTION::function_stack_pointer(std::string str)
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
        if (line[3] == "nullptr" || line[3] == "NULL") {
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
        } else if (line[3][0] == '*') {
            line[3].erase(0, 1);
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
        } else {
            value[index] = return_address(line[3], 1);
        }
    }
    area[index] = it->second;
    name[index] = line[1];
    function_area[index] = name[index];
    ++index;
    if (b) {
        address.erase(99);
        b = false;
    }
    return;
}

std::string FUNCTION::function_call(std::string& str, int SP, std::map<int, std::string>& code)
{
    call_and_implementation();
    bool if_flag = false;
    std::stringstream s(str);
    std::string return_type = "";
    std::string temprory = "";
    int table = 0;
    s >> table;
    s >> address[index]; // 0xF00000
    s >> return_type;
    std::string tt = "";
    if (!s.eof()) {
        std::getline(s, tt);
        temprory += tt;
    }
    value[index] = "\033[9;32m" + function_prototype(return_type + temprory) + "\033[0m";
    area[index] = "";
    name[index] = "";
    prev_address = address[index];
    ++index;
    while (++table) {
        std::string tmp = "";
        std::stringstream clear_prabel(code[table]);
        clear_prabel >> tmp;
        if (tmp != "{" && tmp != "}" && tmp[0] != '/') {
            int flag = type_and_action.type_action(tmp);
            if (flag == -1) {
                attribute(code[table]);
            } else if (flag == 1) {
                function_stack_give_value(code[table]);
            } else if (flag == 2) {
                function_stack_lvalue_referenc(code[table]);
            } else if (flag == 3) {
                function_stack_pointer(code[table]);
            } else if (flag == 4) {
                H.delete_allocate_space(code[table]);
            } else if (flag == 5) {
                if_flag = true;
                break;
            } else if (flag == 6) {
                function_stack_pointer(code[table]);
            }
        }
    }
    address[index] = "------------------------------END---------------------------------";
    value[index] = "";
    name[index] = "";
    area[index] = "\n";
    ++index;
    if (if_flag) {
        if (code[table].size() <= 6) {
            function_area.clear();
            return "";
        } else {
            std::stringstream s(code[table]);
            s >> temprory;
            s >> temprory;
            temprory.pop_back();
            if (temprory[0] == 39 || (temprory[0] >= '0' && temprory[0] <= '9')) {
                function_area.clear();
                return temprory;
            }
            if (temprory[0] == '*') {
                temprory.erase(0, 1);
                temprory = return_address(temprory);
                temprory = return_recursive_value(find_address(temprory));
                function_area.clear();
                return temprory;
            }
            if (temprory[0] == '&') {
                function_area.clear();
                return return_address(temprory);
            }
            return return_address(temprory, 2);
        }
    }
    return "";
}

bool FUNCTION::search_name(const std::string& str)
{
    for (auto& it : name) {
        if (str == it.second) {
            return true;
        }
    }
    return false;
}


void FUNCTION::attribute(std::string str)
{
    bool flag = true;
    std::vector<std::string> line;
    std::stringstream s(str);
    while (s >> str) {
        line.push_back(str);
    }
    int size = line.size();
    line[size-1].pop_back();
    std::string temp = line[0];
    if (line[0][0] == '*') {
        if (line[0][1] == '*') {
            line[0].erase(0, 2);
        } else {
            line[0].erase(0, 1);
        }
        flag = search_name(line[0]);
        if (flag) {
            line[0] = "&" + line[0];
            line[0] = find_address(return_address(line[0]));
            line[0] = return_name(line[0]);
        } else {
            line[0] = temp;
        }
    } else {
        flag = search_name(line[0]);
    }
    if (line[2] == "new") {
        if (!flag) {
            M._D->attribute(line[0] + " = " + line[2] + " " + line[3] + ";");
        } else {
            M._H->heap_allocate_space(line[0] ,line[3]);
            change_value(line[0], M._H->temprory_address, 1);
        }
        return;
    } else if (line[2][0] == 39 || (line[2][0] >= '0' && line[2][0] <= '9')) {
        if (!flag) {
            M._D->attribute(line[0] + " = " + line[2] + ";");
        } else {
            change_value(line[0], install_string(line[2]));
        }
        return;
    } else if (std::isalpha(line[2][0])) {

        if (line[2] == "nullptr" || line[2] == "NULL") {
            change_value(line[0], install_string(line[2]), 1);
            return;
        }
        line[2] = return_address(line[2], 1);
        if (line[2].find("0x") != -1) {
            line[2] = return_recursive_value(line[2]);
        }
        if (line[2] == tmp_nullptr || line[2] == tmp_null) {
            change_value(line[0], line[2], 1);
            return;
        }

        if (line[2].find("0x") != -1) {
            if (!flag) {
                M._D->attribute(line[0] + " = " + line[2] + ";");
            } else {
                change_value(line[0], line[2], 1);
            }
        } else {
            if (!flag) {
                line[2] = cut_from_string(line[2]);
                M._D->attribute(line[0] + " = " + line[2] + ";");
            } else {
                change_value(line[0], install_string(line[2]));
            }
        }
        return;
    } else if (line[2][0] == '&') {
        line[2].erase(0, 1);
        line[2] = return_address(line[2]);
        if (!flag) {
            M._D->attribute(line[0] + " = " + line[2] + ";");
        } else {
            change_value(line[0], line[2], 1);
        }
    } else if (line[2][0] == '*') {
        if (line[2][1] == '*') {
            line[2].erase(0, 2);
        } else {
            line[2].erase(0, 1);
        }
        line[2] = return_address(line[2], 1);
        if (line[2].find("0x") == -1) {
            if (!flag) {
                M._D->attribute(line[0] + " = " + line[2] + ";");
            } else {
                change_value(line[0], line[2]);
            }
        } else {
            line[2] = return_recursive_value(find_address(line[2]));
            if (!flag) {
                M._D->attribute(line[0] + " = " + line[2] + ";");
            } else {
                change_value(line[0], line[2]);
            }
        }
    }
}
std::string FUNCTION::return_name(std::string str)
{
    std::string str1;
    for (auto& it : temprory_address) {
        str1 = find_address(it.second);
        str1.erase(0, 1);
        str1.pop_back();
        if (str1 == str) {
            if (rezolve_arguments[it.first].find("0x") != -1) {
                return return_name(find_address(rezolve_arguments[it.first]));
            }
        }
    }
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
    s = M._S->return_name(str);
    if (s != "") {
        return s;
    }
    return "";
}

std::string FUNCTION::return_recursive_value(std::string str)
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
    s = M._S->return_recursive_value(str);
    if (s != "") {
        return s;
    }
    return "";
}

bool FUNCTION::change_address_for_value(std::string str_name, std::string str_value)
{
    static bool flag = true;
    for (auto& it : address) {
        if (it.second == str_name) {
            if (value[it.first].find("0x") == -1) {
                flag = false;
                value[it.first] = str_value;
            } else {
                str_name = find_address(value[it.first]);
                change_address_for_value(str_name, str_value);
            }
            return 1;
        }
    }
    flag = true;
    if (flag) {
        if (M._H->change_value(str_name, str_value)) {
            return 1;
        }
        if (M._D->data_change_value(str_name, str_value)) {
            return 1;
        }
        if (M._S->stack_change_value(str_name, str_value)) {
            return 1;
        }
    }
    return 0;
}

bool FUNCTION::change_value(std::string str_name, std::string str_value, bool flag)
{
    auto itr = rezolve_arguments.find(str_name);
    if (itr != rezolve_arguments.end()) {
        if (itr->second.find("0x") != -1) {
            itr->second = find_address(itr->second);
            change_address_for_value(itr->second, str_value);
        } else {
            itr->second = install_string(str_value);
        }
        return 1;
    }
    if (str_name.find("0x") != -1) {
        str_name = return_name(str_name);
    }
    for (auto& it : name) {
        if (it.second == str_name) {
            if (flag || value[it.first].find("0x") == -1) {
                value[it.first] = str_value;
            } else {
                str_name = find_address(value[it.first]);
                change_address_for_value(str_name, str_value);
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
    if (M._S->stack_change_value(str_name, str_value, flag)) {
        return 1;
    }
    return 0;
}

void FUNCTION::call_and_implementation()
{
    std::string str = "0xFF011111";
    auto itr = rezolve_arguments.begin();
    for (int i = 0; i < vector_rezolv_arguments.size(); ++i) {
        if (vector_rezolv_arguments[i].size()-1 == ']') {
            int x = vector_rezolv_arguments[i].find('[');
            vector_rezolv_arguments[i] = vector_rezolv_arguments[i].substr(0, x);
        }
    }
    int i = 0;
    for (auto& it : function_arguments_rezolve) {
        if (itr != rezolve_arguments.end()) {
            if (itr->first[itr->first.size()-1] == ']') {
                int x = itr->first.find('[');
                auto& PTR = const_cast<std::string&>(itr->first);
                std::string cut = itr->first;
                cut = cut.substr(0, x);
                PTR = cut;
            }
            rezolve_arguments[vector_rezolv_arguments[i++]] = it;
            ++itr;
        }
    }
    itr = rezolve_arguments.begin();
    for (auto itr1 = itr; itr1 != rezolve_arguments.end(); ++itr1) {
        if (itr1->second[0] != '[') {
            itr1->second = install_string(itr1->second);
        }
    }
    for (auto& it : rezolve_arguments) {
        str = address_adjuster(str, 4);
        temprory_address[it.first] = install_string(str);
    }
    function_arguments_rezolve.clear();
}

void FUNCTION::clear_rezolve_function()
{
    if (!rezolve_arguments.empty()) {
        rezolve_arguments.clear();
    }
    if (!temprory_address.empty()) {
        temprory_address.clear();
    }
}
void FUNCTION::print()
{
     RAM_print(style, address, name, value, area);
}

void FUNCTION::function_stack_give_value(std::string str)
{
    std::vector<std::string> line;
    std::stringstream tmp(str);
    std::string t = "";
    while (tmp >> t) {
        if (t == "=") {
            int flag = std::count(line[line.size()-1].begin(), line[line.size()-1].end(), '[');
            if (flag == 1) {
                function_static_array(str);
                return;
            } else if (flag == 2) {
                function_static_matrix(str);
                return;
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
                function_area[index] = name[index];
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
                function_area[index] = name[index];
            }
            if (i == 2) {
                continue;
            }
            if (i == 3) {
                if (line[3][0] == '*') {
                    if (line[3][1] == '*') {
                        line[3].erase(0, 2);
                    } else {
                        line[3].erase(0, 1);
                    }
                    line[3] = return_address(line[3], 1);
                    if (line[3].find("0x") == -1) {
                        change_value(line[1], line[3]); // esel tt = 88
                    } else {
                        line[3] = return_recursive_value(find_address(line[3]));
                        change_value(line[1], line[3]);
                    }
                } else if (line[3][0] == 39 || (line[3][0] >= '0' && line[3][0] <= '9')) {
                    tm = install_string(line[3]);
                } else {
                    if (line[3][0] != '[') {
                        line[3] = return_address(line[3], 1);
                        if (line[3].find("0x") != -1) {
                            line[3] = return_recursive_value(line[3]);
                        }
                    }
                    if (line[3].size() > 12 && line[3][0] == '[') {
                        line[3] = function_return_value(find_address(line[3]));
                    }
                }
                tm = install_string(line[3]);
            }
            value[index] = tm;
        }
    }
    ++index;
    return;
}

std::string FUNCTION::function_return_value(const std::string& str)
{
    int index = 0;
    for (auto& it : address) {
        if (it.second == str) {
            index = it.first;
            break;
        }
    }
    if (rezolve_arguments.find(str) != rezolve_arguments.end()) {
        return rezolve_arguments[str];
    }
    if (index != 0) {
        return value[index];
    }
    std::string ss = M._D->return_value(str);
    if (ss != "") {
        return ss;
    }
    ss = M._S->return_value(str);
    if (ss != "") {
        return ss;
    }
    return "";
}

int FUNCTION::function_array_element_count(std::string tmp)
{
     if (tmp[0] >= '0' && tmp[0] <= '9') {
         return std::stoi(tmp);
     } else if (isalpha(tmp[0])) {
         tmp = return_address(tmp);
         if (tmp.find("0x") != -1) {
            tmp = find_address(tmp);
            tmp = function_return_value(tmp);
         }
         tmp = cut_from_string(tmp);
         return std::stoi(tmp);
     }
     return 0;
}

void FUNCTION::function_static_matrix(std::string str)
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
                row = function_array_element_count(tmp);
            } else {
                column = function_array_element_count(tmp);
            }
        }
        ++cut;
        --x;
    } while (x > 0);

    std::vector<std::string> vec = M._S->return_matrix_elements(line[3], column, row);
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
    for (int i = 0; i < column; ++i) {
       if (vec[j] == " ") {
           ++j;
       }
       address[index] = address_adjuster(address[index-1], it->first);
       value[index] = install_string(vec[j++]);
       name[index] = line[1] + "[" + std::to_string(tmp_row) + "]" + "[" + std::to_string(i) + "]";
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

void FUNCTION::function_static_array(std::string str)
{
    static bool b = true;
    if (index == 100 && b) {
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
    std::vector<std::string> vec = function_return_array_elements(tmp);
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
        if (tmp[0] >= '0' && tmp[0] <= '9') {
            count = std::stoi(tmp);
        } else if (isalpha(tmp[0])) {
            tmp = return_address(tmp);
            tmp = find_address(tmp);
            if (tmp.find("0x") != -1) {
                tmp = function_return_value(tmp);
            }
            tmp = cut_from_string(tmp);
            count = std::stoi(tmp);
        }
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
    for (int i = 0; i < count; ++i) {
          address[index] = address_adjuster(address[index-1], it->first);
          if (j < size) {
             value[index] = install_string(vec[j++]);
          } else {
            value[index] = garbij;
          }
          name[index] = line[1] + "[" + std::to_string(i) + "]";
          function_area[index] = name[index];
          area[index] = it->second;
          ++index;
    }

    address[temprory] = "\033[3;33m" + line[1] + "\033[0m";

    if (b) {
        address.erase(99);
        b = false;
    }
}

std::vector<std::string> FUNCTION::function_return_array_elements(std::string& str)
{
    std::vector<std::string> vec;
    std::string tmp = "";
    str.pop_back();
    int size = str.size();
    for (int i = 0; i < size; ++i) {
        if (str[i] == '{' || str[i] == '}') {
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


bool FUNCTION::function_stack_change_value(std::string str_name, std::string str_value)
{
    for (auto& it : name) {
        if (it.second == str_name) {
            value[it.first] = str_value;
            return 0;
        }
    }
    return 1;
}
