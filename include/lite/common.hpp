#pragma once
#include <cstddef> // std::size_t std::ptrdiff_t
#include <iterator> // std::input_iterator_tag

#ifndef NULLPTR
#  define NULLPTR nullptr
#endif

#define CONSTEXPR constexpr
#define NOEXCEPT noexcept
#define NULLPTR nullptr
#define NODISCARD [[nodiscard]]

template<typename D, typename T>
struct Indirection
{
    typedef typename T::reference reference;

    reference operator*()
    {
        D& self = static_cast<D&>(*this);
        return T::indirection(self.get());
    }

    const reference operator*() const
    {
        const D& self = static_cast<const D&>(*this);
        return T::indirection(self.get());
    }
};

template<typename D, typename T>
struct Increment
{
    D& operator++()
    {
        D& self = static_cast<D&>(*this);
        T::prefix_increment(self.get());
        return self;
    }

    const D& operator++() const
    {
        D& self = static_cast<const D&>(*this);
        T::prefix_increment(self.get());
        return self;
    }

    D operator++(int)
    {
        D& self = static_cast<D&>(*this);
        D temp = self;
        T::prefix_increment(self.get());
        return temp;
    }
};

template<typename D, typename T>
struct Decrement
{
    D& operator--()
    {
        D& self = static_cast<D&>(*this);
        T::prefix_decrement(self.get());
        return self;
    }

    const D& operator--() const
    {
        const D& self = static_cast<const D&>(*this);
        T::prefix_decrement(self.get());
        return self;
    }

    D operator--(int)
    {
        D& self = static_cast<D&>(*this);
        D temp = self;
        T::prefix_decrement(self.get());
        return temp;
    }
};

template<typename D, typename T>
struct MemberAccess
{
    typename T::pointer operator->()
    {
        D& self = static_cast<D&>(*this);
        return T::member_access(self.get());
    }
};


template<typename D, typename T>
struct Subscript
{
   typedef typename T::reference reference;
   typedef typename T::difference_type difference_type;

   reference operator[](difference_type n)
   {
       D& self = static_cast<D&>(*this);
       return T::subscript(self.get(), n);
   }

   reference operator[](difference_type n) const
   {
        D& self = static_cast<D&>(*this);
       return T::subscript(self.get(), n);
   }
};

template<typename D, typename T>
struct AdditionAssignment
{
    typedef typename T::difference_type difference_type;

    D& operator+=(difference_type n)
    {
        D& self = static_cast<D&>(*this);
        T::addition_assignment(self.get(), n);
        return self;
    }
};

template<typename D, typename T>
struct SubtractionAssignment
{
    typedef typename T::difference_type difference_type;

    D& operator-=(difference_type n)
    {
        D& self = static_cast<D&>(*this);
        T::subtraction_assignment(self.get(), n);
        return self;
    }
};

template<typename D, typename T>
struct Assignment
{
    D& operator=(const D& other)
    {
        D& self = static_cast<D&>(*this);
        if (&self != &other)
        {
            T::assignment(self, other.m_iter);
        }
        return self;
    }
};

template<typename D, typename T>
struct Equality
{
    bool operator==(const D& other)
    {
        D& self = static_cast<D&>(*this);
        return &self == other.get();
    }

    bool operator!=(const D& other)
    {
        D& self = static_cast<D&>(*this);
        return &self != other.get();
    }
};
