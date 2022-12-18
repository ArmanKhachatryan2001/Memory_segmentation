void foo(RAM* P) {
        P->print();
}

void ran_code(std::map<int, std::string>& code, int end) {
    SP = T.function_coll(code); // ֆունկցիաների համար՝ վերադարձնում է main ի թիվը
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
                if(flag_action == 1) { // functia e
                        std::string stack_coll = T.return_address(code[SP]); // veradardznum e function i masin texekutyun
                        F.function_coll(stack_coll);
                } else if (flag_action == 2) { // urish gorcoxutyun
                
                }
                //foo();
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

std::string STACK::return_address(std::string str) // str == name // popoxakan e
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
                s += "\033[3;33m" + address[it.first] + "\033[0m" + "]";
             }
            return s;
        }
        ++i_ref;
        ++i_ret;
    }
    if (s == "[") {
        s = M._R->return_address(str);
        if (s != "[       ]") {
            return s;
        }
        s = M._H->return_address(str);
        if (s != "[       ]") {
            return s;
        }
        s = M._B->return_address(str);
        if (s != "[       ]") {
            return s;
        }
        s = M._D->return_address(str);
        if (s != "[       ]") {
            return s;
        }
    }
    return s + prabel + "]";
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
            value[index] = M._R->stack_coll_char_pointer(line[1], line[3]);//"const char arr"////////***************************start heap
        } else if (line[3] == "new") {
            value[index] = M._H->heap_allocate_space(line[1] ,line[4]); // վերձնում ենք տարածք heap֊ից
        } else {
            value[index] = return_address(line[3]);
        }
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

int STACK::stack_lvalue_referenc(std::string str)
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
    value[index] = return_address(line[3]);
    area[index] = it->second;
    ++index;
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
            if (line[line.size()-1].find('[') != -1) {
                static_array(str);
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
                tm = install_string(line[3]);
            }
            value[index] = tm;
        }
    }
    ++index;
    if (b) {
        address.erase(99);
        b = false;
    }
    return 0;
}

void STACK::stack_change_value(std::string str_name, std::string str_value) // orinak heap ic poxum enq garbijov
{
    auto itr = value.begin();
    for (auto& it : name) {
        if (it.second == str_name) {
            value[itr->first] = str_value;
            break;
        }
        ++itr;
    }
}
//char arr[] = {'a', 'a', 'a', '\0'};
void STACK::static_array(std::string str)
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
        count = std::stoi(tmp);
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
        count = std::stoi(str.substr(x+1, (x2 - x)-1)); //qanaky
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
        name[index] = Name + "[" + "0" + "]";
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


void HEAP::delete_allocate_space(std::string str, bool flag) // flagy nra hamara vor ogtagorcoxy delete chani destructori mijic poxancenq true
{
    // ete che uremn destructori mejic kkanchenq
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
std::string HEAP::return_address(std::string)
{
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

std::string TEXT::return_address(std::string str) // վերադարձնում է ստրինգ որտեղ մինջև առաջին պռաբելը 
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

int TEXT::function_coll(std::map<int, std::string> code)//վերադարձնում է թէ որտողից է սկսվում main֊ը //ֆունկցիաների համար
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
        if (scope_count == 0 && temprory) {
            int flag = type_and_action.type_action(str);
            //std::cout << flag << " ffffff";
             if (flag == 1) {               // int || int&&
                M._D->data_give_value(it.second);
            } else if (flag == 2) {              // int&
                M._D->data_lvalue_referenc(it.second);
            } else if (flag == 3) {
                M._D->data_pointer(it.second);
            }

            //std::cout << it.second << " <><> ";
            //global popoxakanneri hamar ///////////////////////////////////////////////
        }

       // while (tmp >> str) {
            //մնացած ֆունկցիաները
        //}
    }
    return 0;
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
int BSS::bss_lvalue_referenc(std::string){return 0;} // չի կարա ստեղ լվալյու լինի


std::string BSS::return_address(std::string str)
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
                s += "\033[3;33m" + address[it.first] + "\033[0m" + "]";
             }
            return s;
        }
        ++i_ref;
        ++i_ret;
    }
        //std::cout << s;
    return s + prabel + "]";
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

std::string READ_ONLY::stack_coll_char_pointer(std::string n, std::string str) //         \0 ------------------------------
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

std::string READ_ONLY::return_address(std::string str)
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
                s += "\033[3;33m" + address[it.first] + "\033[0m" + "]";
             }
            return s;
        }
        ++i_ref;
        ++i_ret;
    }
    if (s == "[") {
        s = M._H->return_address(str);
        if (s != "[       ]") {
            return s;
        }
        s = M._B->return_address(str);
        if (s != "[       ]") {
            return s;
        }
        s = M._D->return_address(str);
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
            t = install_string(line[3]);
        }
        value[index] = t;
    }
    ++index;
    if (b) {
        address.erase(99);
        b = false;
    }
    return 0;
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
        value[index] = M._R->stack_coll_char_pointer(line[1], line[3]);/////////////////////////////////////////
        } else {
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
    value[index] = return_address(line[3]);
    area[index] = it->second;
    ++index;
    return 0;
}

std::string DATA::return_address(std::string str)
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
                s += "\033[3;33m" + address[it.first] + "\033[0m" + "]";
             }
            return s;
        }
        ++i_ref;
        ++i_ret;
    }
    if (s == "[") {
       s = M._B->return_address(str);
       //std::cout << s;
       if (s != "[") {
          return s;
       }
       //s = M._R->return_address(str);
       //std::cout << s;
       //if (s != "[       ]") {
       //   return s;
      // }
    }
    return s + prabel + "]";
}



/////////////////////////////////////////////////////////TYPE
int TYPE::type_action(std::string str) {
    if (str[0] < 'a' && str[0] > 'z' && str[0] < 'A' && str[0] > 'Z') {
        return -1;
    }
    if (delete_heap.find(str) != delete_heap.end()) {
        return 4;
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

    return -1;
}

  //void stack_change_value(std::string, std::string);
  //void function_static_array(std::string); //static zangvac
  //std::vector<std::string> return_array_elements(std::string&); //{scopneri mijiny sarqume vectoi i arjeq u return anum  {1, 2, 4 , 4};  }
  //void print() override;



////////////////////FUNCTION

void FUNCTION::function_area(std::string str) //       int main() syntax a sarqum
{
    //std::cout << str;
    //str = prabel + str;
    //style[i++] = str;
}

void FUNCTION::function_coll(std::string& str/*std::map<int, std::string>*/) // functian texadrelu hamar
{
    //cankacac texic return miangamic endy u veradarnum e -----------------end-----------------------------
    //std::cout << " 0" << str << "0 ";
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


    address[index] = "------------------------------END---------------------------------";
    value[index] = "";
    name[index] = "";
    area[index] = "\n";
    ++index;
    // erb tesni return miangamic tox functiayi endy dni
    // -----------------end-----------------------------

    //std::cout << str;
}

void FUNCTION::print()
{
     RAM_print(style, address, name, value, area);
}

std::string FUNCTION::return_address(std::string str)
{
    return "";
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
    return 2;
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

