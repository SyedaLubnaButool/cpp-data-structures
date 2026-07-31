#include "splashkit.h"
#include "utilities.h"
#include "generics.hpp"

struct color_rectangle
{
    color col;
    rectangle rect;
};

void test_const_array(const bounded_array<double, 50> &my_array)
{
    write_line(my_array.get(0));
}

void test_value(double value,double expected,string fail_message)
{
    if(value != expected)
    {
        write_line(to_string(value)+" != " + to_string(expected)+ " | "+fail_message);
    }
}

int main()
{
    // bounded_array<double,5> num;
    // write_line(to_string(num.length()));
    // num.add(7.5);
    // num.add(3);
    // num.add(9);
    // write_line(to_string(num.length()));
    // for(int i=0; i<num.length();i++ )
    // {
    //     write_line("Index: "+ to_string(i)+ " is "+ to_string(num.get(i)));
    // }
    // num.remove(1);
    // for(int i=0; i<num.length();i++ )
    // {
    //     write_line("Index: "+ to_string(i)+ " is "+ to_string(num.get(i)));
    // }

    // bounded_array <color_rectangle,4> rects;

    // rects.add({COLOR_BLACK , rectangle_from(0,600,1280,120)});
    // rects.add({COLOR_RED , rectangle_from(0,0,1280,600)});
    // rects.add({COLOR_PINK ,rectangle_from(800,100,300,150)});
    // rects.add({COLOR_BLUE, rectangle_from(1280/2-300/2,350,300,300)});

    // open_window("Shapes",1280,720);

    // while(!quit_requested())
    // {
    //     process_events();
    //     clear_screen(COLOR_WHITE);

    //     for(int i =0;i<rects.length();i++)
    //     {
    //         fill_rectangle(rects.get(i).col,rects.get(i).rect);
    //     }
    //     refresh_screen();
    // }
    // close_all_windows();

    // bounded_array<double,50> my_array;
    // my_array.add(2);
    // my_array.add(23);
    // my_array.add(53);
    //test_const_array(my_array);
    //write_line(to_string(my_array[2]));

    bounded_array<double,50> numbers;
    //write_line(to_string(numbers[51]));
    write_line("Running tests - no output means no errors : ");
    print_repeated("-",8,true);

    test_value(numbers.length(),0,"0 length fail");

    numbers.add(7);
    test_value(numbers.get(0),7,"1st Get fail");

    numbers.add(3);
    test_value(numbers.get(1),3,"2nd Get fail");

    numbers.add(24);
    test_value(numbers[2],24,"4th index fail");

    numbers.get(2) = -8.5;
    test_value(numbers.get(2),-8.5,"2nd change val via get Get fail");

    test_value(numbers.length(),3,"# length fail");

    try
    {
        while(numbers.length()<50)
        {
            numbers.add(0);
        }
    }
    catch(const string& str)
    {
        write_line("Add stopped adding elements before reaching capacity: ");
    }
    
    try
    {
        numbers.add(0);
        write_line("Add conttinued adding elements past max capacity");
    }
    catch(const string& str)
    {
        //we expect an error so this is success! no output
    }

    try
    {
        numbers.get(-4);
        write_line("Accessing get -4 didnt error!");
    }
    catch(const string& str)
    {
        //we expect an error so this is success! no output
    }
    
    try
    {
        numbers[-4];
        write_line("Accessing index -4 via index didnt error!");
    }
    catch(const string& str)
    {
        //we expect an error so this is success! no output
    }

     try
    {
        numbers[50];
        write_line("Accessing index 50 didnt error!");
    }
    catch(const string& str)
    {
        //we expect an error so this is success! no output
    }

    print_repeated("Tests end",5,false);
}