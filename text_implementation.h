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
