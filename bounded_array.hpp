template<typename T, int MAX_CAPACITY>
class bounded_array
{
    int size;
    T data[MAX_CAPACITY];
 public:
    bounded_array()
    {
        size =0;
    }

    int length()
    {
        return size;
    }

    int capacity()
    {
        return MAX_CAPACITY;
    }

    T& get(int index)
    {
        if((index<0) or (index>=size))
        {
            throw string("Invalid index,,accesses outside of array");
        }

        return data[index];
    }

    const T& get(int index) const
    {
        if((index<0) or (index>=size))
        {
            throw string("Invalid index,,accesses outside of array");
        }

        return data[index];
    }

    T& operator[](int index)
    {
        return get(index);
    }
    const T& operator[](int index) const
    {
        return get(index);
    }

    void add( T value)
    {
        if(size>= MAX_CAPACITY)
        {
            throw string("Array is full");
        }

        data[size] = value;
        size++;
    }

    void remove(int index)
    {
        if (index < 0 || index >= size)
        {
            throw string("Invalid index");
        }

        for (int i = index; i < size - 1; i++)
        {
            data[i] = data[i + 1];
        }

        data[size - 1].~T();

        size--;
    }
};
