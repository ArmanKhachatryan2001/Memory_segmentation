#ifndef MEMOR_PROTOTYPE_H
#define MEMOR_PROTOTYPE_H

typedef const std::map<int, std::string>  Map;

int SP = 0;
void RAM_print(Map style, Map value, Map name, Map area);//տպում է վերջնական տեսքը
std::string function_prototype(std::string);//նաղատիպնե վերադարձնում or.0x0+++++start
std::string install_string(std::string);

struct TYPE {
    TYPE();
    int type_action(std::string);
    std::unordered_map<std::string, bool> type;
    std::unordered_map<std::string, bool> type_reference;
    std::unordered_map<std::string, bool> type_Rv_reference;
    std::unordered_map<std::string, bool> type_pointer;
    std::unordered_map<std::string, bool> delete_heap;
}type_and_action;


class RAM {
public:
    virtual void print() = 0;
    virtual std::string return_address(std::string str) = 0; // poxancum enq popoxakany, veradardznum e hascen:
protected:
    std::map<int, std::string> address; // Հասցեն
    std::map<int, std::string> name; // Փոփոխական
    std::map<int, std::string> value; // Արժեքը
    std::map<int, std::string> area; // Տարածք
    std::string prabel = "       ";
    std::string garbij = "[  \033[2;31mGARBIJ\033[0m  ]";
    int i = 0; //style ֊ի համար սկզբնական
    int index = 100; // բոլոր հասցեները, արժեքները, փոփոխականները և տարածքները սկսվում են այս ինդեքսից։
};

class STACK : public RAM {
public:
    STACK();
    void function_area(std::string);
    int stack_give_value(std::string);
    int stack_pointer(std::string);
    int stack_lvalue_referenc(std::string);
    std::string return_address(std::string) override;
    void stack_change_value(std::string, std::string); // orinak heap ic poxum enq garbijov
    void static_array(std::string); //static zangvac
    std::vector<std::string> return_array_elements(std::string&); //{scopneri mijiny sarqume vectoi i arjeq u return anum}
    void print() override;
private:
    std::map<int, std::string> style;
}S;

class HEAP : public RAM {
 public:
    HEAP();
    void print() override;
    std::string return_address(std::string) override;
    std::string heap_allocate_space(std::string, std::string);
    void delete_allocate_space(std::string, bool flag = false);
    void add_delete_heap_area(std::string, int);//ես ֆունկցիայով հասկանում ենք թէ heap֊ից ինչ տարածքէ վերձրել[] ? ()
private:
    std::map<std::string, int> delete_heap_area;//[] ? () պահում ենք սրա մեջ;
    std::map<int, std::string> style;
    std::map<std::string, int> delete_space;
    int new_space_count = 0;
    std::string leak = "\033[5;33m Memory leak\033[0m";
}H;

class TEXT : public RAM {
public:
    TEXT();
    //std::string function_coll()
    int function_coll(std::map<int, std::string>); // ret main index
    std::string return_address(std::string) override;
    void print() override;
private:
    std::map<int, std::string> style;
    std::map<std::string, int> fun; //funkcion  int foo() {}  kkanchi foo - n  stex kpahenq index գրել ֆունկցի որը stackum ցույց կտա որ ֆունկցիան հատկացրել է տարածք և ինչխես main֊ը կկարդանք ինչոր ֆայլի մեջ և ֆունկցիան կդառնա temprory mainորպես
}T;

class BSS : public RAM {
 public:
    BSS();
    void print() override;
    std::string return_address(std::string) override;
    int bss_give_value(std::string);
    int bss_pointer(std::string);
    int bss_lvalue_referenc(std::string);

 private:
    std::string NULLPTR = "[  nullptr ]";
    std::map<int, std::string> style;
}B;

class READ_ONLY : public RAM {
 public:
    READ_ONLY();
    void print() override;
    std::string return_address(std::string);
    std::string stack_coll_char_pointer(std::string, std::string); // stanum e "hello" ->>   tox`   ev veradardznum arajin elementi hascen or`-> 0x00000022
 private:
    std::map<int, std::string> style;
}R;

class DATA : public RAM {
public:
    DATA();
    void print() override;
    std::string return_address(std::string);
    int data_give_value(std::string);
    int data_pointer(std::string);
    int data_lvalue_referenc(std::string);

private:
    std::map<int, std::string> style;
}D;

class MEMORY_CONTROLLER {
    public:
        STACK* _S = &S;
        HEAP* _H = &H;
        TEXT* _T = &T;
        BSS* _B = &B;
        READ_ONLY* _R = &R;
        DATA* _D = &D;
}M;

#include "Memory_implementation.hpp"
#endif //MEMOR_PROTOTYPE_H

