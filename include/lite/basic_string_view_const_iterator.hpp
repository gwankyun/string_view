#pragma once

template <typename T>
struct basic_string_view_const_iterator
{
    typedef std::ptrdiff_t difference_type;
    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;

    static reference indirection(pointer ptr)
    {
        return *ptr;
    }

    static void prefix_increment(pointer &ptr)
    {
        ++ptr;
    }

    static void prefix_decrement(pointer &ptr)
    {
        --ptr;
    }

    static pointer member_access(pointer ptr)
    {
        return ptr;
    }

    static reference subscript(pointer ptr, difference_type n)
    {
        return *(ptr + n);
    }

    static void addition_assignment(pointer &ptr, difference_type n)
    {
        ptr += n;
        // return *this;
    }

    static void assignment(pointer &a, pointer b)
    {
        a = b;
    }

    static void subtraction_assignment(pointer &ptr, difference_type n)
    {
        ptr -= n;
        // return *ptr;
    }

    static difference_type subtraction(const pointer &a, const pointer &b)
    {
        return a - b;
    }
};
