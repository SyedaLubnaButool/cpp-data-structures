#include "splashkit.h"
#include "utilities.h"
template<typename T>
class node
{
    public:
    node *next;
    T data;
   
};
template<typename T>
class linked_list
{
    public:
    node<T> *first;
    node<T> *last;
    //default initializer
    linked_list()
    {
        first = nullptr;
        last = nullptr;
    }
    //function to add new node
    node<T>* add_node(T data)
    {
        node< T > *new_node = new node<T>();//initializing new node ptr to new default node-nothing

        new_node->data = data;//adding data to node 
        new_node->next = nullptr;//its points to nothing
        if(first ==nullptr)//if 1st node is being added
        {
            first =new_node;
            last = new_node;//first and last in linked list point to this first node
        }
        else//otherwise 2nd 3rd node
        {
            last->next = new_node;//old last node must point to this new node
            last = new_node;//last in linked list must point to this
        }
        return new_node;
    }

    node<T>* find_node(T data)
    {
        node<T> *current = first;
        while(current != nullptr)
        {
            if(current->data == data)
            {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    node<T>* find_previous_node(node<T> *target_node)//takes target node ptr and returns previous node ptr
    {
        if (target_node == first)
        {
            return nullptr;//no previous node for first node
        }
        node<T> *current = first;//current ptr points to first node
        while(current != nullptr)
        {
            if (current -> next == target_node)//if previous nodes next points to taget return current ptr which points to previos node
            {
                return current;
            }

            current = current->next;//move on to next node
        }
        //exception if no previous node found
        throw string("find_previous_node search failed : node not in list");
    }

    void clear()
    {
        node<T> *clear = first;
        
        while(clear != nullptr)
        {
            node<T> *next = clear->next;
            delete clear;
            clear = next;
        }
        //reset first and last
        first = nullptr;
        last = nullptr;
    }
    ~linked_list()
    {
        clear();
        write_line("List deleted");
    }

    void delete_node(node<T> *del_node)
    {
        node<T> *prev = nullptr;
        //if first node remove, set first to 2nd node and prev is null
        if(first == del_node)
        {
            first = del_node-> next;
            prev = nullptr;
        }
        else
        {
            //find prev node and its(prev) next set to del node next
            prev = find_previous_node(del_node);
            prev->next = del_node->next;
        }
        //if last node removing last set to prev of del node
        if(last == del_node)
        {
            last = prev;//prev next is del node next (nothing ) so it works
        }
        //after assigning first,last,making connetcion between prev and next correct
        delete del_node;
    }
    void print()
    {
        if(first == nullptr)
        {
            write_line("Nothing in list");
            return;
        }
        node<T> *current = first;
        while (current != nullptr)
        {
            write_line(current->data);
            current = current->next;
        }
        
    }
    node<T>* insert_after(node<T> *insert_after, T data)
    {
        node<T> *ins_node = new node<T>();
        ins_node->data = data;
        ins_node->next =nullptr;
        if (insert_after == last)
        {
            insert_after->next = ins_node;
            ins_node->next = nullptr;
            last = ins_node;
        }
        else
        {
            ins_node->next = insert_after->next;
            insert_after->next = ins_node;
        }
        return ins_node;
    }
    node<T>* insert_before(node<T> *insert_before, T data)
    {
        node<T> *ins_node = new node<T>();
        ins_node->data = data;
        ins_node->next = insert_before;
        if (insert_before == first)
        {
            first = ins_node;
        }
        else
        {
            node<T> *prev = find_previous_node(insert_before);
            prev->next = ins_node;
        }
        return ins_node;
    }
    node<T>* prepend(T data)
    {
        node<T>* pre_node = new node<T>();
        pre_node->data = data;
        pre_node->next = first;
        first = pre_node;
        //if list is empty last also points to pre node
        if (last == nullptr)
        {
            last = pre_node;
        }
        return pre_node;
    }
};