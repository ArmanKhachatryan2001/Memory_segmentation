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

