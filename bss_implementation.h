BSS::BSS()
{
    style[i++] = "*/*";
    style[i++] = "        \033[4;32mBSS----------------------------------------------------------START\033[0m\n";
    //style[i++] = "*/*";
}
void BSS::print()
{
    address[index++] = "\033[4;32mBSS------------------------------------------------------------END\033[0m\n";
    RAM_print(style, address, name, value, area);
}

void BSS::attribute(std::string str)
{
    //std::cout << str << " " << "bss" << '\n';
    // faind kanem ete chka BSS  nu man galu texy
    std::vector<std::string> line;
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
        line[0] = "&" + line[0];
        //std::cout << line[0] << "A\n";
        line[0] = find_address(M._D->return_address(line[0]));

        //std::cout << line[0] << "A\n";
        line[0] = return_name(line[0]); // orinak *P P n vortexe cuyc talis num kam arr[0] ev = 8
        //std::cout << line[0] << "A\n";
    }
    if (line[2].find("0x") != -1) {
        bss_change_value(line[0], line[2]);
    }
    if (line[2] == "new") {
        M._H->heap_allocate_space(line[0] ,line[3]);
        bss_change_value(line[0], M._H->temprory_address, 1);
        return;
        //std::cout << M._H->heap_allocate_space(line[0] ,line[3]);
    } else if (line[2][0] == 39 || (line[2][0] >= '0' && line[2][0] <= '9')) {
        bss_change_value(line[0], install_string(line[2]));
        return;
    } else if (std::isalpha(line[2][0])) {

        if (line[2] == "nullptr" || line[2] == "NULL") {           // es erb p = nullptr
            bss_change_value(line[0], install_string(line[2]), 1);
            return;
        }

        line[2] = M._D->return_address(line[2], 1);// ete 1 apa value ete che address
        //std::cout << line[2] << '\n';
        if (line[2] == tmp_nullptr || line[2] == tmp_null) { // es erbvor klini int* p = nullptr
            bss_change_value(line[0], line[2], 1);
            return;
        }

        if (line[2].find("0x") != -1) { // es erb vor valuen klini hasce
            bss_change_value(line[0], line[2], 1);
        } else {
            bss_change_value(line[0], install_string(line[2])); // esel tt = 88
        }
        return;

    } else if (line[2][0] == '&') {
        line[2].erase(0, 1);
        line[2] = M._D->return_address(line[2]);// ete 1 apa value ete che address
        bss_change_value(line[0], line[2], 1); // 1-y nra hamara vor henc et hascein veragri
    } else if (line[2][0] == '*') {
        if (line[2][1] == '*') {
            line[2].erase(0, 2);
        } else {
            line[2].erase(0, 1);
        }
        line[2] = return_address(line[2], 1);
        if (line[2].find("0x") == -1) {
            bss_change_value(line[0], line[2]); // esel tt = 88
        } else {
            line[2] = return_recursive_value(find_address(line[2]));
            bss_change_value(line[0], line[2]); 
        }
    }
}

std::string BSS::return_name(std::string str) // poxancum em anuny *P vor veradardznie  arr[0]
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
    s = M._S->return_name(str);
    if (s != "") {
        return s;
    }
    s = M._D->return_name(str);
    if (s != "") {
        return s;
    }
    return "";

}

std::string BSS::return_recursive_value(std::string str)//recursive mana galis minjev hasni arjeqi      address poxancum enq inqy gtnum veradardznum e arjeqy
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
    return "";
}


bool BSS::bss_change_address_for_value(std::string str_name, std::string str_value)
{
    //std::cout << str_name << " " << str_value   << " " << str_name.size() << '\n';
    //std::cout << 111;
    //exit(0);
    if (str_name.find("0x") != -1) {
        return 1;
    }
    static bool flag = true; // esi static vorovhetev recursiva

    //std::cout << str_name << " ";
    for (auto& it : address) {
        if (it.second == str_name) {
            if (value[it.first].find("0x") == -1) {
                flag = false;
                value[it.first] = str_value;
            } else {
                str_name = find_address(value[it.first]);
                bss_change_address_for_value(str_name, str_value);
            }
            return 1;
        }
    }
    if (flag) {
        //std::cout << "PPP";
        if (M._H->change_value(str_name, str_value)) {
            return 1;
        }
        if (M._D->data_change_value(str_name, str_value)) {
        //std::cout << str_name << str_value << '\n';
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

bool BSS::bss_change_value(std::string str_name, std::string str_value, bool flag) // orinak heap ic poxum enq garbijov
{
    //std::cout << str_name << " " << str_value << " " << flag << " \n\n";
    //exit(0);
    for (auto& it : name) {
        if (it.second == str_name) {
            //std::cout << it.second << " " << str_value << "S" << '\n' << '\n';
            if (flag || value[it.first].find("0x") == -1) {
               // std::cout << value[it.first];
               // exit(0);
                value[it.first] = str_value;
            //std::cout << it.first << "<>" << it.second  << "<>" << str_name << "<>" << str_value << '\n';
            } else {
               // std::cout << value[it.first];
               // exit(0);
                str_name = find_address(value[it.first]);
                bss_change_address_for_value(str_name, str_value);
            }
            //std::cout << value[it.first] << str_value << '\n';
            return 1;
        }
        //++itr;
    }

    // bss---------------------------------------
    if (M._H->change_value(str_name, str_value)) {
        return 1;
    }
    if (M._S->stack_change_value(str_name, str_value, flag)) {
        return 1;
    }

    if (M._D->data_change_value(str_name, str_value, flag)) {
        return 1;
    }
    return 0;
}




int BSS::bss_give_value(std::string str) // nuyny BSS
{
    static bool b = true;
    if (b) {
        address[index-1] = "0xB0000110";
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
    mp = type_sizeof_and_appearance(line[0]);
    auto it = mp.begin();

    address[index] = address_adjuster(address[index-1], it->first);
    area[index] = it->second;
    name[index] = line[1];
    value[index] = install_string("0");
    ++index;
    if (b) {
        address.erase(99);
        b = false;
    }
    return 0;
}

int BSS::bss_pointer(std::string str)
{
    static bool b = true;
    if (index == 100) {
        address[index-1] = "0xB0000110";
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
    address[index] = address_adjuster(address[index - 1], it->first);
    value[index] = NULLPTR;
    name[index] = line[1];
    area[index] = it->second;
    ++index;
    if (b) {
        address.erase(99);
        b = false;
    }

    return 0;
}
//int BSS::bss_lvalue_referenc(std::string){
  //      std::cout << "BSS lvalue" << std::endl;
   // return 0;} // չի կարա ստեղ լվալյու լինի


std::string BSS::return_address(std::string str, int arg)
{
    //std::cout << str << " " << arg <<'\n';
    //exit(0);
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
             //std::cout << s;/////////////////////////////////////////////////////////////
            return s;
        }
        if (i_ref != area.end()) {
            ++i_ref;
        }
        if (i_ret != value.end()) {
            ++i_ret;
        }
    }
       // std::cout << str << arg;
        //nayem mihat tenam ete orinak et anuny ka stackum etam entex searchov knayem
    /*s = M._S->return_address(str, arg);
    if (s != "[       ]") {
        return s;
    }*/
    /*s = M._D->return_address(str, arg);
    if (s != "[       ]") {
        return s;
    }*/
    return s + prabel + "]";
}

std::string BSS::return_value(const std::string& str) // poxancum enq address
{
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
    return "";
}


bool BSS::find_address_bool(const std::string& str)
{
    for (auto& it : address) {
        if (it.second == str) {
            //std::cout << 222;
            return 1;
        }
    }
    return 0;
}
