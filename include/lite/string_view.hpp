#pragma once
#include <string>    // std::char_traits
#include <cassert>   // std::assert
#include <stdexcept> // std::out_of_range
#include <algorithm>
#include <iterator>
#include "LegacyRandomAccessIterator.hpp"
#include "basic_string_view_const_iterator.hpp"

namespace lite
{
    template <typename CharT, typename Traits = std::char_traits<CharT>>
    class basic_string_view
    {
    public:
        typedef Traits traits_type;
        typedef CharT value_type;
        typedef CharT *pointer;
        typedef const CharT *const_pointer;
        typedef CharT &reference;
        typedef const CharT &const_reference;

        // typedef const basic_string_view_const_iterator<CharT> const_iterator;
        typedef LegacyRandomAccessIterator<basic_string_view_const_iterator<CharT>> const_iterator;
        typedef const_iterator iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef const_reverse_iterator reverse_iterator;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;

        CONSTEXPR basic_string_view() NOEXCEPT // 1
        {
            _init(NULLPTR, 0);
        }

        CONSTEXPR basic_string_view(const basic_string_view &other) NOEXCEPT // 2
        {
            _init(other.data(), other.size());
        }

        CONSTEXPR basic_string_view(const CharT *s, size_type count) // 3
        {
            _init(s, count);
        }

        CONSTEXPR basic_string_view(const CharT *s) // 4
        {
            _init(s, Traits::length(s));
        }

        ~basic_string_view()
        {
        }

        CONSTEXPR basic_string_view &operator=(const basic_string_view &view) NOEXCEPT
        {
            if (this != &view)
            {
                _init(view.data(), view.size());
            }
            return *this;
        }

        static CONSTEXPR size_type npos()
        {
            return size_type(-1);
        }

        // basic_string_view<CharT,Traits>::begin

        CONSTEXPR const_iterator begin() const NOEXCEPT
        {
            return const_iterator(const_cast<CharT *>(data()));
        }

        // std::basic_string_view<CharT, Traits>::cbegin

        constexpr const_iterator cbegin() const noexcept
        {
            return data();
        }

        // basic_string_view<CharT,Traits>::end

        constexpr const_iterator end() const noexcept
        {
            return const_iterator(const_cast<CharT *>(data() + size()));
        }

        // basic_string_view<CharT,Traits>::cend

        constexpr const_iterator cend() const noexcept
        {
            return data() + size();
        }

        // basic_string_view<CharT,Traits>::rbegin

        constexpr const_reverse_iterator rbegin() const noexcept
        {
            // return data() + size() - 1;
            // return begin() + size() - 1;
            return std::reverse_iterator<const_iterator>(end());
        }

        // basic_string_view<CharT,Traits>::crbegin

        constexpr const_reverse_iterator crbegin() const noexcept
        {
            return begin() + size() - 1;
        }

        constexpr const_reverse_iterator rend() const noexcept
        {
            // return begin() - 1;
            // return std::prev(begin());
            // return const_reverse_iterator(data()) - 1;
            return rbegin() + size();
        }

        constexpr const_reverse_iterator crend() const noexcept
        {
            return rend();
        }

        // ������

        // Ԫ�ط���

        CONSTEXPR const_reference operator[](size_type pos) const
        {
            assert(pos < size());
            return *(data() + pos);
        }

        CONSTEXPR const_reference at(size_type pos) const
        {
            if (pos >= size())
            {
                throw std::out_of_range(std::string("out_of_range"));
            }
            return (*this)[pos];
        }

        CONSTEXPR const_reference front() const
        {
            assert(!empty());
            return (*this)[0];
        }

        CONSTEXPR const_reference back() const
        {
            assert(!empty());
            return (*this)[size() - 1];
        }

        CONSTEXPR const_pointer data() const NOEXCEPT
        {
            return m_data;
        }

        // ����

        CONSTEXPR size_type size() const NOEXCEPT
        {
            return m_size;
        }

        CONSTEXPR size_type length() const NOEXCEPT
        {
            return size();
        }

        CONSTEXPR size_type max_size() const NOEXCEPT
        {
            return 1000 * 1000;
        }

        NODISCARD CONSTEXPR bool empty() const NOEXCEPT
        {
            return size() == 0;
        }

        // �޸���

        CONSTEXPR void remove_prefix(size_type n)
        {
            assert(n <= size());
            m_data += n;
            m_size -= n;
        }

        CONSTEXPR void remove_suffix(size_type n)
        {
            assert(n <= size());
            m_size -= n;
        }

        CONSTEXPR void swap(basic_string_view &v) NOEXCEPT
        {
            std::swap(m_data, v.m_data);
            std::swap(m_size, v.m_size);
        }

        // 的

        CONSTEXPR size_type copy(CharT *dest, size_type count, size_type pos = 0) const
        {
            if (pos > size())
            {
                throw std::out_of_range(std::string("out_of_range"));
            }
            size_type rcount = _min(count, size() - pos);
            Traits::copy(dest, data() + pos, rcount);
            return rcount;
        }

        CONSTEXPR basic_string_view substr(size_type pos = 0, size_type count = npos()) const
        {
            size_type copy_count = 0;
            if (count == basic_string_view::npos())
            {
                copy_count = size() - pos;
            }
            else
            {
                copy_count = count < size() - pos ? count : size() - pos;
            }
            return basic_string_view(data() + pos, copy_count);
        }

        CONSTEXPR int compare(basic_string_view v) const NOEXCEPT // 1
        {
            size_type rlen = size() < v.size() ? size() : v.size();
            int c = Traits::compare(data(), v.data(), rlen);
            if (c < 0)
            {
                return -1;
            }
            else if (c == 0)
            {
                if (size() < v.size())
                {
                    return -1;
                }
                else if (size() == v.size())
                {
                    return 0;
                }
                else // if (size() > v.size())
                {
                    return 1;
                }
            }
            else
            {
                return 1;
            }
        }

        CONSTEXPR int compare(
            size_type pos1, size_type count1,
            basic_string_view v) const // 2
        {
            return substr(pos1, count1).compare(v);
        }

        CONSTEXPR int compare(
            size_type pos1, size_type count1, basic_string_view v,
            size_type pos2, size_type count2) const // 3
        {
            return substr(pos1, count1).compare(v.substr(pos2, count2));
        }

        CONSTEXPR int compare(const CharT *s) const // 4
        {
            return compare(basic_string_view(s));
        }

        CONSTEXPR int compare(
            size_type pos1, size_type count1,
            const CharT *s) const // 5
        {
            return substr(pos1, count1).compare(basic_string_view(s));
        }

        CONSTEXPR int compare(
            size_type pos1, size_type count1,
            const CharT *s, size_type count2) const // 6
        {
            return substr(pos1, count1).compare(basic_string_view(s, count2));
        }

        CONSTEXPR bool starts_with(basic_string_view sv) const NOEXCEPT // 1
        {
            return substr(0, sv.size()).compare(sv) == 0;
        }

        CONSTEXPR bool starts_with(CharT c) const NOEXCEPT // 2
        {
            return !empty() && Traits::eq(front(), c);
        }

        CONSTEXPR bool starts_with(const CharT *s) const // 3
        {
            return starts_with(basic_string_view(s));
        }

        // basic_string_view<CharT,Traits>::ends_with

        CONSTEXPR bool ends_with(basic_string_view sv) const NOEXCEPT // 1
        {
            return size() >= sv.size() && compare(size() - sv.size(), npos(), sv) == 0;
        }

        CONSTEXPR bool ends_with(CharT c) const NOEXCEPT // 2
        {
            return !empty() && Traits::eq(back(), c);
        }

        CONSTEXPR bool ends_with(const CharT *s) const // 3
        {
            return ends_with(basic_string_view(s));
        }

        // basic_string_view<CharT,Traits>::contains

        CONSTEXPR bool contains(basic_string_view sv) const NOEXCEPT
        {
            return find(sv) != npos();
        }

        CONSTEXPR bool contains(CharT c) const NOEXCEPT
        {
            return find(c) != npos();
        }

        CONSTEXPR bool contains(const CharT *s) const
        {
            return find(s) != npos();
        }

        // basic_string_view<CharT, Traits>::find

        CONSTEXPR size_type find(basic_string_view v, size_type pos = 0) const NOEXCEPT // 1
        {
            if (size() - pos < v.size())
            {
                return npos();
            }

            const_iterator iter = std::search(begin() + pos, end(), v.begin(), v.end());
            if (iter == end())
            {
                return basic_string_view::npos();
            }
            return iter - this->begin();
        }

        CONSTEXPR size_type find(CharT ch, size_type pos = 0) const NOEXCEPT // 2
        {
            if (size() - pos < 1)
            {
                return npos();
            }

            const_iterator iter = std::find(begin() + pos, end(), ch);
            if (iter == end())
            {
                return basic_string_view::npos();
            }
            return iter - this->begin();
        }

        CONSTEXPR size_type find(const CharT *s, size_type pos, size_type count) const // 3
        {
            return find(basic_string_view(s, count), pos);
        }

        CONSTEXPR size_type find(const CharT *s, size_type pos = 0) const // 4
        {
            return find(basic_string_view(s), pos);
        }

        // basic_string_view<CharT,Traits>::rfind

        CONSTEXPR size_type rfind(basic_string_view v, size_type pos = npos()) const NOEXCEPT // 1
        {
            if (size() < v.size())
                return _npos();

            const_reverse_iterator iter = std::search(rbegin() + pos, rend(), v.rbegin(), v.rend());
            if (iter == rend())
                return this->_npos();

            return size() - (iter - rbegin()) - v.size();
        }

        CONSTEXPR size_type rfind(CharT c, size_type pos = npos()) const NOEXCEPT // 2
        {
            return rfind(basic_string_view(&c, 1), pos);
        }

        CONSTEXPR size_type rfind(const CharT* s, size_type pos, size_type count) const // 3
        {
            return rfind(basic_string_view(s, count), pos);
        }

        CONSTEXPR size_type rfind(const CharT* s, size_type pos = npos()) const // 4
        {
            return rfind(basic_string_view(s), pos);
        }

        // basic_string_view<CharT,Traits>::find_first_of

        CONSTEXPR size_type find_first_of(basic_string_view v, size_type pos = 0) const NOEXCEPT // 1
        {
            return std::find_first_of(begin() + pos, end(), v.begin(), v.end()) - begin();
        }

        CONSTEXPR size_type find_first_of(CharT c, size_type pos = 0) const NOEXCEPT // 2
        {
            return find_first_of(basic_string_view(&c, 1), pos);
        }

        CONSTEXPR size_type find_first_of(const CharT* s, size_type pos, size_type count) const // 3
        {
            return find_first_of(basic_string_view(s, count), pos);
        }

        CONSTEXPR size_type find_first_of(const CharT* s, size_type pos = 0) const // 4
        {
            return find_first_of(basic_string_view(s), pos);
        }

        // basic_string_view<CharT,Traits>::find_last_of

        CONSTEXPR size_type find_last_of(basic_string_view v, size_type pos = npos()) const NOEXCEPT // 1
        {
            const_reverse_iterator iter = std::find_first_of(rbegin(), rend() - pos, v.rbegin(), v.rend());
            if (iter == rend())
                return this->_npos();

            return size() - (iter - rbegin()) - 1;
        }

        CONSTEXPR size_type find_last_of(CharT c, size_type pos = npos()) const NOEXCEPT // 2
        {
            return find_last_of(basic_string_view(&c, 1), pos);
        }

        CONSTEXPR size_type find_last_of(const CharT* s, size_type pos, size_type count) const // 3
        {
            return find_last_of(basic_string_view(s, count), pos);
        }

        CONSTEXPR size_type find_last_of(const CharT* s, size_type pos = npos()) const // 4
        {
            return find_last_of(basic_string_view(s), pos);
        }

        // basic_string_view<CharT,Traits>::find_first_not_of

        CONSTEXPR size_type find_first_not_of(basic_string_view v, size_type pos = 0) const NOEXCEPT
        {
            struct Pred
            {
                Pred(basic_string_view& _v) : m_v(_v) {};
                ~Pred() {};
                bool operator()(const value_type& a, const value_type&)
                {
                    return std::find(m_v.begin(), m_v.end(), a) == m_v.end();
                }
            private:
                basic_string_view& m_v;
            } pred(v);

            return std::find_first_of(begin() + pos, end(), v.begin(), v.end(), pred) - this->begin();
        }

        size_type _npos() const
        {
            return size_type(-1);
        }

    private:
        void _init(const_pointer _data, size_type _size)
        {
            m_data = _data;
            m_size = _size;
        }

        size_type _min(const size_type &_a, const size_type &_b) const
        {
            return _a < _b ? _a : _b;
        }

        const_pointer m_data;
        size_type m_size;
    };

    typedef basic_string_view<char, std::char_traits<char>> string_view;
}

template <typename CharT, typename Traits>
CONSTEXPR bool operator==(
    lite::basic_string_view<CharT, Traits> lhs,
    lite::basic_string_view<CharT, Traits> rhs) NOEXCEPT
{
    return lhs.compare(rhs) == 0;
}

template <typename CharT, typename Traits>
CONSTEXPR bool operator!=(
    const lite::basic_string_view<CharT, Traits> lhs,
    const lite::basic_string_view<CharT, Traits> rhs) NOEXCEPT
{
    return lhs.compare(rhs) != 0;
}

template <typename CharT, typename Traits>
CONSTEXPR bool operator<(
    lite::basic_string_view<CharT, Traits> lhs,
    lite::basic_string_view<CharT, Traits> rhs) NOEXCEPT
{
    return lhs.compare(rhs) < 0;
}

template <typename CharT, typename Traits>
CONSTEXPR bool operator<=(
    lite::basic_string_view<CharT, Traits> lhs,
    lite::basic_string_view<CharT, Traits> rhs) NOEXCEPT
{
    return lhs.compare(rhs) <= 0;
}

template <typename CharT, typename Traits>
CONSTEXPR bool operator>(
    lite::basic_string_view<CharT, Traits> lhs,
    lite::basic_string_view<CharT, Traits> rhs) NOEXCEPT
{
    return lhs.compare(rhs) > 0;
}

template <typename CharT, typename Traits>
CONSTEXPR bool operator>=(
    lite::basic_string_view<CharT, Traits> lhs,
    lite::basic_string_view<CharT, Traits> rhs) NOEXCEPT
{
    return lhs.compare(rhs) >= 0;
}
