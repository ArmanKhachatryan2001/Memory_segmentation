void MEMORY_CONTROLLER::print_segmentation()
{
     S.print();
     H.print();
     T.print();
     B.print();
     R.print();
     D.print();
     std::cout <<"\033[4;31mRAM-------------------------------------------------------------------------------END\033[0m\n";
}

int MEMORY_CONTROLLER::function_or_action(const std::string& str)
{
    int x = 0;
    x = str.find('(');
    if (x != -1) {
        return 1;
    }
    return 0;
}
