DATA::DATA()
{
    style[i++] = "*/*";
    style[i++] = "        \033[4;32mDATA---------------------------------------------------------START\033[0m\n";
}
void DATA::print()
{
    address[index++] = "\033[4;32mDATA-----------------------------------------------------------END\033[0m\n";
    RAM_print(style, address, name, value, area);
}



bool DATA::search_name(std::string str) // veradardznum e 1 ete ka
{
    for (auto& it : name) {
        if (it.second == str) {
            return true;
        }
    }
    return false;
}


void DATA::attribute(std::string str)
{
    // faind kanem ete chka BSS  nu man galu texy
    //std::cout << str << " "  << str.size() << " ";
    //exit(0);
    std::vector<std::string> line;
    std::string str_bss = str;
    std::stringstream s(str);
    while (s >> str) {
        line.push_back(str);
    }
    int size = line.size();
    line[size-1].pop_back(); // es verjin ';' -i hamar
    // heapum petqa unenanq change_value function vorpessi poxenq arjeqy
    if (line[0][0] == '*') {
        if (line[0][1] == '*') {
            line[0].erase(0, 2);
        } else {
            line[0].erase(0, 1);
        }
        if (!search_name(line[0])) {
            M._B->attribute(str_bss);
            return;
        }
        line[0] = "&" + line[0];
        line[0] = find_address(return_address(line[0]));
        line[0] = return_name(line[0]); // orinak *P P n vortexe cuyc talis num kam arr[0] ev = 8
    }
    if (line[2].find("0x") != -1) {
        data_change_value(line[0], line[2]);
    }
    if (line[2] == "new") {
        M._H->heap_allocate_space(line[0] ,line[3]);
        data_change_value(line[0], M._H->temprory_address, 1);
        return;
        //std::cout << M._H->heap_allocate_space(line[0] ,line[3]);
    } else if (line[2][0] == 39 || (line[2][0] >= '0' && line[2][0] <= '9')) {
        data_change_value(line[0], install_string(line[2]));
        return;
    } else if (std::isalpha(line[2][0])) {
        if (line[2] == "nullptr" || line[2] == "NULL") {           // es erb p = nullptr
            data_change_value(line[0], install_string(line[2]), 1);
            return;
        }
        std::string tmp = return_address(line[2], 1);// ete 1 apa value ete che address
        //std::cout << tmp;
        //exit(0);
        if (tmp == "[       ]") {
            tmp = M._D->return_address(line[2], 1);
            if (tmp == "[       ]") {
                tmp = M._S->return_address(line[2], 1);
            }
        }
        line[2] = tmp;
        //std::cout << " " << line[2];////////
        if (line[2] == tmp_nullptr || line[2] == tmp_null) { // es erbvor klini int* p = nullptr
            data_change_value(line[0], line[2], 1);
            return;
        }

        if (line[2].find("0x") != -1) { // es erb vor valuen klini hasce
            data_change_value(line[0], line[2], 1);
        } else {
            data_change_value(line[0], install_string(line[2])); // esel tt = 88
        }
        return;

    } else if (line[2][0] == '&') {
        line[2].erase(0, 1);
        line[2] = return_address(line[2]);// ete 1 apa value ete che address
        data_change_value(line[0], line[2], 1); // 1-y nra hamara vor henc et hascein veragri
    } else if (line[2][0] == '*') {
        if (line[2][1] == '*') {
            line[2].erase(0, 2);
        } else {
            line[2].erase(0, 1);
        }
        line[2] = return_address(line[2], 1);
        if (line[2].find("0x") == -1) {
            data_change_value(line[0], line[2]); // esel tt = 88
        } else {
            //std::cout << line[2];
            line[2] = return_recursive_value(find_address(line[2]));
            data_change_value(line[0], line[2]); 
        }
    }
    if (line[2].find("0x") != -1) {
        //std::cout << line[0] << " "  <<  line[2]  << "R" <<'\n';
        M._B->bss_change_value(line[0], line[2], 1);
        //std::cout << 111;
        //exit(0);
    }
}

std::string DATA::return_name(std::string str) // poxancum em anuny *P vor veradardznie  arr[0]
{
    //std::cout << str << "S";
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
    s = M._B->return_name(str);
    if (s != "") {
        return s;
    }
    s = M._S->return_name(str);
    if (s != "") {
        return s;
    }
    //exit(0);
    // bss
    return "";

}

std::string DATA::return_recursive_value(std::string str)//recursive mana galis minjev hasni arjeqi      address poxancum enq inqy gtnum veradardznum e arjeqy
{
    //static int i = 0;
    //++i;
    //if (i == 2) {
      // }
    //std::cout << str;
    //exit(0);
    //static std::string temp = str;
    if (str[0] == '[') {
        str = find_address(str);
    }
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
    if (M._B->find_address_bool(str)) {
        s = M._B->return_recursive_value(str);
        if (s != "") {
            return s;
        }
    }
    s = M._S->return_recursive_value(str);
    if (s != "") {
        return s;
    }
    return "";
}


bool DATA::data_change_address_for_value(std::string str_name, std::string str_value)
{
    //std::cout << str_name << " ?"  << str_value << str_name.size() << '\n';
    static bool flag = true; // esi static vorovhetev recursiva
    
    if (!(str_value[0] == 39 || (str_value[0] >= '0' && str_value[0] <= '9'))) {
        //kkanchenq vor imananq arjeqy  ete mijiny hace a kam & *
    }

    //std::cout << str_name << " ";
    for (auto& it : address) {
        if (it.second == str_name) {
            if (value[it.first].find("0x") == -1) {
                flag = false;
                value[it.first] = str_value;
            } else {
                str_name = find_address(value[it.first]);
                data_change_address_for_value(str_name, str_value);
            }
            return 1;
        }
    }
    if (flag) {
        //std::cout << "PPP";
        if (M._H->change_value(str_name, str_value)) {
            return 1;
        }
        if (M._B->bss_change_value(str_name, str_value)) {
            //std::cout << str_name << " "<< str_value << "AAA1"<<'\n';
            return 1;
        }
        if (M._S->stack_change_value(str_name, str_value)) {
            return 1;
        }
        //kkanchenq heap ic -----------------------------------------------------------------
    }
    flag = true;
    return 0;
}

bool DATA::data_change_value(std::string str_name, std::string str_value, bool flag) // orinak heap ic poxum enq garbijov
{
    //auto itr = value.begin();
    //static int jj = 0;
    //++jj;
    //if (jj == 2) {
    //std::cout << " " << str_name <<  " "  << str_value;
    //exit(0);
    //}
    if (str_name.find("0x") != -1) {
        str_name = return_name(str_name);
    }
    for (auto& it : name) {
        if (it.second == str_name) {
            if (flag || value[it.first].find("0x") == -1) {
                //std::cout << 1222;
                if (!flag && str_value.find("0x") != -1) {
                    str_value = return_recursive_value(str_value);
                }
                //std::cout << str_value <<  " " << 11 <<  " " << flag << " " << value[it.first];
                //exit(0);
                value[it.first] = str_value;
            } else {
                str_name = find_address(value[it.first]);
                data_change_address_for_value(str_name, str_value);
            }
            return 1;
        }
        //++itr;
    }

    //exit(0);
    // bss---------------------------------------
    if (M._B->bss_change_value(str_name, str_value, flag)) {
           // std::cout << str_name << " "<< str_value << "AAA2"<<'\n';
        return 1;
    }
    if (M._H->change_value(str_name, str_value)) {
        return 1;
    }
    if (M._S->stack_change_value(str_name, str_value, flag)) {
        return 1;
    }
    return 0;
}





int DATA::data_give_value(std::string str) // nuyny BSS
{
    //std::cout << str;
    static bool b = true;
    if (b) {
        address[index-1] = "0xD0000010";
    }
    std::vector<std::string> line;
    std::stringstream tmp(str);
    std::string t = "";
    while (tmp >> t) {
        line.push_back(t);
    }
    int size = line.size();
    line[size-1].pop_back();
    std::map<int, std::string> mp;
    if (size == 2) {
        M._B->bss_give_value(str);
        return 0;
    } else if (line[3] == "0" || line[3] == "nullptr" || line[3] == "NULL"){
        M._B->bss_give_value(str);
        return 0;
    }
    for (int i = 0; i < size; ++i) {
        t = "[          ]";
        if (i == 0) {
            mp = type_sizeof_and_appearance(line[i]);
            auto it = mp.begin();
            address[index] = address_adjuster(address[index-1], it->first);// kara inchvor mi tex index-1 y tox tpi
            area[index] = it->second;
        }
        if (i == 1) {
            name[index] = line[i];
        }
        if (i == 2) {
            //value[index] = garbij;
            continue;////// =, +=, -= ..
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
                    data_change_value(line[1], line[3]); // esel tt = 88
                } else {
                    line[3] = return_recursive_value(find_address(line[3]));
                    data_change_value(line[1], line[3]); 
                    ++index;
                }
                return 0;
            }
            if (line[3][0] == 39 || (line[3][0] >= '0' && line[3][0] <= '9')) {
                t = install_string(line[3]);
                //std::cout << t << " ";
            } else {
                if (line[3][0] != '[') {
                    line[3] = return_address(line[3], 1);
                }
                if (line[3].size() > 12 && line[3][0] == '[') {// es en depqna erb referencic uzum en arjeqy
                    int x = line[3].find("0x");
                    line[3] = line[3].substr(x, 10);
                    line[3] = return_value(line[3]);
                }
                t = install_string(line[3]);
            }
        }
    }
    if (t != "[          ]") {
        value[index] = t;
            //t = install_string(line[3]);
    }
    ++index;
    if (b) {
        address.erase(99);
        b = false;
    }
    return 0;
}

std::string DATA::return_value(const std::string& str) // poxancum enq address
{
    std::string ss = M._B->return_value(str);
    if (ss != "") {
        return ss;
    }
    int index = 0;
    for (auto& it : address) {
        if (it.second == str) {
            index = it.first;
            break;
        }
    }
    if (index != 0) {
        return value[index];
    }
    ss = "[";
    return ss + "\033[3;33m" + str + "\033[0m" + "]"; ;
}

int DATA::data_pointer(std::string str)
{
    static bool b = true;
    if (index == 100) {
        address[index-1] = "0xD0000010";
    }
    std::vector<std::string> line;
    std::stringstream tmp(str);
    std::string t = "";
    while (tmp >> t) {
        line.push_back(t);
    }
    int size = line.size();
    line[size-1].pop_back();
    if (size == 2) {
        M._B->bss_pointer(str);
        return 0;
    } else if (line[3] == "nullptr" || line[3] == "NULL") {
        M._B->bss_pointer(str);
        return 0;
    }
    std::map<int, std::string> type_size = type_sizeof_and_appearance(line[0]);
    auto it = type_size.begin();
    address[index] = address_adjuster(address[index - 1], it->first);
    if (line[3][0] == '"') {
        int i = 4;
        while (line.size() > i) {
            line[3] += " ";
            line[3] += line[i++];
        }
        line[3].pop_back();
        line[3].erase(0, 1);
        value[index] = M._R->stack_call_char_pointer(line[1], line[3]);/////////////////////////////////////////
        } else {
            if (line[3][0] == '*') {
                line[3].erase(0, 1);
            }

            value[index] = return_address(line[3]);
        }
    area[index] = it->second;
    name[index] = line[1];
    ++index;
    if (b) {
        address.erase(99);
        b = false;
    }
    return 0;
}


int DATA::data_lvalue_referenc(std::string str)
{
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
    t = return_address(line[3]);
    if (t[0] != '[') {
        line[3] = "[";
       value[index] = line[3] +  "\033[3;33m" + t + "\033[0m" + "]";
    } else {
        value[index] = t;
    }
    //value[index] = return_address(line[3]);
    area[index] = it->second;
    ++index;
    return 0;
}

std::string DATA::return_address(std::string str, int arg)
{

    std::string s = "[";
    s = M._B->return_address(str, arg);
    if (s != "[       ]") {
       return s;
    }
    //std::cout << s << " " << str << " " << arg << '\n';
    s = "[";
    char point = ' ';
    if (str[0] == '&') {
        point = '&';
        str.erase(0, 1);
    }
    int tmp = 0;
    auto i_ref = area.begin();
    auto i_ret = value.begin();
    for (auto& it : name) {
        if (it.second == str) {
            if((i_ref->second.find('&') != -1 && i_ref->second.find("&&") == -1) || (i_ref->second.find('*') != -1) && point != '&') {// && i_ref->second.find("&&") == -1))) {
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
            //std::cout << s << " " << arg << '\n';
            return s;
        }
        if (i_ref != area.end()) {
            ++i_ref;
        }
        if (i_ret != value.end()) {
            ++i_ret;
        }
    }
    return s + prabel + "]";
}
