#pragma once
#include <algorithm>
#include "macro.hpp"

//#define CONSTEXPR constexpr

namespace lite
{
    template<typename InputIt, typename ForwardIt, typename BinaryPredicate>
    CONSTEXPR InputIt find_last_of(
        InputIt first, InputIt last,
        ForwardIt s_first, ForwardIt s_last,
        BinaryPredicate p)
    {
        typedef std::reverse_iterator<InputIt> reverse_input;
        reverse_input rfirst(last);
        reverse_input rlast(first);

        typedef std::reverse_iterator<ForwardIt> reverse_forward;
        reverse_forward rs_first(s_last);
        reverse_forward rs_last(s_first);

        reverse_input it = std::find_first_of(rfirst, rlast, rs_first, rs_last, p);
        if (it == rlast) return last;

        std::ptrdiff_t size = last - first;
        std::ptrdiff_t dist = it - rfirst;

        return first + (size - 1 - dist);
    }

    template<typename T, typename U>
    struct Eq
    {
        Eq() {};
        ~Eq() {};
        bool operator()(const T& a, const U& b)
        {
            return a == b;
        }
    };

    template<typename InputIt, typename ForwardIt>
    CONSTEXPR InputIt find_last_of(
        InputIt first, InputIt last,
        ForwardIt s_first, ForwardIt s_last)
    {
        typedef typename std::iterator_traits<InputIt>::value_type input_type;
        typedef typename std::iterator_traits<ForwardIt>::value_type forward_type;
        
        Eq<input_type, forward_type> eq;

        return find_last_of(first, last, s_first, s_last, eq);
    }
}
