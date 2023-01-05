#ifndef MEMOR_PROTOTYPE_H
#define MEMOR_PROTOTYPE_H

typedef const std::map<int, std::string>  Map;

std::map<int, std::string> code;
std::vector<std::string> function_arguments_rezolve;
int SP = 0;
//int SP_END = 0;

void ran_code(std::map<int, std::string>&, int);

void RAM_print(Map style, Map value, Map name, Map area);//տպում է վերջնական տեսքը
std::string function_prototype(std::string);//նաղատիպնե վերադարձնում or.0x0+++++start
std::string install_string(std::string);
std::string cut_from_string(std::string);
std::string find_address(std::string);

//verjum sranq klcem inchvor classi mej vorpes gorciqner

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
    virtual std::string return_address(std::string str, int arg = 0) = 0; // poxancum enq popoxakany, veradardznum e hascen:
protected:
    std::map<int, std::string> address; // Հասցեն
    std::map<int, std::string> name; // Փոփոխական
    std::map<int, std::string> value; // Արժեքը
    std::map<int, std::string> area; // Տարածք
    std::string prabel = "       ";
    std::string garbij = "[ \033[2;31mGARBAGE\033[0m  ]";
    std::string tmp_nullptr = "[ nullptr  ]";
    std::string tmp_null = "[   NULL   ]";
    int i = 0; //style ֊ի համար սկզբնական
    int index = 100; // բոլոր հասցեները, արժեքները, փոփոխականները և տարածքները սկսվում են այս ինդեքսից։
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
    bool stack_change_value(std::string, std::string, bool flag = 0); // orinak heap ic poxum enq garbijov
    int array_element_count(std::string);
    void static_array(std::string); //static zangvac
    void static_matrix(std::string); //matrix
    std::vector<std::string> return_matrix_elements(const std::string&, int);
    std::vector<std::string> return_array_elements(std::string&); //{scopneri mijiny sarqume vectoi i arjeq u return anum}
    std::vector<std::string> argument_rezolve(std::string);// veradardznum e argumentnery int a  int b or`
    void transfer(std::vector<std::string>&, std::string);
    std::string return_value(const std::string&); // poxancum enq address
    std::string return_recursive_value(std::string); //recursive mana galis minjev hasni arjeqi address
    std::string return_name(std::string); // *P p-n cuyc e talis arr[0]
    void print() override;
private:
    std::map<std::string, std::string> static_array_address;
    std::map<int, std::string> style;
}S;

class HEAP : public RAM {
 public:
    HEAP();
    void print() override;
    std::string return_name(std::string); // *P p-n cuyc e talis arr[0]
    std::string return_address(std::string, int arg = 0) override;
    std::string return_recursive_value(std::string);//recursive mana galis minjev hasni arjeqi      address poxancum enq inqy gtnum veradardznum e arjeqy
    std::string heap_allocate_space(std::string, std::string);
    void delete_allocate_space(std::string, bool flag = 0);
    void add_delete_heap_area(std::string, int);//ես ֆունկցիայով հասկանում ենք թէ heap֊ից ինչ տարածքէ վերձրել[] ? ()
    void add_pointer_allocate_space(std::string, std::string);//erb inchvor pointerov hxvum enq heap ic verctac taracqi vra int* p = arr;   ajs functiayov pahum enq anuny vorpesi haskananq te inchne jnjum
    bool change_value(std::string, std::string);// anuny u te inchenq poxum
    bool change_address_for_value(std::string, std::string);
    std::string search_pointer(std::string);
private:
    std::map<std::string, int> delete_heap_area;//[] ? () պահում ենք սրա մեջ;
    std::map<std::string, std::string> new_pointer_heap; // aystex pahum enq ayn pointernery vorov nayum enq heapic vrtcrac taracqi vra;
    std::map<int, std::string> style;
    std::map<std::string, int> delete_space;
    int new_space_count = 0;
    std::string leak = "\033[5;31m\033[1;7m Memory leak \033[0m";
}H;

class TEXT : public RAM {
public:
    TEXT();
    //std::string function_call()
    int function_call(std::map<int, std::string>); // ret main index
    std::string return_address(std::string, int arg = 0) override; //վերադարձնում է թէ որ տողում է այդ ֆունկցիան և նրա prototype֊ը։
    void print() override;
    void global_variable(std::string, std::string); // text i mijic miangamic globalneri hamar
private:
    std::map<int, std::string> style;
    std::map<std::string, std::string> fun; //funkcion  int foo() {}  kkanchi foo - n  stex kpahenq index գրել ֆունկցի որը stackum ցույց կտա որ ֆունկցիան հատկացրել է տարածք և ինչխես main֊ը կկարդանք ինչոր ֆայլի մեջ և ֆունկցիան կդառնա temprory mainորպես
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
    std::string return_value(const std::string&); // poxancum enq address
    std::string return_name(std::string); // poxancum em anuny *P vor veradardznie  arr[0]
    std::string return_recursive_value(std::string);//recursive mana galis minjev hasni arjeqi      address poxancum enq inqy gtnum veradardznum e arjeqy
    bool bss_change_address_for_value(std::string, std::string);
    bool bss_change_value(std::string, std::string, bool flag = 0); // orinak heap ic poxum enq garbijov

 private:
    std::string NULLPTR = "[  nullptr ]";
    std::map<int, std::string> style;
}B;

class READ_ONLY : public RAM {
 public:
    READ_ONLY();
    void print() override;
    std::string return_address(std::string, int arg = 0);
    std::string stack_call_char_pointer(std::string, std::string); // stanum e "hello" ->>   tox`   ev veradardznum arajin elementi hascen or`-> 0x00000022
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
    std::string return_value(const std::string&); // poxancum enq address
    void attribute(std::string);
    std::string return_name(std::string); // poxancum em anuny *P vor veradardznie  arr[0]
    std::string return_recursive_value(std::string);//recursive mana galis minjev hasni arjeqi      address poxancum enq inqy gtnum veradardznum e arjeqy
    bool data_change_address_for_value(std::string, std::string);
    bool data_change_value(std::string, std::string, bool flag = 0); // orinak heap ic poxum enq garbijov
    bool search_name(std::string); // veradardznum e 1 ete ka

private:
    std::map<int, std::string> style;
}D;


class FUNCTION : public RAM {
public:
    void function_stack_give_value(std::string);
    std::string function_call(std::string&, int, std::map<int, std::string>&); // functian texadrelu hamar
    void function_stack_pointer(std::string);
    void function_stack_lvalue_referenc(std::string);
    std::string return_address(std::string, int arg = 0) override;
    bool function_stack_change_value(std::string, std::string);
    void function_static_array(std::string); //static zangvac
    void function_static_matrix(std::string); //matrix //????????????????????????????????????????????===========
    void function_attribute(std::string);
    int function_array_element_count(std::string); // "int arr["4"]" elementneri qanaky
    std::vector<std::string> function_return_array_elements(std::string&); //{scopneri mijiny sarqume vectoi i arjeq u return anum  {1, 2, 4 , 4};  }
    void clear_rezolve_function();
    std::string function_return_value(const std::string&); // poxancum enq address
    std::map<std::string, std::string> rezolve_arguments;//functiayin poxancelu hamar
    void print() override;
private:
    void call_and_implementation();
    std::map<std::string, std::string> temprory_address;//functiayi argumentneri patahakan hascener
    //std::vector<std::string> function_arguments(std::string); // functiaji argumentnery
    std::map<int, std::string> function_area; // որպեսզի ֆունկցիայի մեջ սկզբում ման գա փոփոխականը
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
        //stex kareliya vector pahel vor functioni elemetnery pahenq mejy
}M;

#include "memory_implementation.hpp"
#endif //MEMOR_PROTOTYPE_H

