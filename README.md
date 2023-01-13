<i><h1>#Segmentation</h1><br />
<br />
1.This program represents the location of variables declared in the user program.<br />
2.Works for simple types, including functions․ // local & global<br />
    
    type, type&, type&&, type*, type**, type[], type[][]
    
3.Does not perform cycles, conditional and mathematical operations.<br />
4.The program works with a nice syntax․<br />
    
    type name = value;
    type& name = value;
    type&& name = value;
    type* name = value;
    type** name = value;
    type name[] = value;
    type name[][] = value;
<br />
<br />
How to work․<br />
   1 - g++ user_file_name.cpp<br />
   2 - g++ memory_segmentation.cpp<br />
   3 - ./a.out user_file_name.cpp<br />
<br />
<br />
Example 1 - as a result of executing this program, the user will see the output shown in Example 2.<br />
<br />
Example 1.<br />
    
![Screenshot from 2023-01-13 20-59-14](https://user-images.githubusercontent.com/104670251/212376516-ad98f2df-a437-44de-be6f-2ead3704af67.png)
   
<br />
Example 2.<br />

![Screenshot from 2023-01-13 20-34-19](https://user-images.githubusercontent.com/104670251/212374916-e97a2da0-470f-419a-aebe-53685d7a1ce9.png)
![Screenshot from 2023-01-13 20-38-06](https://user-images.githubusercontent.com/104670251/212375032-b15bb889-76fc-45f3-bca2-a16ebb0bf02d.png)

In this program, a variable has an address, what is stored at that address, a name, and how much space that variable occupies.<br />
<br />    
