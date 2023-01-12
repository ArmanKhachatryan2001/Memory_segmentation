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
