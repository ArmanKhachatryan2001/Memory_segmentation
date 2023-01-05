void foo(RAM* P) {
        P->print();
}

void ran_code(std::map<int, std::string>& code, int end) {
    static bool b = true;
    if (b) {
        SP = T.function_call(code); // ֆունկցիաների համար՝ վերադարձնում է main ի թիվը
        b = false;
    }
    std::string tmp = "";
    ++SP;
    while (SP != end) {
        std::stringstream clear_prabel(code[SP]);
        clear_prabel >> tmp;
        if (tmp != "{" && tmp != "}" && tmp[0] != '/') {
            //tepa te gorcoxutyun
            int flag = type_and_action.type_action(tmp); //class է որպեսզի հասկանա ինչ տիպ է
            // kareliya kazmakerpel swich ov kam functin pointer ov
            if (flag == -1) {
                int flag_action = M.function_or_action(code[SP]); // vorpesszi haskananq functia e te che
                if(flag_action == 1) { // functia e foo();
                        std::string stack_call = T.return_address(code[SP]); // veradardznum e function i masin texekutyun
                        std::vector<std::string> vec = S.argument_rezolve(stack_call); // arguments   int a  int b
                        S.transfer(vec, code[SP]);

                        F.function_call(stack_call, SP, code);

                        M._F->clear_rezolve_function();
                } else {
                    S.attribute(code[SP]); // gorcoxutyan hamara
                }
                //std::cout << code[SP];

                //std::cout << tmp << '\n';                             // gorcoxutyun
            } else if (flag == 1) {               // int || int&&
                //ete orinak senca kanenq ajstex int a = foo();
                S.stack_give_value(code[SP]);
            } else if (flag == 2) {              // int&
                //ete orinak senca kanenq ajstex int a = foo();
                S.stack_lvalue_referenc(code[SP]);
            } else if (flag == 3) { // int*
                //ete orinak senca kanenq ajstex int a = foo();
                S.stack_pointer(code[SP]);
            } else if (flag == 4) { // delete
                H.delete_allocate_space(code[SP]);
            } else if (flag == 6) { //int**
                S.stack_pointer(code[SP]);
            }
        }
        ++SP;
    }
}



void RAM_print(Map style, Map address, Map name, Map value, Map area)
{
        static int flag = 0;
        std::string s = "        ";
        auto it = style.begin();
        for (; it != style.end(); ++it) {
            if (it->second == "*/*") {
                std::cout << std::endl;
                continue;
            }
            if (flag == 2) {
                ++flag;
                M._F->print();
                std::cout <<  " "; // kkanchenq functianeri zbaxecrac taracqnery
            } else if (flag <= 2) {
                ++flag;
            }
            std::cout << it->second << " ";
        }
        auto ita = address.begin();
        auto itn = name.begin();
        auto itv = value.begin();
        auto itr = area.begin();
        std::cout << std::endl;
        while (ita != address.end() || itn != name.end() || itv != value.end() || itr != area.end()) {

            if (ita != address.end()) {
                std::cout <<  s << "\033[1;31m" + ita->second + "\033[0m" << " ";
                ++ita;
            }
            if (itv != value.end()) {
                std::cout << itv->second << " ";
                ++itv;
            }
            if (itn != name.end()) {
                std::cout << "\033[1;32m" + itn->second + "\033[0m" <<  " ";
                ++itn;
            }

             if (itr != area.end()) {
                std::cout << "\033[1;3m" + itr->second + "\033[0m"<< " ";
                ++itr;
            }
            std::cout << std::endl;
        }
}

///////////////////////////////////////////TYPE
TYPE::TYPE() {
        type = {{"void", 1}, {"int", 1}, {"bool", 1}, {"char", 1}, {"short", 1}, {"double", 1}, {"float", 1}, {"long", 1}};
        type_Rv_reference = {{"int&&", 1}, {"bool&&", 1}, {"char&&", 1}, {"short&&", 1}, {"double&&", 1}, {"float&&", 1}, {"long&&", 1}};
        type_reference = {{"int&", 1}, {"bool&", 1}, {"char&", 1}, {"short&", 1}, {"double&", 1}, {"float&", 1}, {"long&", 1}};
        type_pointer = {{"void*", 1}, {"int*", 1}, {"bool*", 1}, {"char*", 1}, {"short*", 1}, {"double*", 1}, {"float*", 1}, {"long*", 1}};
        delete_heap = {{"delete" , 1}, {"delete[]", 1}};
        return_function = {{"return", 1}, {"return;", 1}};
        type_two_pointer = {{"void**", 1}, {"int**", 1}, {"bool**", 1}, {"char**", 1}, {"short**", 1}, {"double**", 1}, {"float**", 1}, {"long**", 1}};
        //short
      //itype_pointer["void*"] = 1;
      //type_pointer["int*"] = 1;
      //type_pointer["bool*"] = 1;
      //type_pointer["char*"] = 1;
      //type_pointer["double*"] = 1;
      //type_pointer["float*"] = 1;
      //type_reference["int&"] = 1;
      //type_reference["bool&"] = 1;
      //type_reference["char&"] = 1;
      //type_reference["double&"] = 1;
      //type_reference["float&"] = 1;
}


/////////////////////////////////////////////////RAM



/////////////////////////////////////////////////STACK
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
void STACK::function_area(std::string str) { //       int main() syntax a sarqum
    //std::cout << str;
    str = prabel + str;
    style[i++] = str;
}

// else if (line[3] == "new") {
  //          value[index] = M._H->heap_allocate_space(line[1] ,line[4]); 
void STACK::attribute(std::string str)
{
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
        line[0] = find_address(return_address(line[0]));
        //std::cout << line[0] << "A\n";
        line[0] = return_name(line[0]); // orinak *P P n vortexe cuyc talis num kam arr[0] ev = 8
        //std::cout << line[0] << "A\n";
    }
    if (line[2] == "new") {
        M._H->change_value(line[0], M._H->heap_allocate_space(line[0] ,line[3]));
        return;
        //std::cout << M._H->heap_allocate_space(line[0] ,line[3]);
    } else if (line[2][0] == 39 || (line[2][0] >= '0' && line[2][0] <= '9')) {
        stack_change_value(line[0], install_string(line[2]));
        return;
    } else if (std::isalpha(line[2][0])) {

        if (line[2] == "nullptr" || line[2] == "NULL") {           // es erb p = nullptr
            stack_change_value(line[0], install_string(line[2]), 1);
            return;
        }

        line[2] = return_address(line[2], 1);// ete 1 apa value ete che address
        //std::cout << line[2];
        if (line[2] == tmp_nullptr || line[2] == tmp_null) { // es erbvor klini int* p = nullptr
            stack_change_value(line[0], line[2], 1);
            return;
        }

        if (line[2].find("0x") != -1) { // es erb vor valuen klini hasce
            stack_change_value(line[0], line[2], 1);
        } else {
            stack_change_value(line[0], install_string(line[2])); // esel tt = 88
        }
        return;

    } else if (line[2][0] == '&') {
        line[2].erase(0, 1);
        line[2] = return_address(line[2]);// ete 1 apa value ete che address
        stack_change_value(line[0], line[2], 1); // 1-y nra hamara vor henc et hascein veragri
    } else if (line[2][0] == '*') {
        if (line[2][1] == '*') {
            line[2].erase(0, 2);
        } else {
            line[2].erase(0, 1);
        }
        line[2] = return_address(line[2], 1);
        if (line[2].find("0x") == -1) {
            stack_change_value(line[0], line[2]); // esel tt = 88
        } else {
            line[2] = return_recursive_value(find_address(line[2]));
            stack_change_value(line[0], line[2]); 
        }
    }

}


std::string STACK::return_name(std::string str) // poxancum em anuny *P vor veradardznie  arr[0]
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

std::string STACK::return_recursive_value(std::string str)//recursive mana galis minjev hasni arjeqi      address poxancum enq inqy gtnum veradardznum e arjeqy
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

std::string STACK::return_address(std::string str, int arg) // str == name // popoxakan e
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
                 //std::cout << "sss" << " "  << s;
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
        //std::cout << s;
        //exit(0);
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

std::string STACK::return_value(const std::string& str) // poxancum enq address
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

            std::vector<std::string> vec = argument_rezolve(stack_call); // arguments   int a  int b
            transfer(vec, str);
            //for (int i = 0; i < function_arguments_rezolve.size(); ++i) {
              //  std::cout << function_arguments_rezolve[i] << '\n';
            //}

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
            value[index] = M._R->stack_call_char_pointer(line[1], line[3]);//"const char arr"////////***************************start heap
        } else if (line[3] == "new") {
            value[index] = M._H->heap_allocate_space(line[1] ,line[4]); // վերձնում ենք տարածք heap֊ից
        } else {
            if (line[3][0] == '*') {
                line[3].erase(0, 1);
            }
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
//////////////////////////////////////////////////////hima pointeri ev referanci hamar 
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
        std::vector<std::string> vec = argument_rezolve(stack_call); // arguments   int a  int b
        transfer(vec, str);
        //for (int i = 0; i < function_arguments_rezolve.size(); ++i) {
          //  std::cout << function_arguments_rezolve[i] << '\n';
        //}

        value[index] = F.function_call(stack_call, SP, code);

        M._F->clear_rezolve_function();

        //std::cout << value[index] << "AAA";
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
                address[index] = address_adjuster(address[index-1], it->first);// kara inchvor mi tex index-1 y tox tpi???????????????
                area[index] = it->second;
            }
            if (i == 1) {
                name[index] = line[i];
            }
            if (i == 2) {
                //value[index] = garbij;
                continue;/////////////////////////////////////////////////// =, +=, -= ..............................................
            }
            if (i == 3) {
                if (line[3].find('(') != -1) {
                    std::string stack_call = T.return_address(line[3]);
                    //std::cout << str << " ";
                    std::vector<std::string> vec = argument_rezolve(stack_call); // arguments   int a  int b
                    transfer(vec, str);
                    //for (int i = 0; i < function_arguments_rezolve.size(); ++i) {
                        //std::cout << function_arguments_rezolve[i] << '\n';
                    //}

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
                        stack_change_value(line[1], line[3]); // esel tt = 88
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
                        line[3] = return_address(line[3], 1);// ete 1 apa value ete che address
                        //std::cout << line[3] << " ";
                        //exit(0);
                    }
                    if (line[3].size() > 12 && line[3][0] == '[') {// es en depqna erb referencic uzum en arjeqy
                        int x = line[3].find("0x");
                        if (x != -1) {
                            line[3] = line[3].substr(x, 10);
                            line[3] = return_value(line[3]);
                        }
                    }
                    tm = install_string(line[3]);
                }
             }
                    //std::cout << tm;
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



void STACK::transfer(std::vector<std::string>& vec, std::string str) // stexic miangamic global vectorin
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
        //std::cout << return_address(arg[i], 1) << '\n';
        //std::cout << vec[i] << '\n';
        if (vec[i].find('*') != -1) {
            //tmp[k] = return_address(arg[i]);
            function_arguments_rezolve.push_back(return_address(arg[i]));
        } else if (vec[i].find('&') != -1 && vec[i].find("&&") == -1) {
            //tmp[k] = return_address(arg[i]);
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
            //tmp[k] = return_address(arg[i], 1);
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
    //std::cout << str << "s" << '\n';
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
                //std::cout << first1 << " ";
                second1 = first1;
                b = false;
            } else if (k == 3){
                //std::cout << first1 << " ";
                if (first1[0] >= '0' && first1[0] <= '9') {
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
    //std::cout << str_name << " "  << str_name.size();

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
                stack_change_address_for_value(str_name, str_value);
            }
            return 0;
        }
    }
    if (flag) {
        //std::cout << "PPP";
        if (M._H->change_value(str_name, str_value)) {
            return 1;
        }
        if (M._D->data_change_value(str_name, str_value)) {
            return 1;
        }
        //kkanchenq heap ic -----------------------------------------------------------------
    }
    flag = true;
    return 1;
}

bool STACK::stack_change_value(std::string str_name, std::string str_value, bool flag) // orinak heap ic poxum enq garbijov
{

   // std::cout << 222;
    //auto itr = value.begin();
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
        //++itr;
    }

    //data bss---------------------------------------
    if (M._H->change_value(str_name, str_value)) {
        return 1;
    }
    if (M._D->data_change_value(str_name, str_value)) {
        return 1;
    }
    return 0;
}

void STACK::static_matrix(std::string str) //matrix //????????????????????????????????????????????===========
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
                row = array_element_count(tmp);
            } else {
                column = array_element_count(tmp);
            }
        }
        ++cut;
        --x;
    } while (x > 0);

    std::vector<std::string> vec = return_matrix_elements(line[3], column); //--stex kdzem vor {{1,2,3},3,3} ashxati
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
       if (vec[j] == " ") { //////////////////hly es eli knayem
           ++j;
       }
       address[index] = address_adjuster(address[index-1], it->first);
       value[index] = install_string(vec[j++]);
       name[index] = line[1] + "[" + std::to_string(tmp_row) + "]" + "[" + std::to_string(i) + "]";
       if (flag_array) {
          static_array_address[line[1]] = address[index]; // anuny u hascen
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

std::vector<std::string> STACK::return_matrix_elements(const std::string& str, int column)
{
    std::vector<std::string> vec;
    std::string tmp = "";
    int size = str.size()-2;
    int num = column;
    for (int i = 1; i < size; ++i) {
        if (str[i] == '{') {
            if (num < column) {
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
                vec.push_back(" ");
            }
            --i;
        } else {
            if (str[i] == ',') {
                vec.push_back(tmp);
                --num;
                if (!num) {
                    num = column;
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
    return vec;
}

int STACK::array_element_count(std::string tmp) // "int arr["4"]" elementneri qanaky
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


//char arr[] = {'a', 'a', 'a', '\0'};
void STACK::static_array(std::string str)
{
    //    ete const lini stexic miangamic data kuxarmem ///
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
        count = vec.size(); // es en depqna vor chuni grac veci sizenel elementneri qanakna
    } else {
        count = array_element_count(tmp);
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

/////////////////////////////////////////////////HEAP
HEAP::HEAP()
{
    //style[i++] = "*/*";
    style[i++] = "        \033[4;32mHEAP---------------------------------------------------------START\033[0m\n";
}
void HEAP::print()
{
    if (delete_space.size()) {
        delete_allocate_space("", true);
    }
    address[index++] = "\033[4;32mHEAP-----------------------------------------------------------END\033[0m\n";
    RAM_print(style, address, name, value, area);
}

std::string HEAP::return_name(std::string str)
{
    for (auto& it : address) {
        if(it.second == str) {
            if (value[it.first].find("0x") == -1) {
                //std::cout << '\n' << value[it.first] << '\n';
                return name[it.first];
            } else {
                return return_name(find_address(value[it.first]));
            }
        }
    }
    // data  bss heap
    return "";

}

std::string HEAP::return_recursive_value(std::string str)//recursive mana galis minjev hasni arjeqi      address poxancum enq inqy gtnum veradardznum e arjeqy
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
    // data  bss
    return "";
}



bool HEAP::change_address_for_value(std::string str_name, std::string str_value)
{
    //std::cout << str_name << " "  << str_name.size();
    for (auto& it : address) {
        if (it.second == str_name) {
            if (value[it.first].find("0x") == -1) {
                value[it.first] = str_value;
            } else {
                str_name = find_address(value[it.first]);
                return change_address_for_value(str_name, str_value);
            }
            return 0;
        }
    }
    return 1;
}


bool HEAP::change_value(std::string str_name, std::string str_value)// anuny u te inchenq poxum
{
    for (auto& it : name) {
        if (it.second == str_name) {
            if (value[it.first].find("0x") == -1) {
                value[it.first] = str_value;
            } else {
                str_name = find_address(value[it.first]);
                change_address_for_value(str_name, str_value);
            }
            return 1;
        }
    }
    return 0;
}

void HEAP::add_pointer_allocate_space(std::string new_str_name, std::string str_name)
{
    int tmp = 0;
    if (delete_heap_area.find(str_name) != delete_heap_area.end()) {
        tmp = 1;
    }
    if (tmp) {
        new_pointer_heap[new_str_name] = str_name;
    }
}


std::string HEAP::heap_allocate_space(std::string Name, std::string str) //return address
{
    ++new_space_count; // te qani angam e new areac
    static bool b = true;
    if (index == 100) {
        address[index-1] = "0xE0000000";
    }
   
    std::string return_address = "[";
    std::string type_area = "";
    int x = str.find('[');
    int x2 = str.find(']');
    int count = 0;
    std::map<int, std::string> mp;
    int x3 = str.find('(');
    int x4 = str.find(')');
    bool allocate_one = false;
    int new_count = 0;
    std::string char_tmp = "";

    int num = 0;
    if (x != -1) {
        add_delete_heap_area(Name, 2);//ես ֆունկցիայով հասկանում ենք թէ heap֊ից ինչ տարածքէ վերձրել[] ? ()
        //count = std::stoi(str.substr(x+1, (x2 - x)-1)); //qanaky
        count = M._S->array_element_count(str.substr(x+1, (x2 - x)-1));
        type_area = str.substr(0, x); // inch tip e
    } else if (x3 != -1) {
        add_delete_heap_area(Name, 1);
        type_area = str.substr(0, x3); // inch tip e
        if (type_area == "char" || type_area == "double" || type_area == "float") {
            char_tmp = str.substr(x3+1, (x4 - x3)-1);
        } else {
            new_count = std::stoi(str.substr(x3+1, (x4 - x3)-1)); //qanaky
        }
        allocate_one = true;
        count = 1;
        //////////////////// int* p = new int(5);
    } else {
        //str.pop_back();
        type_area = str; // inch tip e
        count = 1;
    }

    mp = type_sizeof_and_appearance(type_area); // sizeof y u qani byte e
    auto it = mp.begin();

    int temprory = index;
    address[index] = address[index-1];
    value[index] = "\033[3;33m=>\033[0m";
    name[index] = "\033[3;33m" + type_area + "\033[0m";
    name[index] = "\033[3;33m(sizeof(" + name[index] + "\033[3;33m" + ") * " + std::to_string(count) + ")\033[0m";
    area[index] = "";
    delete_space[Name] = index;
    ++index;
   
    if (allocate_one) {
        address[index] = address_adjuster(address[index-1], it->first);
        return_address += "\033[3;33m" + address[index] + "\033[0m" + "]";
        if ( type_area == "double" || type_area == "float" || type_area == "char") {
            value[index] = install_string(char_tmp);
        } else {
            value[index] = install_string(std::to_string(new_count));
        }
        name[index] = Name + "[0]";
        area[index] = it->second;
        ++index;
    } else {
        for (int i = 0; i < count; ++i) {
            address[index] = address_adjuster(address[index-1], it->first);
            if (i == 0) {
                return_address += "\033[3;33m" + address[index] + "\033[0m" + "]";
            }
            value[index] = garbij;
            name[index] = Name + "[" + std::to_string(i) + "]";
            area[index] = it->second;
            ++index;
        }
    }
    address[temprory] = "\033[3;33m" + Name + "\033[0m";

    if (b) {
        address.erase(99);
        b = false; // anuny poxanci
    }
    return return_address;
}

std::string HEAP::search_pointer(std::string str) // man enq galis ayn pointery vory nayum e heap in vor haskananq te vori vra e nayum;
{
    for (auto& iii : new_pointer_heap) {
        //std::cout << iii.first << "\n";
        if (str == iii.first) {
            return iii.second;
        }
        //std::cout << iii.first << " " << iii.second << "\n";
    }
    return "1";
}

void HEAP::delete_allocate_space(std::string str, bool flag) // flagy nra hamara vor ogtagorcoxy delete chani destructori mijic poxancenq true
{
    // ete che uremn destructori mejic kkanchenq
    //std::cout << str << " ";
    if (new_space_count <= 0) {
        return;
    }
    std::stringstream s(str);
    s >> str;
    int num = -1;
    if (!flag) {
        if (str.find('[') != -1) {
            s >> str;
            str.pop_back();

            std::string new_pointer = search_pointer(str);
            if (new_pointer != "1") {
                str = new_pointer;
            }

            if (delete_heap_area[str] != 2) {
                        //std::cout << delete_heap_area[str] << " " << str;
                return;
            }
        } else {
            s >> str;
            str.pop_back();
            if (delete_heap_area[str] == 2) {
                M._S->stack_change_value(str, garbij);
                return;
            }
        }
        for (auto& it : delete_space) {
            if (it.first == str) {
                delete_space.erase(it.first);
                return;
            }
        }
    } else {
        for (auto& it : delete_space) {
            name[it.second] = leak;
        }
    }

}

void HEAP::add_delete_heap_area(std::string str, int num)
{
    delete_heap_area[str] = num;
}
std::string HEAP::return_address(std::string str, int arg)
{
    for (auto& it : name) {
        if (it.second == str) {
            if (arg) {
                return value[it.first];
            } else {
                return address[it.first];
            }
        }
    }
    return "[       ]";
}


/////////////////////////////////////////////////tEXT
TEXT::TEXT()
{
    style[i++] = "*/*";
    style[i++] = "        \033[4;32mTEXT---------------------------------------------------------START\033[0m\n\n";
    style[i++] = "0x00000000";
    style[i++] = "int main()";
}

std::string TEXT::return_address(std::string str, int arg) // վերադարձնում է ստրինգ որտեղ մինջև առաջին պռաբելը 
    // ցույց է տալիս թէ որ տողում է գտնվում այդ ֆունկցիայի սկիզբը, իսկ մնացացը ֆունկցիայի prototype֊ն է։
{
    int i = str.size()-1;
    while (i) {
        if (str[i] != '(') {
            str.pop_back();
            --i;
        } else {
            str.pop_back();
            break;
        }
    }
    std::stringstream s(str);
    s >> str;
    for (const auto& it : fun) {
        if (it.first == str) {
            return it.second;
        }
    }

    return "";
}



void TEXT::print()
{
    address[index++] = "\033[4;32mTEXT-----------------------------------------------------------END\033[0m\n";
    RAM_print(style, address, name, value, area);
}

int TEXT::function_call(std::map<int, std::string> code)//վերադարձնում է թէ որտողից է սկսվում main֊ը //ֆունկցիաների համար
{
    bool b = true;
    int scope_count = 0;
    int main_index = 0;
    for (auto& it : code) {
        std::stringstream tmp(it.second);
        std::string str = "";
        tmp >> str;

        if (b && it.second.find("main(") != -1 || it.second.find("main (") != -1) {
            b = false;
            int k = it.second.size()-1;
            while (it.second[k] != ')') {
                it.second.pop_back();
                --k;
            }
            style[3] = "\033[1;33m" + it.second + "\033[0m";
            main_index = it.first;
            S.function_area(function_prototype(style[2] + " " + it.second));
            style[2] = prabel + "\033[1;33m" + style[2] + "\033[0m";
            return main_index;
        }
        if (str == "}") {
            --scope_count;
            continue;
        }
        if (str == "{") {
            ++scope_count;
            continue;
        }
        int temprory = scope_count;
        scope_count += std::count(it.second.begin(), it.second.end(), '{');
        scope_count -= std::count(it.second.begin(), it.second.end(), '}');
        if (temprory == scope_count) {
            temprory = 1;
        } else {
            temprory = 0;
        }
        if (it.second.find('(') != -1 && it.second.find(')') != -1 && type_and_action.type_action(str) > 0) {
                static bool b = true;
                if (b) {
                    address[index-1] = "0xF0000000";
                }
                int cat = 0;
                int i = it.second.size()-1;
                for (; i >= 0; --i) {
                    if (it.second[i] != ')') {
                        ++cat;
                    } else {
                        break;
                    }
                }
                if (cat) {
                    it.second.erase(i + 1, cat);
                }
                address[index] = address_adjuster(address[index-1], 256);
                value[index] = "\033[1;33m" + it.second + "\033[0m";
                name[index] = " ";
                area[index] = " ";
                if (b) {
                    address.erase(99);
                    b = false;
                }
                tmp >> str;
                int cat_name = str.find('(');
                if (cat_name != -1) {
                    str.erase(cat_name, str.size() - cat_name);
                }
                // fun map֊ի մեջ պահում ենք թէ որ տողում է գտնվում այդ ֆունկցիան և նրա prototype:

                fun[str] = std::to_string(it.first);
                fun[str] += " ";
                fun[str] += address[index];
                fun[str] += " ";
                fun[str] += it.second;
                //std::cout << fun[str] << '\n';
                ++index;
                if (temprory) {
                    temprory = 0;
                }
        }
        ///////////////////////////////////////////////// stexic arji urish functia kanchel
        if (scope_count == 0 && temprory) {
            //std::cout << str << " "  << it.second << '\n';
            global_variable(str, it.second);
        }
    }
    return 0;
}

void TEXT::global_variable(std::string str, std::string it_second) // text i mijic miangamic globalneri hamar
{
    int flag = type_and_action.type_action(str);
            //std::cout << flag << " ffffff";
            if (flag == -1) {
                /*int flag_action = M.function_or_action(code[SP]); // vorpesszi haskananq functia e te che
                if(flag_action == 1) { // functia e foo();
                        std::string stack_call = T.return_address(code[SP]); // veradardznum e function i masin texekutyun
                        std::vector<std::string> vec = S.argument_rezolve(stack_call); // arguments   int a  int b
                        S.transfer(vec, code[SP]);
                        F.function_call(stack_call, SP, code);
                        M._F->clear_rezolve_function();
                } else {
                    M._D->attribute(code[SP]); // gorcoxutyan hamara
                }*/
            //std::cout << flag   <<  " " << it_second << " ffffff";
                M._D->attribute(it_second); // gorcoxutyan hamara??????????????????????????????
            } else if (flag == 1) {               // int || int&&
                M._D->data_give_value(it_second);
            } else if (flag == 2) {              // int&
                M._D->data_lvalue_referenc(it_second);
            } else if (flag == 3) {
                M._D->data_pointer(it_second);
            } else if (flag == 6) {
                M._D->data_pointer(it_second);
            }

}

/////////////////////////////////////////////////BSS
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
    //std::cout << str << '\n';
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
    if (line[2] == "new") {
        M._H->change_value(line[0], M._H->heap_allocate_space(line[0] ,line[3]));
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
    return "";
}


bool BSS::bss_change_address_for_value(std::string str_name, std::string str_value)
{
    //std::cout << str_name << " "  << str_name.size();

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
            return 1;
        }
        //kkanchenq heap ic -----------------------------------------------------------------
    }
    flag = true;
    return 0;
}

bool BSS::bss_change_value(std::string str_name, std::string str_value, bool flag) // orinak heap ic poxum enq garbijov
{
    //std::cout << 222;
    //auto itr = value.begin();
    for (auto& it : name) {
        if (it.second == str_name) {
            if (flag || value[it.first].find("0x") == -1) {
                value[it.first] = str_value;
            } else {
                str_name = find_address(value[it.first]);
                bss_change_address_for_value(str_name, str_value);
            }
            return 1;
        }
        //++itr;
    }

    // bss---------------------------------------
    if (M._H->change_value(str_name, str_value)) {
        return 1;
    }
    if (M._D->data_change_value(str_name, str_value)) {
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


/////////////////////////////////////////////////READ_ONLY
READ_ONLY::READ_ONLY()
{
    style[i++] = "*/*";
    style[i++] = "        \033[4;32mREAD_ONLY----------------------------------------------------START\033[0m\n";
}
void READ_ONLY::print()
{
    address[index++] = "\033[4;32mREAD_ONLY------------------------------------------------------END\033[0m\n";
    //style[i++] = "*/*";
    //style[i++] = "\033[4;31mRAM-------------------------------------------------------------------------------END\033[0m\n";
    //style[i++] = "*/*";
    RAM_print(style, address, name, value, area);
}

std::string READ_ONLY::stack_call_char_pointer(std::string n, std::string str) //         \0 ------------------------------
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
/////////////////////////////////////////////////////////DATA
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
    if (line[2] == "new") {
        M._H->change_value(line[0], M._H->heap_allocate_space(line[0] ,line[3]));
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

        line[2] = return_address(line[2], 1);// ete 1 apa value ete che address
       // std::cout << line[2];////////
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
            line[2] = return_recursive_value(find_address(line[2]));
            data_change_value(line[0], line[2]); 
        }
    }
}

std::string DATA::return_name(std::string str) // poxancum em anuny *P vor veradardznie  arr[0]
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
    s = M._B->return_name(str);
    if (s != "") {
        return s;
    }
    // bss
    return "";

}

std::string DATA::return_recursive_value(std::string str)//recursive mana galis minjev hasni arjeqi      address poxancum enq inqy gtnum veradardznum e arjeqy
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
    s = M._B->return_recursive_value(str);
    if (s != "") {
        return s;
    }
    //  bss
    return "";
}


bool DATA::data_change_address_for_value(std::string str_name, std::string str_value)
{
    //std::cout << str_name << " "  << str_name.size();

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
    if (str_name.find("0x") != -1) {
        str_name = return_name(str_name);
    }
    for (auto& it : name) {
        if (it.second == str_name) {
            if (flag || value[it.first].find("0x") == -1) {
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
    if (M._H->change_value(str_name, str_value)) {
        return 1;
    }
    if (M._B->bss_change_value(str_name, str_value)) {
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
    return "";
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
    if (s == "[") {
       s = M._B->return_address(str, arg);
       if (s != "[       ]") {
          return s;
       }
    }
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



/////////////////////////////////////////////////////////TYPE
int TYPE::type_action(std::string str) {
    if (str[0] < 'a' && str[0] > 'z' && str[0] < 'A' && str[0] > 'Z') {
        return -1;
    }
    if (return_function.find(str) != return_function.end()) {
        return 5; //return
    }
    if (delete_heap.find(str) != delete_heap.end()) {
        return 4; // delete
    }
    if (type.find(str) != type.end()) {
        return 1; // int
    }
    if (type_Rv_reference.find(str) != type_Rv_reference.end()) {
        return 1; // int&&
    }
    if (type_reference.find(str) != type_reference.end()) {
        return 2; // int&
    }
    if (type_pointer.find(str) != type_pointer.end()) {
        return 3; // int*
    }
    if (type_two_pointer.find(str) != type_two_pointer.end()) {
        return 6; // int**
    }

    return -1;
}

  //void stack_change_value(std::string, std::string);
  //void function_static_array(std::string); //static zangvac
  //std::vector<std::string> return_array_elements(std::string&); //{scopneri mijiny sarqume vectoi i arjeq u return anum  {1, 2, 4 , 4};  }
  //void print() override;



////////////////////FUNCTION

std::string FUNCTION::return_address(std::string str, int arg) // str == name // popoxakan e
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
        //std::cout << ref.first << " " << ref.second << '\n';
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
                function_attribute(code[table]);
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
            int find_ret = 0;
            if (return_type.find('*') != -1) {
                std::string ret_temp = return_address(temprory);
                ret_temp += "!" + temprory;
                function_area.clear();
                return ret_temp;
            }
            if (return_type.find('&') != -1) {
                function_area.clear();
                return return_address(temprory);
            }
            return return_address(temprory, 1);
        }
    }
    return "";
}

void FUNCTION::function_attribute(std::string str)
{
    std::vector<std::string> line;
    //std::string temprory = "";
    std::stringstream s(str);
    while (s >> str) {
        line.push_back(str);
    }
    int size = line.size();
    line[size-1].pop_back(); // es verjin ';' -i hamar
    // heapum petqa unenanq change_value function vorpessi poxenq arjeqy
    if (line[2] == "new") {
        M._H->change_value(line[0], M._H->heap_allocate_space(line[0] ,line[3]));
        //std::cout << M._H->heap_allocate_space(line[0] ,line[3]);
    }
}



void FUNCTION::call_and_implementation()
{
    std::string str = "0xFF011111";
    auto itr = rezolve_arguments.begin();
    for (auto& it : function_arguments_rezolve) {
        if (itr != rezolve_arguments.end()) {
            if (itr->first[itr->first.size()-1] == ']') {
                int x = itr->first.find('[');
                auto& PTR = const_cast<std::string&>(itr->first);
                std::string cut = itr->first;
                cut = cut.substr(0, x);
                PTR = cut;
            }
            itr->second = it;
            ++itr;
        }
    }
    for (auto itr1 = itr; itr1 != rezolve_arguments.end(); ++itr1) {
        if (itr1->second[0] != '[') {
            itr1->second = install_string(itr1->second);
        }
    }
    for (auto& it : rezolve_arguments) {
        str = address_adjuster(str, 4);
        temprory_address[it.first] = install_string(str);
    }
    /*for (auto& it : rezolve_arguments) {
        std::cout << it.first  << " " << it.second << '\n';;
    }
    std::cout << '\n';
    for (auto& it : temprory_address) {
        std::cout << it.first << " " << it.second << '\n';
    }*/
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
                if (line[3][0] == 39 || (line[3][0] >= '0' && line[3][0] <= '9')) {
                    tm = install_string(line[3]);
                } else {
                    if (line[3][0] != '[') {
                    //std::cout << line[3][0] << " ";
                        line[3] = return_address(line[3], 1);
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

    std::vector<std::string> vec = M._S->return_matrix_elements(line[3], column); //--stex kdzem vor {{1,2,3},3,3} ashxati
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

////////////////////////////


//////////////////////////MEMORY_CONTROLLER
int MEMORY_CONTROLLER::function_or_action(const std::string& str)
{
    int x = 0;
    x = str.find('(');
    if (x != -1) {
        return 1;
    }
    return 0;
}



////////////////////

std::string function_prototype(std::string str)
{
    static bool b = true;
    //std::cout << str << '\n';
    std::string tmp = "0x00000000 int main()++++++++++++++++++++++++++++++++++++++++START";
    //std::cout << tmp.size() << " ";
    for (int i = str.size() - 1; i >= 0; --i) {
        if (str[i] != ')') {
            str.pop_back();
        } else {
            break;
        }
    }
    if (b) {
        tmp.erase(0, str.size());
        b = false;
    } else {
        tmp.erase(0, 21);
        int cat = (tmp.size() + str.size() + 11) - 66;
        //std::cout << cat << " " << str << " ";
        tmp.erase(0, cat);
    }
    str += tmp;
    //str = " " + str;
    return "\033[1;33m" + str + "\033[0m";
}

std::string install_string(std::string str)
{
    std::string tm = "[          ]";
    int k = (12 - str.size()) / 2;
    for (int j = 0; j < str.size(); ++j) {
        tm[k++] = str[j];
    }
    return tm;
}


std::string cut_from_string(std::string str)
{
    std::string ret = "";
    for (int i = 1; i < str.size() - 1; ++i) {
        if (str[i] != ' ') {
            ret += str[i];
        }
    }
    return ret;
}


std::string find_address(std::string str)
{
     if (str.size() > 12) {
        int x = str.find("0x");
        str = str.substr(x, 10);
     }
     return str;
}
