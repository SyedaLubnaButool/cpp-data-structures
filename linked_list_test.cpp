#include "splashkit.h"
#include "linked_list.hpp"

enum menu_option
{
    PRINT_LIST =1,
    ADD_NODE,
    INSERT_NODE,
    DELETE_NODE,
    QUIT
};

string to_string(menu_option opt)
{
    switch(opt)
    {
        case PRINT_LIST:
            return "Print list";
        case ADD_NODE:
            return "Add node";
        case INSERT_NODE:
            return "Insert node";
        case DELETE_NODE:
            return "Delete node";
        case QUIT:
            return "Quit";
        default:
            return "Invalid Option";
    }
}
menu_option read_menu_option(string prompt)
{
    write_line(prompt);
    for (int i=0;i<5;i++)
    {
        menu_option current_opt = menu_option(i+1);
        write_line(to_string(i+1)+ " : "+ to_string(current_opt));
    }
    int choice = read_integer_range("Select menu option",1,5);
    return menu_option(choice);
}



int main()
{
    //linked_list<int> list;

    // ---- Test first node ----
    // node<int>* first_node = list.add_node(2);

    // write("This should be 2: ");
    // write_line(list.first->data);

    // write("This should also be 2: ");
    // write_line(first_node->data);

    // write("This should also be 2: ");
    // write_line(list.last->data);

    //---- Test adding more nodes ----

    // list.add_node(4);
    // list.add_node(8);
    // list.add_node(16);
    // write("This should be 4: ");
    // write_line(list.first->next->data);

    // write("This should be 8: ");
    // write_line(list.first->next->next->data);

    // write("This should be 16: ");
    // write_line(list.last->data);
    // //iterating through elements theory:-
    // node <T> *current = list -> first;//storing list -first ptr in current pointer
    // while (current != nullptr)
    // {
    //     //do anything with iteration
        
    //     //move current to next node
    //     node<T> *next = current ->next;//preserves link to next node if we delete
    //     current = next;//using next to go to next node by storing next in current ptr
    // }
    // ---- Test find_previous_node ----

    // node<int>* node_a = list.add_node(9);
    // node<int>* node_b = list.add_node(18);

    // if (list.find_previous_node(node_b) == node_a)
    // {
    //     write_line("Found the correct previous node");
    // }
    // else
    // {
    //     write_line("Found the wrong previous node");
    // }
    //delete test
    // node<int> *first_node = list.add_node(2);
    // node<int> *second_node = list.add_node(23);
    // node<int> *third_node = list.add_node(87);
    // node<int> *fourth_node = list.add_node(15);
    // node<int> *fifth_node =list.add_node(0);
    // list.print();
    // print_repeated(".",10,true);
    // list.delete_node(third_node);
    // list.print();
    
    //strings test
    // linked_list <string> strings;
    // strings.add_node("First");
    // node<string> *del =strings.add_node("Second");
    // strings.add_node("Third");
    // strings.print();
    // strings.delete_node(del);
    // strings.print();

    //insert after test
    // node<int> *first_node = list.add_node(2);
    // node<int> *second_node = list.add_node(23);
    // node<int> *third_node = list.add_node(87);
    // node<int> *fourth_node = list.add_node(15);
    // node<int> *fifth_node = list.add_node(0);
    // list.print();
    // print_repeated("-",10,true);
    // list.insert_after(second_node,34);
    // list.print();
    // print_repeated("-",10,true);
    // list.insert_after(first_node,34);
    // list.print();
    // print_repeated("-",10,true);
    // list.insert_after(fifth_node,34);
    // list.print();
    // print_repeated("-",10,true);

       //insert before test
    // node<int> *first_node = list.add_node(2);
    // node<int> *second_node = list.add_node(23);
    // node<int> *third_node = list.add_node(87);
    // node<int> *fourth_node = list.add_node(15);
    // node<int> *fifth_node = list.add_node(0);
    // list.print();
    // print_repeated("-",10,true);
    // list.insert_before(third_node,34);
    // list.print();
    // print_repeated("-",10,true);
    // list.insert_before(first_node,34);
    // list.print();
    // print_repeated("-",10,true);
    // list.insert_before(fifth_node,34);
    // list.print();
    // print_repeated("-",10,true);

    //prepend test
    // list.prepend(5);
    // list.print();
    // list.prepend(98);
    // list.print();

    //TEST PROGRAM
    linked_list <int> numbers;
    menu_option menu_opt = read_menu_option("Choose menu option");
    while(menu_opt != QUIT)
    {
        switch (menu_opt)
        {
            case PRINT_LIST:
            {
                numbers.print();
                break;
            }
            case ADD_NODE:
            {
                int data = read_integer("Enter data to add: ");
                numbers.add_node(data);
                break;
            }
            case INSERT_NODE:
            {
                int existing_data = read_integer("Insert near what data: ");
                node<int>*insert_node = numbers.find_node(existing_data);
                if(insert_node == nullptr)
                {
                    write_line("Data not there in list");
                    break;
                }
                int insert_data = read_integer("Enter data to be inserted: ");
                bool ins_before = read_boolean("Insert before: ");
                if(ins_before)
                {
                    numbers.insert_before(insert_node,insert_data);
                }
                else
                {
                    numbers.insert_after(insert_node,insert_data);
                }
                break;
            }
            case DELETE_NODE:
            {
                int delete_data = read_integer("Enter data to delete: ");
                node<int>*del_node = numbers.find_node(delete_data);

                if(del_node == nullptr)
                {
                    write_line("Node not found");
                }
                else
                {
                    numbers.delete_node(del_node);
                }
                break;
            }
            case QUIT:
            {
                break;
            }
        }
        menu_opt = read_menu_option("Choose menu option: ");
    }
}