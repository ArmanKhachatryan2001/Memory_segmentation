std::string address(unsigned int number)
{
    unsigned int nam = 0;
    char arr[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','\0'};
    std::string str = "0x00000000";
    int i = str.size() - 1;
    do{
        nam = number % 16;
        str[i--] = arr[nam];
        number /= 16;
    } while(number != 0);
    return str;
}

std::string address_adjuster(std::string arr, int size_type) {
    if (arr == "") {
        arr = "0x00000000";
    }
    int nam = 1;
    int sam = 0;
    int index = 0;
    int n = 16;
    for (int i = arr.size()-1; i >= 2; --i) {
        switch (arr[i]) {
            case 'A':
                nam = 10;
                break;
            case  'B':
                nam = 11;
                break;
            case 'c':
                nam = 12;
                break;
            case 'D':
                nam = 13;
                break;
            case 'E':
                nam = 14;
                break;
            case 'F':
                nam = 15;
                break;
            default:
                nam = arr[i] - 48;
        }
        for(int i = 0; i < index; ++i) {
            nam *= n;
        }
        ++index;
        sam += nam;
    }
    return address(sam+size_type);
}
