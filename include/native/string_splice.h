//
// Copyright (c) 2013 Mike Naquin. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef NATIVE_STRING_SPLICE_H__
#define NATIVE_STRING_SPLICE_H__

#include "native/string_base.h"
#include "native/string_core.h"

namespace native {

// Splice strings are a way to efficiently refer to a sub-string without having
// to make a copy of the super-string.
//
// string super_string = "abcdefghijklmnopqrstuvwxyz";
// string_splice sub_string = super_string.substr(13, 3); // just points to "mno"
//
//
// "abcdefghijklmnopqrstuvwxyz" - super-string
//              ^
//             "xyz"            sub-string
//
// As a consequence of this, splices *cannot be null terminated*. A null terminated
// copy can instead be obtained with one of the string conversion methods
// str() or std_str().
template <typename Ch>
class basic_string_splice:
    public string_base {
public:
    typedef basic_string_splice<Ch> this_type;
    typedef std::char_traits<Ch> traits_type;
    typedef typename traits_type::char_type value_type;
    typedef std::allocator<Ch> allocator_type;
    typedef typename allocator_type::size_type size_type;
    typedef typename allocator_type::difference_type difference_type;
    typedef typename allocator_type::reference reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef typename allocator_type::pointer pointer;
    typedef typename allocator_type::const_pointer const_pointer;
    typedef const_pointer const_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    typedef std::basic_string<value_type> std_type;
    typedef detail::string_common<this_type> common;
    typedef basic_string_core<Ch> core_type;
    typedef basic_string_splice<value_type> splice_type;
    typedef basic_string<Ch> string_type;

    friend string_type;

    static const size_type npos;
    
    // 1) Default constructor. Constructs empty string.
    basic_string_splice() noexcept;

    // 2) Constructs the string with count copies of character ch.
    basic_string_splice(size_type n, value_type c);

    // 3) Constructs the string with a substring [pos, pos+count) of other. If
    //    the requested substring lasts past the end of the string, or if
    //    count == npos, the resulting substring is [pos, size()). If
    //    pos >= other.size(), std::out_of_range is thrown.
    basic_string_splice(const string_type& str, size_type pos, size_type n = npos);
    basic_string_splice(const std_type& str,    size_type pos, size_type n = npos);
    basic_string_splice(const basic_string_splice& str, size_type pos, size_type n = npos);

    // 4) Constructs the string with the first count characters of character
    //    string pointed to by s. s can contain null characters. s must not be
    //    a NULL pointer.
    basic_string_splice(const_pointer s, size_type n);

    // 5) Constructs the string with the contents of null-terminated character
    //    string pointed to by s. The length of the string is determined by the
    //    first null character. s must not be a NULL pointer.
    basic_string_splice(const_pointer s);

    // 6) Constructs the string with the contents of the range [first, last).
    template<class InputIterator>
    basic_string_splice(InputIterator begin, InputIterator end);

    // 7) Copy constructor. Constructs the string with the copy of the contents
    //    of other.
    basic_string_splice(const basic_string_splice& str);
    basic_string_splice(const string_type& str);
    basic_string_splice(const std_type& str);

    // 8) Move constructor. Constructs the string with the contents of other
    //    using move semantics.
    basic_string_splice(basic_string_splice&& str) noexcept;

    // 9) Constructs the string with the contents of the initializer list init.
    explicit basic_string_splice(std::initializer_list<value_type>);


    // 1) Replaces the contents with a copy of str
    basic_string_splice& operator=(const basic_string_splice& str);
    basic_string_splice& operator=(const std_type& str);
    basic_string_splice& operator=(const string_type& str);

    // 2) Replaces the contents with those of str using move semantics. str is
    //    in undefined state after the operation.
    basic_string_splice& operator=(basic_string_splice&& str);
    
    // 3) Replaces the contents with those of null-terminated character string
    //    pointed to by s.
    basic_string_splice& operator=(const value_type* s);

    // 4) Replaces the contents with character ch
    basic_string_splice& operator=(value_type ch);

    // 5) Replaces the contents with those of the initializer list ilist.
    basic_string_splice& operator=(std::initializer_list<value_type> ilist);

    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;

    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator rend() const noexcept;

    const_iterator         cbegin() const noexcept;
    const_iterator         cend() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    const_reverse_iterator crend() const noexcept;

    size_type size() const noexcept;
    size_type length() const noexcept;
    size_type max_size() const noexcept;
    size_type capacity() const noexcept;

    bool empty() const noexcept;

    const_reference operator[](size_type pos) const;

    const_reference at(size_type n) const;

    const_reference front() const;
    const_reference back() const;

    size_type copy(pointer s, size_type n, size_type pos = 0) const;
    template <std::size_t Size>
    size_type copy(value_type (&s)[Size], size_type pos = 0) const;
    this_type substr(size_type pos = 0, size_type n = npos) const;

    string_type str() const { return std::move(string_type(data(), size())); }
    std_type std_str() const;
    // No c_str() on purpose. Splices are NOT NULL TERMINATED!!!
    const_pointer data() const noexcept;

    // 1) Split a string by ch.
    std::vector<basic_string_splice> split(value_type ch) const;

    // 2) Split a string by s.
    std::vector<basic_string_splice> split(const_pointer s) const;

    // 3) Split a string by str.
    template <typename String>
    typename std::enable_if<
        is_string_class<String>::value,
        std::vector<basic_string_splice>
    >::type split(const String& str) const;

    // 1) Finds the first substring equal to str. If str.size() is zero, returns pos.
    template <typename String>
    typename std::enable_if<
        is_string_class<String>::value,
        size_type
    >::type find(const String& str, size_type pos = 0) const noexcept;

    // 2) Finds the first substring equal to the first count characters of the
    //    character string pointed to by s. s can include null characters. If
    //    count is zero, returns pos.
    size_type find(const_pointer s, size_type pos, size_type n) const noexcept;

    // 3) Finds the first substring equal to the character string pointed to by
    //    s. The length of the string is determined by the first null character.
    //    If the length of s is zero, returns pos.
    size_type find(const_pointer s, size_type pos = 0) const noexcept;

    // 4) Finds the first character ch.
    size_type find(value_type c, size_type pos = 0) const noexcept;
    
    // 1) Finds the last substring equal to str. If str.size() is zero, returns
    //    pos or size()-1, whichever is smaller.
    template <typename String>
    typename std::enable_if<
        is_string_class<String>::value,
        size_type
    >::type rfind(const String& str, size_type pos = npos) const noexcept;

    // 2) Finds the last substring equal to the first count characters of the
    //    character string pointed to by s. s can include null characters. If
    //    count is zero, returns pos or size()-1, whichever is smaller.
    size_type rfind(const_pointer s, size_type pos, size_type n) const noexcept;

    // 3) Finds the last substring equal to the character string pointed to by
    //    s. The length of the string is determined by the first null character.
    //    If the length of s is zero, returns pos or size()-1, whichever is
    //    smaller.
    size_type rfind(const_pointer s, size_type pos = npos) const noexcept;

    // 4) Finds the last character ch.
    size_type rfind(value_type c, size_type pos = npos) const noexcept;

    // 1) Finds the first character equal to one of characters in str.
    template <typename String>
    typename std::enable_if<
        is_string_class<String>::value,
        size_type
    >::type find_first_of(const String& str, size_type pos = 0) const noexcept;

    // 2) Finds the first character equal to one of characters in the first
    //    count characters of the character string pointed to by s. s can
    //    include null characters.
    size_type find_first_of(const_pointer s, size_type pos, size_type n) const noexcept;

    // 3) Finds the first character equal to one of characters in character
    //    string pointed to by s. The length of the string is determined by the
    //    first null character.
    size_type find_first_of(const_pointer s, size_type pos = 0) const noexcept;

    // 4) Finds the first character equal to ch.
    size_type find_first_of(value_type c, size_type pos = 0) const noexcept;
    
    // 1) Finds the last character equal to one of characters in str.
    template <typename String>
    typename std::enable_if<
        is_string_class<String>::value,
        size_type
    >::type find_last_of(const String& str, size_type pos = npos) const noexcept;

    // 2) Finds the last character equal to one of characters in the first count
    //    characters of the character string pointed to by s. s can include null
    //    characters.
    size_type find_last_of(const_pointer s, size_type pos, size_type n) const noexcept;

    // 3) Finds the last character equal to one of characters in character
    //    string pointed to by s. The length of the string is determined by the
    //    first null character.
    size_type find_last_of(const_pointer s, size_type pos = npos) const noexcept;

    // 4) Finds the last character equal to ch.
    size_type find_last_of(value_type c, size_type pos = npos) const noexcept;

    // 1) Finds the first character equal to none of characters in str.
    template <typename String>
    typename std::enable_if<
        is_string_class<String>::value,
        size_type
    >::type find_first_not_of(const String& str, size_type pos = 0) const noexcept;

    // 2) Finds the first character equal to none of characters in the first
    //    count characters of the character string pointed to by s. s can
    //    include null characters.
    size_type find_first_not_of(const_pointer s, size_type pos, size_type n) const noexcept;

    // 3) Finds the first character equal to none of characters in character
    //    string pointed to by s. The length of the string is determined by the
    //    first null character.
    size_type find_first_not_of(const_pointer s, size_type pos = 0) const noexcept;

    // 4) Finds the first character not equal to ch.
    size_type find_first_not_of(value_type c, size_type pos = 0) const noexcept;
    
    // 1) Finds the last character equal to none of characters in str.
    template <typename String>
    typename std::enable_if<
        is_string_class<String>::value,
        size_type
    >::type find_last_not_of(const String& str, size_type pos = npos) const noexcept;

    // 2) Finds the last character equal to none of characters in the first
    //    count characters of the character string pointed to by s. s can
    //     include null characters.
    size_type find_last_not_of(const_pointer s, size_type pos, size_type n) const noexcept;

    // 3) Finds the last character equal to none of characters in character
    //    string pointed to by s. The length of the string is determined by the
    //    first null character.
    size_type find_last_not_of(const_pointer s, size_type pos = npos) const noexcept;

    // 4) Finds the last character not equal to ch.
    size_type find_last_not_of(value_type c, size_type pos = npos) const noexcept;


    // 1) Compares this string to str. First, calculates the number of
    //    characters to compare, as if by size_type rlen = std::min(size(),
    //    str.size()). Then compares by calling Traits::compare(data(),
    //    str.data(), rlen). For standard strings this function performs
    //    character-by-character lexicographical comparison.
    template <typename String>
    typename std::enable_if<
        is_string_class<String>::value,
        int
    >::type compare(const String& str) const noexcept;

    // 2) Compares a [pos1, pos1+count1) substring of this string to str as if
    //    by basic_string(*this, pos1, count1).compare(str)
    template <typename String>
    typename std::enable_if<
        is_string_class<String>::value,
        int
    >::type compare(size_type pos1, size_type n1, const String& str) const;

    // 3) Compares a [pos1, pos1+count1) substring of this string to a substring
    //    [pos2, pas2+count2) of str as if by
    //    basic_string(*this, pos1, count1).compare(basic_string(str, pos2, count2))
    template <typename String>
    typename std::enable_if<
        is_string_class<String>::value,
        int
    >::type compare(size_type pos1, size_type n1, const String& str,
                size_type pos2, size_type n2) const;

    // 4) Compares this string to the null-terminated character sequence
    //    beginning at the character pointed to by s, as if by
    //    compare(basic_string(s))
    int compare(const_pointer s) const noexcept;

    // 5) Compares a [pos1, pos1+count1) substring of this string to the
    //    null-terminated character sequence beginning at the character pointed
    //    to by s, as if by
    //    basic_string(*this, pos, count1).compare(basic_string(s))
    int compare(size_type pos1, size_type n1, const_pointer s) const;

    // 6) Compares a [pos1, pos1+count1) substring of this string to the first
    //    count2 characters of the character array whose first character is
    //    pointed to by s, as if by
    //    basic_string(*this, pos, count1).compare(basic_string(s, count2)).
    //    (Note: the characters from s to s+count2 may include null characters))
    int compare(size_type pos1, size_type n1, const_pointer s, size_type n2) const;


private:
    // 0)
    basic_string_splice(const core_type& core, size_type pos, size_type length);

    core_type _core;
    const_pointer _head;
    size_type _length;
};

typedef basic_string_splice<char>     string_splice;
typedef basic_string_splice<wchar_t>  wstring_splice;
typedef basic_string_splice<char16_t> u16string_splice;
typedef basic_string_splice<char32_t> u32string_splice;

//
// Implementations
//

template <typename Ch>
constexpr const typename basic_string_splice<Ch>::size_type basic_string_splice<Ch>::npos = -1;

// 0)
template <typename Ch>
basic_string_splice<Ch>::basic_string_splice(
    const core_type& core, size_type pos, size_type n):
    _core(core)
{
    if (pos >= _core.size())
    {
        this->throw_out_of_range();
    }
    else
    {
        _head = _core.data() + pos;
        _length = std::min(n, _core.size());
    }
}

// 1) Default constructor. Constructs empty string.
template <typename Ch>
basic_string_splice<Ch>::basic_string_splice() noexcept:
    _head(),
    _length(),
    _core()
{

}

// 2) Constructs the string with count copies of character ch.
template <typename Ch>
basic_string_splice<Ch>::basic_string_splice(size_type n, value_type c):
    _core(n, c),
    _head(_core.data()),
    _length(n)
{

}

// 3) Constructs the string with a substring [pos, pos+count) of other. If the
//    requested substring lasts past the end of the string, or if count == npos,
//    the resulting substring is [pos, size()). If pos >= other.size(),
//    std::out_of_range is thrown.
template <typename Ch>
basic_string_splice<Ch>::basic_string_splice(
    const basic_string_splice& str, size_type pos, size_type n):
    basic_string_splice(str._core, pos, n)
{

}

template <typename Ch>
basic_string_splice<Ch>::basic_string_splice(
    const std_type& str, size_type pos, size_type n):
    _core()
{
    if (pos >= str.size())
    {
        this->throw_out_of_range();
    }
    else
    {
        n = std::min(n, str.size());
        this->_core = std::move(core_type(str.data() + pos, n));
        _head = _core.data();
        _length = n;
    }
}

template <typename Ch>
basic_string_splice<Ch>::basic_string_splice(
    const string_type& str, size_type pos, size_type n):
    basic_string_splice(str._core, pos, n)
{

}

// 4) Constructs the string with the first count characters of character
//    string pointed to by s. s can contain null characters. s must not be
//    a NULL pointer.
template <typename Ch>
basic_string_splice<Ch>::basic_string_splice(const_pointer s, size_type n):
    _core(s, n),
    _head(_core.data()),
    _length(n)
{

}


// 5) Constructs the string with the contents of null-terminated character
//    string pointed to by s. The length of the string is determined by the
//    first null character. s must not be a NULL pointer.
template <typename Ch>
basic_string_splice<Ch>::basic_string_splice(const_pointer s):
    _core(s),
    _head(_core.data()),
    _length(_core.size())
{

}

// 6) Constructs the string with the contents of the range [first, last).
template <typename Ch>
template<class InputIterator>
basic_string_splice<Ch>::basic_string_splice(
    InputIterator begin, InputIterator end):
    _core(begin, end),
    _head(_core.data()),
    _length(_core.size())
{

}

// 7) Copy constructor. Constructs the string with the copy of the contents
//    of other.
template <typename Ch>
basic_string_splice<Ch>::basic_string_splice(const basic_string_splice& str):
    _core(str._core),
    _head(str._head),
    _length(str._length)
{

}

template <typename Ch>
basic_string_splice<Ch>::basic_string_splice(const string_type& str):
    _core(str._core),
    _head(_core.data()),
    _length(_core.size())
{

}

template <typename Ch>
basic_string_splice<Ch>::basic_string_splice(const std_type& str):
    _core(str.data(), str.size()),
    _head(_core.data()),
    _length(_core.size())
{

}

// 8) Move constructor. Constructs the string with the contents of other
//    using move semantics.
template <typename Ch>
basic_string_splice<Ch>::basic_string_splice(
    basic_string_splice&& str) noexcept:
    _core(std::move(str._core)),
    _head(str._head),
    _length(str._length)
{

}

// 9) Constructs the string with the contents of the initializer list init.
template <typename Ch>
basic_string_splice<Ch>::basic_string_splice(
    std::initializer_list<value_type> ilist):
    _core(ilist.begin(), ilist.end()),
    _head(_core.data()),
    _length(_core.size())
{

}

// 1) Replaces the contents with a copy of str
template <typename Ch>
basic_string_splice<Ch>& basic_string_splice<Ch>::operator=(const basic_string_splice& str)
{
    this->_core = str._core;
    this->_head = str._head;
    this->_length = str._length;
    return *this;
}

template <typename Ch>
basic_string_splice<Ch>& basic_string_splice<Ch>::operator=(const std_type& str)
{
    this->_core   = std::move(core_type(str.data(), str.size()));
    this->_head   = _core.data();
    this->_length = _core.size();
    return *this;
}

template <typename Ch>
basic_string_splice<Ch>& basic_string_splice<Ch>::operator=(const string_type& str)
{
    this->_core   = std::move(core_type(str.data(), str.size()));
    this->_head   = _core.data();
    this->_length = _core.size();
    return *this;
}


// 2) Replaces the contents with those of str using move semantics. str is
//    in undefined state after the operation.
template <typename Ch>
basic_string_splice<Ch>& basic_string_splice<Ch>::operator=(basic_string_splice&& str)
{
    this->_core   = str._core;
    this->_head   = str._head;
    this->_length = str._length;
    return *this;
}

// 3) Replaces the contents with those of null-terminated character string pointed to by s.
template <typename Ch>
basic_string_splice<Ch>& basic_string_splice<Ch>::operator=(const value_type* s)
{
    this->_core   = std::move(core_type(s));
    this->_head   = _core.data();
    this->_length = _core.size();
    return *this;
}

// 4) Replaces the contents with character ch
template <typename Ch>
basic_string_splice<Ch>& basic_string_splice<Ch>::operator=(
    value_type ch)
{
    this->_core   = std::move(core_type(ch, 1));
    this->_head   = _core.data();
    this->_length = _core.size();
    return *this;
}

// 5) Replaces the contents with those of the initializer list ilist.
template <typename Ch>
basic_string_splice<Ch>& basic_string_splice<Ch>::operator=(std::initializer_list<value_type> ilist)
{
    this->_core   = std::move(core_type(ilist.begin(), ilist.end()));
    this->_head   = _core.data();
    this->_length = _core.size();
    return *this;
}


// 1) Split a string by ch.
template <typename Ch>
std::vector<basic_string_splice<Ch>>
    basic_string_splice<Ch>::split(value_type ch) const
{
    std::vector<splice_type> result;
    size_type start = 0, end = find(ch);
    for(;;)
    {
        if (end == npos)
        {
            result.push_back(splice_type(_core, start, size() - start));
            break;
        }

        result.push_back(splice_type(_core, start, end - start));
        start = end + 1;
        end = find(ch, start);
    }

    return std::move(result);
}

// 2) Split a string by s.
template <typename Ch>
std::vector<basic_string_splice<Ch>>
    basic_string_splice<Ch>::split(const_pointer s) const
{
    const size_type n = traits_type::length(s);
    std::vector<splice_type> result;
    if (n == 0)
    {
        result.push_back(splice_type(_core));
    }
    else
    {
        size_type start = 0, end = find(s, 0, n);
        for(;;)
        {
            if (end == npos)
            {
                result.push_back(splice_type(_core, start, size() - start));
                break;
            }

            result.push_back(splice_type(_core, start, end - start));
            start = end + n;
            end = find(s, start, n);
        }
    }

    return std::move(result);
}

// 3) Split a string by str.
template <typename Ch>
template <typename String>
typename std::enable_if<
    is_string_class<String>::value,
    std::vector<basic_string_splice<Ch>>
>::type basic_string_splice<Ch>::split(const String& str) const
{
    std::vector<splice_type> result;
    if (str.empty())
    {
        result.push_back(splice_type(_core));
    }
    else
    {
        size_type start = 0, end = find(str);
        for(;;)
        {
            if (end == npos)
            {
                result.push_back(splice_type(_core, start, size() - start));
                break;
            }

            result.push_back(splice_type(_core, start, end - start));
            start = end + str.size();
            end = find(str, start);
        }
    }

    return std::move(result);
}


template <typename Ch>
inline typename basic_string_splice<Ch>::const_iterator
    basic_string_splice<Ch>::begin() const noexcept
{
    return &data()[0];
}

template <typename Ch>
inline typename basic_string_splice<Ch>::const_iterator
    basic_string_splice<Ch>::basic_string_splice<Ch>::end() const noexcept
{
    return &data()[0] + size();
}

template <typename Ch>
inline typename basic_string_splice<Ch>::const_reverse_iterator
    basic_string_splice<Ch>::rbegin() const noexcept
{
    return const_reverse_iterator(end());
}

template <typename Ch>
inline typename basic_string_splice<Ch>::const_reverse_iterator
    basic_string_splice<Ch>::rend() const noexcept
{
    return const_reverse_iterator(begin());
}

template <typename Ch>
inline typename basic_string_splice<Ch>::const_iterator
    basic_string_splice<Ch>::cbegin() const noexcept
{
    return begin();
}

template <typename Ch>
inline typename basic_string_splice<Ch>::const_iterator
    basic_string_splice<Ch>::cend() const noexcept
{
    return end();
}

template <typename Ch>
inline typename basic_string_splice<Ch>::const_reverse_iterator
    basic_string_splice<Ch>::crbegin() const noexcept
{
    return rbegin();
}

template <typename Ch>
inline typename basic_string_splice<Ch>::const_reverse_iterator
    basic_string_splice<Ch>::crend() const noexcept
{
    return rend();
}

template <typename Ch>
inline typename basic_string_splice<Ch>::size_type
    basic_string_splice<Ch>::size() const noexcept
{
    return this->_length;
}

template <typename Ch>
inline typename basic_string_splice<Ch>::size_type
    basic_string_splice<Ch>::length() const noexcept
{
    return this->_length;
}

template <typename Ch>
inline typename basic_string_splice<Ch>::size_type
    basic_string_splice<Ch>::max_size() const noexcept
{
    return this->_length;
}

template <typename Ch>
inline typename basic_string_splice<Ch>::size_type
    basic_string_splice<Ch>::capacity() const noexcept
{
    return this->_length;
}

template <typename Ch>
inline bool basic_string_splice<Ch>::empty() const noexcept
{
    return this->_length == 0;
}

template <typename Ch>
inline typename basic_string_splice<Ch>::const_reference
    basic_string_splice<Ch>::operator[](size_type pos) const
{
    return data()[pos];
}

template <typename Ch>
typename basic_string_splice<Ch>::const_reference
    basic_string_splice<Ch>::at(size_type n) const
{
    if (n >= this->_length)
    {
        this->throw_out_of_range();
    }
    return data()[n];
}



template <typename Ch>
typename basic_string_splice<Ch>::const_reference
    basic_string_splice<Ch>::front() const
{
    return data()[0];
}

template <typename Ch>
typename basic_string_splice<Ch>::const_reference
    basic_string_splice<Ch>::back() const
{
    return data()[size()-1];
}

template <typename Ch>
typename basic_string_splice<Ch>::size_type
    basic_string_splice<Ch>::copy(pointer s, size_type n, size_type pos) const
{
    if (pos >= size())
    {
        this->throw_out_of_range();
    }
    n = std::min(n, size()-pos);
    for (size_type i = 0; i < n; ++i)
    {
        s[i] = data()[pos++];
    }

    return n;
}

template <typename Ch>
template <std::size_t Size>
inline typename basic_string_splice<Ch>::size_type
    basic_string_splice<Ch>::copy(value_type (&s)[Size], size_type pos) const
{
    return copy(s, Size-1, pos);
}

template <typename Ch>
basic_string_splice<Ch>
    basic_string_splice<Ch>::substr(size_type pos, size_type n) const
{
    const auto length = size();
    if (pos > length)
    {
        this->throw_out_of_range();
    }
    n = std::min(n, length);
    const auto offset = this->_head - this->_core.data();
    pos += offset; // offset to original start
    n += offset;
    return basic_string_splice(this->_core, pos, n);
}

template <typename Ch>
inline typename basic_string_splice<Ch>::std_type
    basic_string_splice<Ch>::std_str() const
{
    return std::move(std_type(data(), size()));
}

template <typename Ch>
inline typename basic_string_splice<Ch>::const_pointer
    basic_string_splice<Ch>::data() const noexcept
{
    return this->_head;
}



//
// find
//
// 1) Finds the first substring equal to str. If str.size() is zero, returns pos.
template <typename Ch>
template <typename String>
inline typename std::enable_if<
    is_string_class<String>::value,
    typename basic_string_splice<Ch>::size_type
>::type
    basic_string_splice<Ch>::find(const String& str, size_type pos) const noexcept
{
    return common::find(*this, str.data(), pos, str.size());
}

template <typename Ch>
inline typename basic_string_splice<Ch>::size_type
    basic_string_splice<Ch>::find(const_pointer s, size_type pos, size_type n) const noexcept
{
    return common::find(*this, s, pos, n);
}

template <typename Ch>
inline typename basic_string_splice<Ch>::size_type
    basic_string_splice<Ch>::find(const_pointer s, size_type pos) const noexcept
{
    return common::find(*this, s, pos, traits_type::length(s));
}

template <typename Ch>
inline typename basic_string_splice<Ch>::size_type
    basic_string_splice<Ch>::find(value_type c, size_type pos) const noexcept
{
    return common::find(*this, c, pos);
}

//
// rfind
//
template <typename Ch>
template <typename String>
inline typename std::enable_if<
    is_string_class<String>::value,
    typename basic_string_splice<Ch>::size_type
>::type
    basic_string_splice<Ch>::rfind(const String& str, size_type pos) const noexcept
{
    return common::rfind(*this, str.data(), pos, str.size());
}

template <typename Ch>
inline typename basic_string_splice<Ch>::size_type
    basic_string_splice<Ch>::rfind(const_pointer s, size_type pos, size_type n) const noexcept
{
    return common::rfind(*this, s, pos, n);
}

template <typename Ch>
inline typename basic_string_splice<Ch>::size_type
    basic_string_splice<Ch>::rfind(const_pointer s, size_type pos) const noexcept
{
    return common::rfind(*this, s, pos, traits_type::length(s));
}

template <typename Ch>
typename basic_string_splice<Ch>::size_type
    basic_string_splice<Ch>::rfind(value_type c, size_type pos) const noexcept
{
    return common::rfind(*this, c, pos);
}

//
// find_first_of
//
template <typename Ch>
template <typename String>
inline typename std::enable_if<
    is_string_class<String>::value,
    typename basic_string_splice<Ch>::size_type
>::type
    basic_string_splice<Ch>::find_first_of(const String& str, size_type pos) const noexcept
{
    return common::find_first_of(*this, str.data(), pos, str.size());
}

template <typename Ch>
inline typename basic_string_splice<Ch>::size_type
    basic_string_splice<Ch>::find_first_of(const_pointer s, size_type pos, size_type n) const noexcept
{
    return common::find_first_of(*this, s, pos, n);
}

template <typename Ch>
inline typename basic_string_splice<Ch>::size_type
    basic_string_splice<Ch>::find_first_of(const_pointer s, size_type pos) const noexcept
{
    return common::find_first_of(*this, s, pos, traits_type::length(s));
}

template <typename Ch>
inline typename basic_string_splice<Ch>::size_type
    basic_string_splice<Ch>::find_first_of(value_type c, size_type pos) const noexcept
{
    return common::find(*this, c, pos);
}

//
// find_last_of
//
template <typename Ch>
template <typename String>
inline typename std::enable_if<
    is_string_class<String>::value,
    typename basic_string_splice<Ch>::size_type
>::type
    basic_string_splice<Ch>::find_last_of(const String& str, size_type pos) const noexcept
{
    return find_last_of(str.data(), pos, str.size());
}

template <typename Ch>
inline typename basic_string_splice<Ch>::size_type
    basic_string_splice<Ch>::find_last_of(const_pointer s, size_type pos, size_type n) const noexcept
{
    return common::find_last_of(*this, s, pos, n);
}

template <typename Ch>
inline typename basic_string_splice<Ch>::size_type
    basic_string_splice<Ch>::find_last_of(const_pointer s, size_type pos) const noexcept
{
    return common::find_last_of(*this, s, pos, traits_type::length(s));
}

template <typename Ch>
inline typename basic_string_splice<Ch>::size_type
    basic_string_splice<Ch>::find_last_of(value_type c, size_type pos) const noexcept
{
    return common::rfind(*this, c, pos);
}

//
// find_first_not_of
//
template <typename Ch>
template <typename String>
inline typename std::enable_if<
    is_string_class<String>::value,
    typename basic_string_splice<Ch>::size_type
>::type
    basic_string_splice<Ch>::find_first_not_of(const String& str, size_type pos) const noexcept
{
    return find_first_not_of(str.data(), pos, str.size());
}

template <typename Ch>
inline typename basic_string_splice<Ch>::size_type basic_string_splice<Ch>::find_first_not_of(const_pointer s, size_type pos, size_type n) const noexcept
{
    return common::find_first_not_of(*this, s, pos, n);
}

template <typename Ch>
inline typename basic_string_splice<Ch>::size_type
    basic_string_splice<Ch>::find_first_not_of(const_pointer s, size_type pos) const noexcept
{
    return common::find_first_not_of(*this, s, pos, traits_type::length(s));
}

template <typename Ch>
inline typename basic_string_splice<Ch>::size_type
    basic_string_splice<Ch>::find_first_not_of(value_type c, size_type pos) const noexcept
{
    return common::find_first_not_of(*this, c, pos);
}

//
// find_last_not_of
//
template <typename Ch>
template <typename String>
inline typename std::enable_if<
    is_string_class<String>::value,
    typename basic_string_splice<Ch>::size_type
>::type
    basic_string_splice<Ch>::find_last_not_of(const String& str, size_type pos) const noexcept
{
    return common::find_last_not_of(*this, str.data(), pos, str.size());
}

template <typename Ch>
inline typename basic_string_splice<Ch>::size_type
    basic_string_splice<Ch>::find_last_not_of(const_pointer s, size_type pos, size_type n) const noexcept
{
    return common::find_last_not_of(*this, s, pos, n);
}

template <typename Ch>
inline typename basic_string_splice<Ch>::size_type
    basic_string_splice<Ch>::find_last_not_of(const_pointer s, size_type pos) const noexcept
{
    return common::find_last_not_of(*this, s, pos, traits_type::length(s));
}

template <typename Ch>
typename basic_string_splice<Ch>::size_type basic_string_splice<Ch>::find_last_not_of(value_type c, size_type pos) const noexcept
{
    return common::find_last_not_of(*this, c, pos);
}

//
// Compare
//
template <typename Ch>
template <typename String>
inline typename std::enable_if<
    is_string_class<String>::value,
    int
>::type
    basic_string_splice<Ch>::compare(const String& str) const noexcept
{
    return common::compare(*this, str);
}

template <typename Ch>
template <typename String>
inline typename std::enable_if<
    is_string_class<String>::value,
    int
>::type
    basic_string_splice<Ch>::compare(
        size_type pos1, size_type n1, const String& str) const
{
    return common::compare(*this, pos1, n1, str);
}

template <typename Ch>
template <typename String>
inline typename std::enable_if<
    is_string_class<String>::value,
    int
>::type
    basic_string_splice<Ch>::compare(
        size_type pos1, size_type n1, const String& str,
                size_type pos2, size_type n2) const
{
    return common::compare(*this, pos1, n1, str, pos2, n2);
}

template <typename Ch>
inline int basic_string_splice<Ch>::compare(const_pointer s) const noexcept
{
    return common::compare(*this, s);
}

template <typename Ch>
inline int basic_string_splice<Ch>::compare(
    size_type pos1, size_type n1, const_pointer s) const
{
    return common::compare(*this, pos1, n1, s);
}

template <typename Ch>
inline int basic_string_splice<Ch>::compare(
    size_type pos1, size_type n1, const_pointer s, size_type n2) const
{
    return common::compare(*this, pos1, n1, s, n2);
}


//
// streams
//
template <typename Ch>
inline std::basic_ostream<Ch>& operator<< (std::basic_ostream<Ch>& ostr, const basic_string_splice<Ch>& s)
{
    ostr.write(s.data(), s.size());
    return ostr;
}

template <typename Ch>
inline std::basic_istream<Ch>& operator<< (std::basic_istream<Ch>& istr, const basic_string_splice<Ch>& s)
{
    std::string tmp;
    istr >> tmp;
    s = istr;
    return istr;
}


} // namespace native

#endif