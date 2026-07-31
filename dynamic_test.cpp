/* dynamic-test.cpp - from the field guide. written by ... */

#include <cstdlib>
#include "splashkit.h"
#include "utilities.h"

int main()
{
    //todo: create a pointer to refer to your data
    double *data_ptr;
    // and a size to keep track of how many items we can store
    int size;
    //todo: read the size of the array from the user
    size = read_integer("Enter the number of values to store: ");
    // and allocate the memory for the data
    data_ptr = (double *)malloc(size * sizeof(double));
    //check if memory allocated
    if (data_ptr == NULL)
    {
        write_line("Memory allocation fail");
        return 1;
    }
    //todo: populate the data
    for(int i =0;i<size;i++)
    {
        data_ptr[i] = read_double("Enter double value: ");
    }
    //todo: print the data
    for (int i =0;i<size;i++)
    {
        write_line("Index: "+ to_string(i)+ " = "+ to_string(data_ptr[i]));
    }
    //todo: free the memory
    free(data_ptr);
    data_ptr = NULL;
    return 0;
}