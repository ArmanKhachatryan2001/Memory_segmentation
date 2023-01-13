void ran_code(std::map<int, std::string>& code, int end) {
    static bool b = true;
    if (b) {
        SP = T.function_call(code);
        b = false;
    }
    std::string tmp = "";
    ++SP;
    while (SP != end) {
        std::stringstream clear_prabel(code[SP]);
        clear_prabel >> tmp;
        if (tmp != "{" && tmp != "}" && tmp[0] != '/') {
            int flag = type_and_action.type_action(tmp);
            if (flag == -1) {
                int flag_action = M.function_or_action(code[SP]);
                if(flag_action == 1) {
                        std::string stack_call = T.return_address(code[SP]);
                        std::vector<std::string> vec = S.argument_rezolve(stack_call);
                        S.transfer(vec, code[SP]);
                        F.function_call(stack_call, SP, code);
                        M._F->clear_rezolve_function();
                } else {
                    S.attribute(code[SP]);
                }
            } else if (flag == 1) {
                S.stack_give_value(code[SP]);
            } else if (flag == 2) {
                S.stack_lvalue_referenc(code[SP]);
            } else if (flag == 3) {
                S.stack_pointer(code[SP]);
            } else if (flag == 4) {
                H.delete_allocate_space(code[SP]);
            } else if (flag == 6) {
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
            std::cout <<  " ";
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
