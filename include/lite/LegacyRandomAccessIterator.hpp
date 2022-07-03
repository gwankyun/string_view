#pragma once
#include "common.hpp"

template<typename Iter>
struct LegacyRandomAccessIterator
    : public Indirection<LegacyRandomAccessIterator<Iter>, Iter>
    , public Increment<LegacyRandomAccessIterator<Iter>, Iter>
    , public Decrement<LegacyRandomAccessIterator<Iter>, Iter>
    , public MemberAccess<LegacyRandomAccessIterator<Iter>, Iter>
    , public Subscript<LegacyRandomAccessIterator<Iter>, Iter>
    , public AdditionAssignment<LegacyRandomAccessIterator<Iter>, Iter>
    , public SubtractionAssignment<LegacyRandomAccessIterator<Iter>, Iter>
    , public Assignment<LegacyRandomAccessIterator<Iter>, Iter>
{
#ifdef __cpp_lib_concepts
    using iterator_concept = std::random_access_iterator_tag;
#endif
    typedef typename Iter::difference_type difference_type;
    typedef typename Iter::value_type value_type;
    typedef typename Iter::pointer pointer;
    typedef typename Iter::reference reference;
    typedef std::random_access_iterator_tag iterator_category;

    LegacyRandomAccessIterator() {}

    LegacyRandomAccessIterator(const LegacyRandomAccessIterator& other)
    : m_iter(other.m_iter)
    {}

    ~LegacyRandomAccessIterator() {}

    LegacyRandomAccessIterator(pointer ptr) : m_iter(ptr) {}

    LegacyRandomAccessIterator& operator=(const pointer ptr)
    {
        m_iter = ptr;
        return *this;
    }

    difference_type difference(const LegacyRandomAccessIterator& other)
    {
        return Iter::subtraction(m_iter, other.m_iter);
    }

    difference_type difference(const LegacyRandomAccessIterator& other) const
    {
        return Iter::subtraction(m_iter, other.m_iter);
    }

    pointer& get()
    {
        return m_iter;
    }

    const pointer& get() const
    {
        return m_iter;
    }

private:
    pointer m_iter;
};

template<typename T>
inline bool operator==(
    const LegacyRandomAccessIterator<T>& lhs,
    const LegacyRandomAccessIterator<T>& rhs)
{
    return lhs.difference(rhs) == 0;
}

template<typename T>
inline bool operator!=(
    const LegacyRandomAccessIterator<T>& lhs,
    const LegacyRandomAccessIterator<T>& rhs)
{
    return !(lhs == rhs);
}

template<typename T>
inline LegacyRandomAccessIterator<T> operator+(
    const LegacyRandomAccessIterator<T>& iter,
    typename LegacyRandomAccessIterator<T>::difference_type n)
{
    LegacyRandomAccessIterator<T> temp = iter;
    return temp += n;
}

template<typename T>
inline LegacyRandomAccessIterator<T> operator+(
    typename LegacyRandomAccessIterator<T>::difference_type n,
    const LegacyRandomAccessIterator<T>& iter)
{
    LegacyRandomAccessIterator<T> temp = iter;
    return temp += n;
}

template<typename T>
inline LegacyRandomAccessIterator<T> operator-(
    const LegacyRandomAccessIterator<T>& iter,
    typename LegacyRandomAccessIterator<T>::difference_type n)
{
    LegacyRandomAccessIterator<T> temp = iter;
    return temp -= n;
}

template<typename T>
inline typename LegacyRandomAccessIterator<T>::difference_type operator-(
    const LegacyRandomAccessIterator<T>& lhs,
    const LegacyRandomAccessIterator<T>& rhs)
{
    return lhs.difference(rhs);
}

template<typename T>
inline bool operator<(
    const LegacyRandomAccessIterator<T>& lhs,
    const LegacyRandomAccessIterator<T>& rhs)
{
    return rhs - lhs > 0;
}

template<typename T>
inline bool operator>(
    const LegacyRandomAccessIterator<T>& lhs,
    const LegacyRandomAccessIterator<T>& rhs)
{
    return rhs < lhs;
}

template<typename T>
inline bool operator>=(
    const LegacyRandomAccessIterator<T>& lhs,
    const LegacyRandomAccessIterator<T>& rhs)
{
    return !(lhs < rhs);
}

template<typename T>
inline bool operator<=(
    const LegacyRandomAccessIterator<T>& lhs,
    const LegacyRandomAccessIterator<T>& rhs)
{
    return !(lhs > rhs);
}
