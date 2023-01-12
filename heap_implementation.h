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
            return 1;
        }
    }
    return 0;
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
    temprory_address = return_address;
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
