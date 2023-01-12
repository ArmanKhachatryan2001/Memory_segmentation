std::string FUNCTION::return_address(std::string str, int arg) // str == name // popoxakan e
{
    char point = ' ';



    //static int i = 0;
    //++i;
    //std::cout << str << " " << arg <<  '\n';
    //exit(0);



    std::string str4 = str;
    if (str[0] == '&') {
        point = '&';
        arg = 0;
        str.erase(0, 1);
    }
    int tmp = 0;
    int search = 0;
    for (auto& ref : function_area) {
       // std::cout << ref.first << " " << ref.second << '\n';
        if (ref.second == str) {
            search = ref.first;
            //std::cout << str << " " << search <<  "s\n";
            break;
        }
    }
    ///if (i == 2) {
       // exit(0);
    //}
    std::string s = "[";
    std::string ssr = "";
    //std::cout << str <<  " " << arg << " " << search<<  '\n';
    //exit(0);
    if (search) {
        if((area[search].find('&') != -1 && area[search].find("&&") == -1) || (area[search].find('*') != -1) && point != '&') {
            s += value[search];
            s.erase(1,1);
            //std::cout << s << "S";
            //exit(0);
            if (arg == 2 && s.find("0x") != -1) {
                //std::cout << s;
                //exit(0);
                s = find_address(s);
                s = return_recursive_value(s);
    //std::cout << s << '\n';
    //std::cout << str <<" " << arg << " A " <<  '\n';
    //exit(0);
                }
         //   std::cout << s << " " << "  A";
        } else {
            if (arg == 0) {
               s += "\033[3;33m" + address[search] + "\033[0m" + "]";
            } else {
                //std::cout << value[search] << " < ";
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
            value[index] = M._R->stack_call_char_pointer(line[1], line[3]);//"const char arr"////////***************************start heap
        } else if (line[3] == "new") {
            value[index] = M._H->heap_allocate_space(line[1] ,line[4]); // վերձնում ենք տարածք heap֊ից
        } else if (line[3][0] == '*') {
                line[3].erase(0, 1);
            //std::cout << line[3];
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
            value[index] = return_address(line[3], 1); // int a = b; (1)
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


std::string FUNCTION::function_call(std::string& str, int SP, std::map<int, std::string>& code) // functian texadrelu hamar
{
    call_and_implementation(); // es functian kapum e functiayi kanchy u poxancvac argumentnery irar het

    bool if_flag = false;
    std::stringstream s(str);
    std::string return_type = ""; // inch tipi e veradeardznum functian
    std::string temprory = ""; // ogtagorcelu
    int table = 0;
    s >> table; // es te vortexic e sksvum functian
    s >> address[index]; // 0xF00000
    s >> return_type; // inch tipi e veradardznum
    //s >> temprory; // anuny u argumentnery
    std::string tt = "";
    if (!s.eof()) {
        std::getline(s, tt);
        temprory += tt;
    }
    value[index] = "\033[9;32m" + function_prototype(return_type + temprory) + "\033[0m";
    area[index] = "";
    name[index] = "";
    prev_address = address[index]; // te inch hasceic e sksvum
    ++index;
    /////////////////////functiayi kancher ++SP_TMP
    //returni pahnel stexic
    while (++table) {
        std::string tmp = "";
        std::stringstream clear_prabel(code[table]);
        clear_prabel >> tmp;
        if (tmp != "{" && tmp != "}" && tmp[0] != '/') {
            //tepa te gorcoxutyun
            int flag = type_and_action.type_action(tmp); //class է որպեսզի հասկանա ինչ տիպ է
            // kareliya kazmakerpel swich ov kam functin pointer ov
            if (flag == -1) {
                    //urish gorcoxutyun  arr[0] = 9;
                attribute(code[table]);
            } else if (flag == 1) {               // int || int&&
                function_stack_give_value(code[table]);
            } else if (flag == 2) {              // int&
                function_stack_lvalue_referenc(code[table]);
            } else if (flag == 3) { // int*
                function_stack_pointer(code[table]);
            } else if (flag == 4) { // delete
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

    // erb tesni return miangamic tox functiayi endy dni
    // -----------------end-----------------------------
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
                //std::cout << temprory;
                //exit(0);
                function_area.clear();
                return temprory;
            }
            if (temprory[0] == '&') {
                function_area.clear();
                return return_address(temprory);
            }
            //std::cout << temprory << '\n';
            //std::cout << return_address(temprory, 2);
            //exit(0);
            return return_address(temprory, 2);
        }
    }
    return "";
}

bool FUNCTION::search_name(const std::string& str)
{
    auto itr = rezolve_arguments.find(str);
    if (itr != rezolve_arguments.end()) {
        //std::cout << itr ->first << itr->second << '\n';
        //if (itr->second.find("0x") != -1) {
            ///????????????????????????????????????????
        //} else {
            return true;
        //}
    }
    for (auto& it : name) {
        if (str == it.second) {
            return true;
        }
    }
    return false;
}


void FUNCTION::attribute(std::string str)
{
    //std::cout << str;
    //exit(0);
    bool flag = true;
    std::vector<std::string> line;
    std::stringstream s(str);
    while (s >> str) {
        line.push_back(str);
    }
    int size = line.size();
    line[size-1].pop_back(); // es verjin ';' -i hamar
    // heapum petqa unenanq change_value function vorpessi poxenq arjeqy
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
            line[0] = return_name(line[0]); // orinak *P P n vortexe cuyc talis num kam arr[0] ev = 8
            //std::cout << line[0];
            //exit(0);
        } else {
            line[0] = temp;
        }
    } else {
        flag = search_name(line[0]);
    }
    if (line[2] == "new") {
        //???????????????????????????????veradardzvox????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????
        if (!flag) {
            M._D->attribute(line[0] + " = " + line[2] + " " + line[3] + ";");
        } else {
            M._H->heap_allocate_space(line[0] ,line[3]);
            change_value(line[0], M._H->temprory_address, 1);
        }
        return;
        //std::cout << M._H->heap_allocate_space(line[0] ,line[3]);
    } else if (line[2][0] == 39 || (line[2][0] >= '0' && line[2][0] <= '9')) {
        if (!flag) {
            M._D->attribute(line[0] + " = " + line[2] + ";");
            //std::cout << 222;
            //exit(0);
        } else {
            //std::cout << line[2];
            //exit(0);
            change_value(line[0], install_string(line[2]));
        }
        return;
    } else if (std::isalpha(line[2][0])) {

        if (line[2] == "nullptr" || line[2] == "NULL") {           // /es erb p = nullptr
            change_value(line[0], install_string(line[2]), 1);
            return;
        }
        //std::cout << return_address(line[2]);
       // exit(0);
        line[2] = return_address(line[2], 1);// ete 1 apa value ete che address
        if (line[2].find("0x") != -1) {
            line[2] = return_recursive_value(line[2]); ////////???''
        }
        if (line[2] == tmp_nullptr || line[2] == tmp_null) { // es erbvor klini int* p = nullptr
            change_value(line[0], line[2], 1);
            return;
        }

        if (line[2].find("0x") != -1) { // es erb vor valuen klini hasce
            if (!flag) {
              //std::cout << line[0] << " " << line[2] << " \n";
              //exit(0);
                M._D->attribute(line[0] + " = " + line[2] + ";");
            } else {
                change_value(line[0], line[2], 1);
            }
        } else {
            if (!flag) {
                line[2] = cut_from_string(line[2]);
                M._D->attribute(line[0] + " = " + line[2] + ";");
            } else {
                change_value(line[0], install_string(line[2])); // esel tt = 88
            }
        }
        return;

    } else if (line[2][0] == '&') {
        //std::cout << line[2];
        line[2].erase(0, 1);
        line[2] = return_address(line[2]);// ete 1 apa value ete che address
        if (!flag) {
            M._D->attribute(line[0] + " = " + line[2] + ";");
        } else {
            change_value(line[0], line[2], 1); // 1-y nra hamara vor henc et hascein veragri
        }
    } else if (line[2][0] == '*') {
        if (line[2][1] == '*') {
            line[2].erase(0, 2);
        } else {
            line[2].erase(0, 1);
        }
        line[2] = return_address(line[2], 1);
        //std::cout << line[2];
        if (line[2].find("0x") == -1) {
            if (!flag) {
                M._D->attribute(line[0] + " = " + line[2] + ";");
            } else {
                change_value(line[0], line[2]); // esel tt = 88
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
std::string FUNCTION::return_name(std::string str) // poxancum em anuny *P vor veradardznie  arr[0]
{
    //std::cout << str;
    //exit(0);
    std::string str1;
    for (auto& it : temprory_address) {
        str1 = find_address(it.second);
        str1.erase(0, 1);
        str1.pop_back();
        if (str1 == str) {
            // rezolve_arguments[it.first].find("0x");
            if (rezolve_arguments[it.first].find("0x") != -1) {
                return return_name(find_address(rezolve_arguments[it.first]));
            }
        //std::cout << it.first << " " << it.second<< '\n';
        }
    }
    //exit(0);
    for (auto& it : address) {
        if(it.second == str) {
            if (value[it.first].find("0x") == -1) {
                return name[it.first];
            } else {
                /////////////////////////////////////////////////////////////////////////////////////////..
                return return_name(find_address(value[it.first]));
            }
        }
    }
    //exit(0);
    std::string s = M._H->return_name(str);
    if (s != "") {
        return s;
    }
    s = M._D->return_name(str);
    if (s != "") {
        return s;
    }
    //std::cout << s << "S";
    //exit(0);
    s = M._S->return_name(str);
    if (s != "") {
        return s;
    }
    return "";

}

std::string FUNCTION::return_recursive_value(std::string str)//recursive mana galis minjev hasni arjeqi      address poxancum enq inqy gtnum veradardznum e arjeqy
{
    static int i = 0;
    ++i;
    if (i == 2) {
    //std::cout << str;
    //exit(0);
    }

    for (auto& it : address) {
        //std::cout << it.second << '\n';
        if(it.second == str) {
            if (value[it.first].find("0x") == -1) {
                return value[it.first];
            } else {
                return return_recursive_value(find_address(value[it.first]));
            }
        }
    }
    //exit(0);
    std::string s = M._H->return_recursive_value(str);
    if (s != "") {
        return s;
    }
    s = M._D->return_recursive_value(str);
    //std::cout << s << " ";
    //exit(0);
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
    //std::cout << str_name << " "  << str_name.size();
    //exit(0);

    static bool flag = true; // esi static vorovhetev recursiva

    //if (!(str_value[0] == 39 || (str_value[0] >= '0' && str_value[0] <= '9'))) {
        //kkanchenq vor imananq arjeqy  ete mijiny hace a kam & *
    //}

    //std::cout << str_name << " ";
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
    flag = true; //??????
    if (flag) {
        //std::cout << "PPP";
        if (M._H->change_value(str_name, str_value)) {
            return 1;
        }
        if (M._D->data_change_value(str_name, str_value)) {
            return 1;
        }
        if (M._S->stack_change_value(str_name, str_value)) {
            return 1;
        }
        //kkanchenq heap ic -----------------------------------------------------------------
    }
    return 0;
}

bool FUNCTION::change_value(std::string str_name, std::string str_value, bool flag) // orinak heap ic poxum enq garbijov
{
    //auto itr = value.begin();
    //std::cout << str_name << " " << str_value;
    //exit(0);
    auto itr = rezolve_arguments.find(str_name);
    if (itr != rezolve_arguments.end()) {
        if (itr->second.find("0x") != -1) {
            itr->second = find_address(itr->second);
            change_address_for_value(itr->second, str_value);
        //std::cout << str_name << str_value << " " << itr->second << '\n';
        //ret
        //exit(0);
        } else {
            itr->second = install_string(str_value);
        }
        //std::cout << itr->first << itr->second;
        return 1;
    }

   //exit(0);
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
        //++itr;
    }

    //std::cout << str_name;
    //exit(0);
    // bss---------------------------------------
    if (M._H->change_value(str_name, str_value)) {
        return 1;
    }
    if (M._D->data_change_value(str_name, str_value, flag)) {
        return 1;
    }
    //exit(0);
    if (M._S->stack_change_value(str_name, str_value, flag)) {
        return 1;
    }
    return 0;
}

void FUNCTION::call_and_implementation()
{
    std::string str = "0xFF011111";
    auto itr = rezolve_arguments.begin();
    //++itr;
    //std::cout << itr->first;///////////////////////////////////////////////////////////////////////////
    //for (auto it : rezolve_arguments) {
      //  std::cout << it.first << " " << it.second << '\n';
    //}
    //exit(0);
    for (int i = 0; i < vector_rezolv_arguments.size(); ++i) {
        if (vector_rezolv_arguments[i].size()-1 == ']') {
            int x = vector_rezolv_arguments[i].find('[');
            vector_rezolv_arguments[i] = vector_rezolv_arguments[i].substr(0, x);
        }
    }
    int i = 0;
    for (auto& it : function_arguments_rezolve) {
        if (itr != rezolve_arguments.end()) {
            //std::cout << 1  << " ";
            if (itr->first[itr->first.size()-1] == ']') {
                //std::cout << itr->first;
                int x = itr->first.find('[');
                auto& PTR = const_cast<std::string&>(itr->first);
                std::string cut = itr->first;
                cut = cut.substr(0, x);
                PTR = cut;
            }
            rezolve_arguments[vector_rezolv_arguments[i++]] = it;
            //itr->second = it;
            ++itr;
        }
        //std::cout << it << '\n';
    }
    //exit(0);
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

    //for (auto& it : rezolve_arguments) {
      //  std::cout << it.first  << " " << it.second << '\n';;
    //}
    //exit(0);
    //std::cout << '\n';
    //for (auto& it : temprory_address) {
      //  std::cout << it.first << " " << it.second << '\n';
    //}
    //exit(0);
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
                address[index] = address_adjuster(address[index-1], it->first);// kara inchvor mi tex index-1 y tox tpi???????????????
                area[index] = it->second;
            }
            if (i == 1) {
                name[index] = line[i];
                function_area[index] = name[index];
            }
            if (i == 2) {
                //value[index] = garbij;
                continue;/////////////////////////////////////////////////// =, +=, -= ..............................................
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
                    /*static int i = 0;
                    ++i;
                    if (i == 2) {
                    std::cout << find_address(line[3]);
                    exit(0);
                    }*/
                        line[3] = return_recursive_value(find_address(line[3]));
                        //std::cout << line[3];
                        change_value(line[1], line[3]);
                    }
                } else if (line[3][0] == 39 || (line[3][0] >= '0' && line[3][0] <= '9')) {
                    tm = install_string(line[3]);
                } else {
                    //std::cout << line[3];
                    //exit(0);
                    if (line[3][0] != '[') {
                    //std::cout << line[3][0] << " ";
                        line[3] = return_address(line[3], 1);
                        if (line[3].find("0x") != -1) {
                            line[3] = return_recursive_value(line[3]);
                        }
                        //std::cout << line[3];
                        //exit(0);
                    }
                    if (line[3].size() > 12 && line[3][0] == '[') {// es en depqna erb referencic uzum en arjeqy
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

std::string FUNCTION::function_return_value(const std::string& str) // poxancum enq address
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

int FUNCTION::function_array_element_count(std::string tmp) // "int arr["4"]" elementneri qanaky
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

void FUNCTION::function_static_matrix(std::string str) //matrix //????????????????????????????????????????????===========
{
    static bool b = false;
    if (index == 100) {
        b = true;
        address[index-1] = "0x00000000";
    }
    std::string tmp = ""; // [55] = {1,2};
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
    int row = 0; // toxeri qanaky
    int column = 0;// syuneri qanaky
    int x = 2;
    do {
        cut1 = cut;
        while (line[1][cut] != ']') {
            ++cut;
        }
        tmp = line[1].substr(cut1 + 1, (cut - cut1) -1);
        if (tmp == "") {
            row = 0; // es en depqna vor chuni grac veci sizenel elementneri qanakna
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

    std::vector<std::string> vec = M._S->return_matrix_elements(line[3], column, row); //--stex kdzem vor {{1,2,3},3,3} ashxati
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
       if (vec[j] == " ") { //////////////////hly es eli knayem
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


//char arr[] = {'a', 'a', 'a', '\0'};
void FUNCTION::function_static_array(std::string str)
{
    //    ete const lini stexic miangamic data kuxarmem ///
    static bool b = true;
    if (index == 100 && b) {
        address[index-1] = "0x00000000";
    }
    std::string tmp = ""; // [55] = {1,2};
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

        //count = std::stoi(tmp);
    }
    //tmp == arr[0]
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


bool FUNCTION::function_stack_change_value(std::string str_name, std::string str_value) // orinak heap ic poxum enq garbijov
{
    //auto itr = value.begin();
    for (auto& it : name) {
        if (it.second == str_name) {
            value[it.first] = str_value;
            return 0;
        }
        //++itr;
    }
    return 1;
}
