#include <iostream>
#include <string>
#include <cstdlib>   // for malloc, free
#include <new>       // for placement new
#include <utility>

// ============================================================
//  dynamic_array<T>
//
//  A heap-allocated, automatically-resizing array.
//  Rules:
//    - Memory is managed with malloc / free (never new T[] / delete[])
//    - Elements are constructed with placement new
//    - Elements are destroyed by calling their destructor directly
//    - Copying is disabled to prevent double-free bugs
// ============================================================
template<typename T>
class dynamic_array
{
    // --------------------------------------------------------
    //  Fields
    // --------------------------------------------------------
    int  size;      // how many elements are currently stored
    int  cap;       // how many elements the allocated block can hold
    T*   data;      // pointer to the raw memory block on the heap

public:

    // --------------------------------------------------------
    //  Constructor
    //
    //  Takes an initial capacity.
    //  Throws if capacity is 0 or if malloc fails.
    // --------------------------------------------------------
    dynamic_array(int initial_capacity)
    {
        // Disallow a zero-sized array – nothing useful can be done with it
        if (initial_capacity <= 0)
        {
            throw std::string("Initial capacity must be greater than 0");
        }

        // Allocate raw bytes – enough for initial_capacity objects of type T.
        // We cast the void* that malloc returns to T*.
        // No constructors are called yet; this is just raw memory.
        data = static_cast<T*>( malloc(initial_capacity * sizeof(T)) );

        // malloc returns nullptr when it cannot satisfy the request
        if (data == nullptr)
        {
            throw std::string("Memory allocation failed in constructor");
        }

        size = 0;
        cap  = initial_capacity;
    }

    // --------------------------------------------------------
    //  Destructor
    //
    //  Destroys every live element, then releases the memory block.
    // --------------------------------------------------------
    ~dynamic_array()
    {
        // Call the destructor for every element that was constructed
        for (int i = 0; i < size; i++)
        {
            data[i].~T();
        }

        // Release the raw memory block
        free(data);
    }

    // --------------------------------------------------------
    //  Disable copying
    //
    //  If copying were allowed, two dynamic_arrays would point at
    //  the same memory and both would try to free it – a double-free.
    // --------------------------------------------------------
    dynamic_array(const dynamic_array&)            = delete;   // no copy constructor
    dynamic_array& operator=(const dynamic_array&) = delete;   // no copy assignment

    // --------------------------------------------------------
    //  length()  – returns the number of stored elements
    // --------------------------------------------------------
    int length() const
    {
        return size;
    }

    // --------------------------------------------------------
    //  capacity() – returns how many elements fit before a resize
    // --------------------------------------------------------
    int capacity() const
    {
        return cap;
    }

    // --------------------------------------------------------
    //  resize()  (private helper)
    //
    //  Doubles the capacity:
    //    1. Allocate a new, larger block with malloc
    //    2. Move each existing element into the new block via placement new
    //    3. Destroy each element in the old block
    //    4. Free the old block
    //    5. Point data at the new block and update cap
    // --------------------------------------------------------
private:
    void resize()
    {
        int new_cap = cap * 2;   // double the current capacity

        // Allocate new raw memory (no constructors called)
        T* new_data = (T*)( malloc(new_cap * sizeof(T)) );

        if (new_data == nullptr)
        {
            throw std::string("Memory reallocation failed during resize");
        }

        // Move each live element into the new block.
        // We use placement new so the element is properly constructed
        // in the new memory location using the move constructor (or
        // copy constructor if no move constructor exists).
    
        int i=0;
        try
        {
            for (; i < size; i++)
            {
                new (new_data + i) T(std::move(data[i]));
            }
        }
        catch (...)
        {
            for (int j = 0; j < i; j++)
            {
                new_data[j].~T();
            }

            free(new_data);
            throw;
        }
        
        for (int i = 0; i < size; i++)
        {
            // Destroy the original element now that we've moved out of it
            data[i].~T();
        }

        // Release the old raw memory block
        free(data);

        // Point to the new block and record the new capacity
        data = new_data;
        cap  = new_cap;
    }

    // --------------------------------------------------------
    //  Back to public interface
    // --------------------------------------------------------
public:

    // --------------------------------------------------------
    //  add(value)
    //
    //  Appends a value to the end of the array.
    //  Automatically resizes (doubles capacity) if full.
    //  Constructs the element in-place using placement new.
    // --------------------------------------------------------
    void add(T value)
    {
        // If there is no room, grow the array first
        if (size >= cap)
        {
            resize();   // may throw if reallocation fails
        }

        // Construct the new element in the next available slot.
        // (data + size) is the address of that slot.
        new (data + size) T( std::move(value) );

        size++;
    }

    // --------------------------------------------------------
    //  remove(index)
    //
    //  Removes the element at 'index' and shifts everything
    //  after it one position to the left (preserves order).
    //  Destoys the old last element; does NOT shrink capacity.
    //
    //  Throws for an invalid index.
    // --------------------------------------------------------
    void remove(int index)
    {
        if (index < 0 || index >= size)
        {
            throw std::string("Invalid index: cannot remove outside of array bounds");
        }

        // Shift elements left to fill the gap left by the removed element.
        // We use move-assignment so we don't make unnecessary copies.
        for (int i = index; i < size - 1; i++)
        {
            data[i] = std::move(data[i + 1]);
        }

        // The last element has been moved out; destroy it explicitly
        data[size - 1].~T();

        size--;
    }

    // --------------------------------------------------------
    //  get(index)  – bounds-checked element access
    //
    //  Two versions:
    //    - non-const: lets the caller read or modify the element
    //    - const:     only lets the caller read (used on const arrays)
    // --------------------------------------------------------
    T& get(int index)
    {
        if (index < 0 || index >= size)
        {
            throw std::string("Invalid index: access outside of array bounds");
        }
        return data[index];
    }

    const T& get(int index) const
    {
        if (index < 0 || index >= size)
        {
            throw std::string("Invalid index: access outside of array bounds");
        }
        return data[index];
    }

    // --------------------------------------------------------
    //  operator[]  – syntactic sugar for get()
    // --------------------------------------------------------
    T& operator[](int index)
    {
        return get(index);
    }

    const T& operator[](int index) const
    {
        return get(index);
    }
};


