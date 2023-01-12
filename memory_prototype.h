#ifndef MEMOR_PROTOTYPE_H
#define MEMOR_PROTOTYPE_H

typedef const std::map<int, std::string>  Map;

std::map<int, std::string> code;
std::vector<std::string> function_arguments_rezolve;
int SP = 0;
void ran_code(std::map<int, std::string>&, int);

void RAM_print(Map style, Map value, Map name, Map area);
std::string function_prototype(std::string);
std::string install_string(std::string);
std::string cut_from_string(std::string);
std::string find_address(std::string);

struct TYPE {
    TYPE();
    int type_action(std::string);
    std::unordered_map<std::string, bool> type;
    std::unordered_map<std::string, bool> type_reference;
    std::unordered_map<std::string, bool> type_Rv_reference;
    std::unordered_map<std::string, bool> type_pointer;
    std::unordered_map<std::string, bool> delete_heap;
    std::unordered_map<std::string, bool> return_function;
    std::unordered_map<std::string, bool> type_two_pointer;
}type_and_action;


class RAM {
public:
    virtual void print() = 0;
    virtual std::string return_address(std::string str, int arg = 0) = 0;
protected:
    std::map<int, std::string> address;
    std::map<int, std::string> name;
    std::map<int, std::string> value;
    std::map<int, std::string> area;
    std::string prabel = "       ";
    std::string garbij = "[ \033[2;31mGARBAGE\033[0m  ]";
    std::string tmp_nullptr = "[ nullptr  ]";
    std::string tmp_null = "[   NULL   ]";
    int i = 0;
    int index = 100;
};

class STACK : public RAM {
public:
    STACK();
    void attribute(std::string);
    void function_area(std::string);
    int stack_give_value(std::string);
    int stack_pointer(std::string);
    int stack_lvalue_referenc(std::string);
    std::string return_address(std::string, int arg = 0) override;
    bool stack_change_address_for_value(std::string, std::string);
    bool stack_change_value(std::string, std::string, bool flag = 0);
    int array_element_count(std::string);
    void static_array(std::string);
    void static_matrix(std::string);
    std::vector<std::string> return_matrix_elements(const std::string&, int, int);
    std::vector<std::string> return_array_elements(std::string&);
    std::vector<std::string> argument_rezolve(std::string);
    void transfer(std::vector<std::string>&, std::string);
    std::string return_value(const std::string&);
    std::string return_recursive_value(std::string);
    std::string return_name(std::string);
    void print() override;
private:
    std::map<std::string, std::string> static_array_address;
    std::map<int, std::string> style;
}S;

class HEAP : public RAM {
 public:
    HEAP();
    void print() override;
    std::string return_name(std::string);
    std::string return_address(std::string, int arg = 0) override;
    std::string return_recursive_value(std::string);
    std::string heap_allocate_space(std::string, std::string);
    void delete_allocate_space(std::string, bool flag = 0);
    void add_delete_heap_area(std::string, int);
    void add_pointer_allocate_space(std::string, std::string);
    bool change_value(std::string, std::string);
    bool change_address_for_value(std::string, std::string);
    std::string search_pointer(std::string);
    std::string temprory_address;
private:
    std::map<std::string, int> delete_heap_area;
    std::map<std::string, std::string> new_pointer_heap;
    std::map<int, std::string> style;
    std::map<std::string, int> delete_space;
    int new_space_count = 0;
    std::string leak = "\033[5;31m\033[1;7m Memory leak \033[0m";
}H;

class TEXT : public RAM {
public:
    TEXT();
    int function_call(std::map<int, std::string>);
    std::string return_address(std::string, int arg = 0) override;
    void print() override;
    void global_variable(std::string, std::string);
private:
    std::map<int, std::string> style;
    std::map<std::string, std::string> fun;
}T;

class BSS : public RAM {
 public:
    BSS();
    void print() override;
    std::string return_address(std::string, int arg = 0) override;
    int bss_give_value(std::string);
    int bss_pointer(std::string);
    int bss_lvalue_referenc(std::string);
    void attribute(std::string);
    std::string return_value(const std::string&);
    std::string return_name(std::string);
    std::string return_recursive_value(std::string);
    bool bss_change_address_for_value(std::string, std::string);
    bool bss_change_value(std::string, std::string, bool flag = 0);
    bool find_address_bool(const std::string&);
 private:
    std::string NULLPTR = "[  nullptr ]";
    std::map<int, std::string> style;
}B;

class READ_ONLY : public RAM {
 public:
    READ_ONLY();
    void print() override;
    std::string return_address(std::string, int arg = 0);
    std::string stack_call_char_pointer(std::string, std::string);
 private:
    std::map<int, std::string> style;
}R;

class DATA : public RAM {
public:
    DATA();
    void print() override;
    std::string return_address(std::string, int arg = 0);
    int data_give_value(std::string);
    int data_pointer(std::string);
    int data_lvalue_referenc(std::string);
    std::string return_value(const std::string&);
    void attribute(std::string);
    std::string return_name(std::string);
    std::string return_recursive_value(std::string);
    bool data_change_address_for_value(std::string, std::string);
    bool data_change_value(std::string, std::string, bool flag = 0);
    bool search_name(std::string);
private:
    std::map<int, std::string> style;
}D;


class FUNCTION : public RAM {
public:
    bool search_name(const std::string&);
    void function_stack_give_value(std::string);
    std::string function_call(std::string&, int, std::map<int, std::string>&);
    void function_stack_pointer(std::string);
    void function_stack_lvalue_referenc(std::string);
    std::string return_address(std::string, int arg = 0) override;
    std::string return_name(std::string);
    std::string return_recursive_value(std::string);
    bool change_address_for_value(std::string, std::string);
    bool change_value(std::string, std::string, bool flag = 0);
    bool function_stack_change_value(std::string, std::string);
    void function_static_array(std::string);
    void function_static_matrix(std::string);
    void attribute(std::string);
    int function_array_element_count(std::string);
    std::vector<std::string> function_return_array_elements(std::string&);
    void clear_rezolve_function();
    std::string function_return_value(const std::string&);
    std::map<std::string, std::string> rezolve_arguments;
    std::vector<std::string> vector_rezolv_arguments;
    void print() override;
private:
    void call_and_implementation();
    std::map<std::string, std::string> temprory_address;
    std::map<int, std::string> function_area;
    std::map<int, std::string> style;
    std::string prev_address = "0xF0000100";
}F;

class MEMORY_CONTROLLER {
    public:
        STACK* _S = &S;
        HEAP* _H = &H;
        TEXT* _T = &T;
        BSS* _B = &B;
        READ_ONLY* _R = &R;
        DATA* _D = &D;
        FUNCTION* _F = &F;
        int function_or_action(const std::string&);
        void print_segmentation();
        ~MEMORY_CONTROLLER() {
            print_segmentation();
        }
}M;

#include "file.h"
#endif //MEMOR_PROTOTYPE_H

