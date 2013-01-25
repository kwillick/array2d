/*
 array2d.hpp - STL-like generic two dimensional array, with both stack allocated
               and heap allocated variants.

 Copyright (c) 2013, Kipp Hickman
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
  * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\
*/

#ifndef ARRAY2D_H
#define ARRAY2D_H

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>

namespace array2d
{

template <typename T, typename Pointer, typename Reference>
struct array2d_iterator
{
    typedef std::ptrdiff_t                      difference_type;
    typedef std::random_access_iterator_tag     iterator_category;
    typedef T                                   value_type;
    typedef Pointer                             pointer;
    typedef Reference                           reference;

  
    pointer _m_pos;
    
    array2d_iterator() : _m_pos(nullptr) { }
    array2d_iterator(pointer pos) : _m_pos(pos) { }

    //allow non-const to const conversion
    template <typename P, typename R>
    array2d_iterator(const array2d_iterator<T, P, R>& o,
                     typename std::enable_if<
                         std::is_convertible<P, pointer>::value
                     >::type* = nullptr)
        : _m_pos(o._m_pos)
    { }

    reference operator*() const { return *_m_pos; }
    pointer operator->() const { return _m_pos; }

    array2d_iterator& operator++() { ++_m_pos; return *this; }
    array2d_iterator operator++(int) { return array2d_iterator(_m_pos++); }

    array2d_iterator& operator--() { --_m_pos; return *this; }
    array2d_iterator operator--(int) { return array2d_iterator(_m_pos--); }

    array2d_iterator& operator+=(difference_type n)
    { _m_pos += n; return *this; }

    array2d_iterator& operator-=(difference_type n)
    { _m_pos -= n; return *this; }

    array2d_iterator operator+(difference_type n) const
    { return array2d_iterator(_m_pos + n); }

    array2d_iterator operator-(difference_type n) const
    { return array2d_iterator(_m_pos - n); }

    reference operator[](difference_type n) const
    { return *(_m_pos + n); }

    bool operator==(const array2d_iterator& o) const
    { return _m_pos == o._m_pos; }
    bool operator!=(const array2d_iterator& o) const
    { return _m_pos != o._m_pos; }
    
    bool operator<(const array2d_iterator& o) const
    { return _m_pos < o._m_pos; }
    bool operator>(const array2d_iterator& o) const
    { return _m_pos > o._m_pos; }

    bool operator<=(const array2d_iterator& o) const
    { return !(_m_pos > o._m_pos); }
    bool operator>=(const array2d_iterator& o) const
    { return !(_m_pos < o._m_pos); }
};


template <typename T, typename Pointer, typename Reference>
struct array2d_column_iterator
{
    typedef std::ptrdiff_t                      difference_type;
    typedef std::random_access_iterator_tag     iterator_category;
    typedef T                                   value_type;
    typedef Pointer                             pointer;
    typedef Reference                           reference;

    pointer _m_pos;
    std::size_t _m_width;

    array2d_column_iterator() : _m_pos(nullptr), _m_width(-1) { }
    array2d_column_iterator(pointer pos, std::size_t width)
        : _m_pos(pos), _m_width(width) { }

    //allow non-const to const conversion
    template <typename P, typename R>
    array2d_column_iterator(const array2d_column_iterator<T, P, R>& o,
                            const typename std::enable_if<
                                std::is_convertible<P, pointer>::value
                            >::type* = nullptr)
        : _m_pos(o._m_pos), _m_width(o._m_width)
    { }

    reference operator*() const { return *_m_pos; }
    pointer operator->() const { return _m_pos; }

    array2d_column_iterator& operator++() { _m_pos += _m_width; return *this; }
    array2d_column_iterator operator++(int)
    {
        array2d_column_iterator tmp(*this);
        _m_pos += _m_width;
        return tmp;
    }

    array2d_column_iterator& operator--() { _m_pos -= _m_width; return *this; }
    array2d_column_iterator operator--(int)
    {
        array2d_column_iterator tmp(*this);
        _m_pos -= _m_width;
        return tmp;
    }

    array2d_column_iterator& operator+=(difference_type n)
    {
        _m_pos += (n * _m_width);
        return *this;
    }

    array2d_column_iterator& operator-=(difference_type n)
    {
        _m_pos -= (n * _m_width);
        return *this;
    }

    array2d_column_iterator operator+(difference_type n) const 
    { return array2d_column_iterator(_m_pos + n * _m_width, _m_width); }

    array2d_column_iterator operator-(difference_type n) const
    { return array2d_column_iterator(_m_pos - n * _m_width, _m_width); }

    reference operator[](difference_type n) const
    { return *(_m_pos + n * _m_width); }

    bool operator==(const array2d_column_iterator& o) const
    { return _m_pos == o._m_pos; }
    bool operator!=(const array2d_column_iterator& o) const
    { return _m_pos != o._m_pos; }

    bool operator<(const array2d_column_iterator& o) const
    { return _m_pos < o._m_pos; }
    bool operator>(const array2d_column_iterator& o) const
    { return _m_pos > o._m_pos; }

    bool operator<=(const array2d_column_iterator& o) const
    { return !(_m_pos > o._m_pos); }

    bool operator>=(const array2d_column_iterator& o) const
    { return !(_m_pos < o._m_pos); }
};


#define ARRAY2D_ITER_COMPARE_OP(op, iter)                               \
    template <typename T>                                               \
    inline bool operator op(const iter<T, T*, T&>& a,                   \
                            const iter<T, const T*, const T&>& b)       \
    { return a._m_pos op b._m_pos; }                                    \
                                                                        \
    template <typename T>                                               \
    inline bool operator op(const iter<T, const T*, const T&>& a,       \
                            const iter<T, T*, T&>& b)                   \
    { return a._m_pos op b._m_pos; }

#define ARRAY2D_ITER_COMPARE_OP_GTE_LTE(op1, op2, iter)                 \
    template <typename T>                                               \
    inline bool operator op1(const iter<T, T*, T&>& a,                  \
                             const iter<T, const T*, const T&>& b)      \
    { return !(a._m_pos op2 b._m_pos); }                                \
                                                                        \
    template <typename T>                                               \
    inline bool operator op1(const iter<T, const T*, const T&>& a,      \
                             const iter<T, T*, T&>& b)                  \
    { return !(a._m_pos op2 b._m_pos); }
  

#define ARRAY2D_ITER_COMPARE(iter)               \
    ARRAY2D_ITER_COMPARE_OP(==, iter)            \
    ARRAY2D_ITER_COMPARE_OP(!=, iter)            \
    ARRAY2D_ITER_COMPARE_OP(<, iter)             \
    ARRAY2D_ITER_COMPARE_OP(>, iter)             \
    ARRAY2D_ITER_COMPARE_OP_GTE_LTE(<=, >, iter) \
    ARRAY2D_ITER_COMPARE_OP_GTE_LTE(>=, <, iter)

ARRAY2D_ITER_COMPARE(array2d_iterator)
ARRAY2D_ITER_COMPARE(array2d_column_iterator)

#undef ARRAY2D_ITER_COMPARE
#undef ARRAY2D_ITER_COMPARE_OP_GTE_LTE
#undef ARRAY2D_ITER_COMPARE_OP


template <typename T, typename Pointer, typename Reference, std::size_t Width>
struct static_array2d_column_iterator
{
    typedef std::ptrdiff_t                      difference_type;
    typedef std::random_access_iterator_tag     iterator_category;
    typedef T                                   value_type;
    typedef Pointer                             pointer;
    typedef Reference                           reference;

    pointer _m_pos;

    static_array2d_column_iterator() : _m_pos(nullptr) { }
    static_array2d_column_iterator(pointer pos) : _m_pos(pos) { }

    //allow non-const to const conversion
    template <typename P, typename R>
    static_array2d_column_iterator(const static_array2d_column_iterator<T, P, R, Width>& o,
                                   typename std::enable_if<
                                       std::is_convertible<P, pointer>::value
                                   >::type* = nullptr)
        : _m_pos(o._m_pos)
    { }

    reference operator*() const { return *_m_pos; }
    pointer operator->() const { return _m_pos; }

    static_array2d_column_iterator& operator++() { _m_pos += Width; return *this; }
    static_array2d_column_iterator& operator++(int)
    {
        static_array2d_column_iterator tmp(*this);
        _m_pos += Width;
        return tmp;
    }

    static_array2d_column_iterator& operator--() { _m_pos -= Width; return *this; }
    static_array2d_column_iterator operator--(int)
    {
        static_array2d_column_iterator tmp(*this);
        _m_pos -= Width;
        return tmp;
    }

    static_array2d_column_iterator& operator+=(difference_type n)
    {
        _m_pos += (n * Width);
        return *this;
    }

    static_array2d_column_iterator& operator-=(difference_type n)
    {
        _m_pos -= (n * Width);
        return *this;
    }

    static_array2d_column_iterator operator+(difference_type n)
    { return static_array2d_column_iterator(_m_pos + n * Width); }

    static_array2d_column_iterator operator-(difference_type n)
    { return static_array2d_column_iterator(_m_pos - n * Width); }

    reference operator[](difference_type n)
    { return *(_m_pos + n * Width); }

    bool operator==(const static_array2d_column_iterator& o) const
    { return _m_pos == o._m_pos; }
    bool operator!=(const static_array2d_column_iterator& o) const
    { return _m_pos != o._m_pos; }

    bool operator<(const static_array2d_column_iterator& o) const
    { return _m_pos < o._m_pos; }
    bool operator>(const static_array2d_column_iterator& o) const
    { return _m_pos > o._m_pos; }

    bool operator<=(const static_array2d_column_iterator& o) const
    { return !(_m_pos > o._m_pos); }

    bool operator>=(const static_array2d_column_iterator& o) const
    { return !(_m_pos < o._m_pos); }
};

#define ARRAY2D_ITER_COMPARE_OP(op, iter)                               \
    template <typename T, std::size_t W>                                \
    inline bool operator op(const iter<T, T*, T&, W>& a,                \
                            const iter<T, const T*, const T&, W>& b)    \
    { return a._m_pos op b._m_pos; }                                    \
                                                                        \
    template <typename T, std::size_t W>                                \
    inline bool operator op(const iter<T, const T*, const T&, W>& a,    \
                            const iter<T, T*, T&, W>& b)                \
    { return a._m_pos op b._m_pos; }

#define ARRAY2D_ITER_COMPARE_OP_GTE_LTE(op1, op2, iter)                 \
    template <typename T, std::size_t W>                                \
    inline bool operator op1(const iter<T, T*, T&, W>& a,               \
                             const iter<T, const T*, const T&, W>& b)   \
    { return !(a._m_pos op2 b._m_pos); }                                \
                                                                        \
    template <typename T, std::size_t W>                                \
    inline bool operator op1(const iter<T, const T*, const T&, W>& a,   \
                             const iter<T, T*, T&, W>& b)               \
    { return !(a._m_pos op2 b._m_pos); }

ARRAY2D_ITER_COMPARE_OP(==, static_array2d_column_iterator)
ARRAY2D_ITER_COMPARE_OP(!=, static_array2d_column_iterator)
ARRAY2D_ITER_COMPARE_OP(<, static_array2d_column_iterator)
ARRAY2D_ITER_COMPARE_OP(>, static_array2d_column_iterator)
ARRAY2D_ITER_COMPARE_OP_GTE_LTE(<=, >, static_array2d_column_iterator)
ARRAY2D_ITER_COMPARE_OP_GTE_LTE(>=, <, static_array2d_column_iterator)

#undef ARRAY2D_ITER_COMPARE_OP
#undef ARRAY2D_ITER_COMPARE_OP_GTE_LTE


template <typename Iter,
          bool Trivial = std::is_trivially_destructible<typename Iter::value_type>::value
          >
struct emplacer
{
    typedef typename Iter::pointer    pointer;
    typedef typename Iter::value_type value_type;

    template <typename... Args>
    static inline Iter emplace(pointer pos, Args&&... args)
    {
        new (pos) value_type(std::forward<Args>(args)...);
        return Iter(pos);
    }
};

template <typename Iter>
struct emplacer<Iter, false>
{
    typedef typename Iter::pointer    pointer;
    typedef typename Iter::value_type value_type;

    template <typename... Args>
    static inline Iter emplace(pointer pos, Args&&... args)
    {
        pos->~value_type();
        new (pos) value_type(std::forward<Args>(args)...);
        return Iter(pos);
    }
};


template <typename T>
class array2d
{
  public:
    typedef T                   value_type;
    typedef T*                  pointer;
    typedef const T*            const_pointer;
    typedef T&                  reference;
    typedef const T&            const_reference;
    typedef std::size_t         size_type;
    typedef std::ptrdiff_t      difference_type;

    typedef array2d_iterator<T, T*, T&>             iterator;
    typedef array2d_iterator<T, const T*, const T&> const_iterator;

    //iterates down a column
    typedef array2d_column_iterator<T, T*, T&>             column_iterator;
    typedef array2d_column_iterator<T, const T*, const T&> const_column_iterator;

    //iterates across a row
    typedef iterator            row_iterator;
    typedef const_iterator      const_row_iterator;
    
  private:        
    size_type m_width;
    size_type m_height;
    T* m_data;
    
  public:
    array2d() = delete;

    array2d(size_type width,
            size_type height)
        : m_width(width), 
          m_height(height),
          m_data(new T[m_width * m_height]) 
    { }

    ~array2d() { delete[] m_data; }

    array2d(array2d&& o)
        : m_width(o.m_width), 
          m_height(o.m_height),
          m_data(o.m_data)
    { o.m_data = nullptr; }

    array2d(const array2d& o)
        : m_width(o.m_width),
          m_height(o.m_height),
          m_data(reinterpret_cast<T*>(new char[o.m_width * o.m_height * sizeof(T)]))
    {
        std::uninitialized_copy_n(o.m_data, m_width * m_height, m_data);
    }
    
    array2d& operator=(const array2d& o)
    {
        m_data = reinterpret_cast<T*>(new char[o.m_width * o.m_height * sizeof(T)]);
        std::uninitialized_copy_n(o.m_data, m_width * m_height, m_data);
        m_width = o.m_width;
        m_height = o.m_height;

        return *this;
    }

    array2d& operator=(array2d&& o)
    {
        m_data = o.m_data;
        o.m_data = nullptr;
        m_width = o.m_width;
        m_height = o.m_height;

        return *this;
    }

    size_type width() const { return m_width; }
    size_type height() const { return m_height; }

    iterator begin() { return iterator(m_data); }
    const_iterator begin() const { return const_iterator(m_data); }

    iterator end() { return iterator(m_data +  m_width * m_height); }
    const_iterator end() const { return const_iterator(m_data + m_width * m_height); }

    row_iterator row_begin(size_type y) { return row_iterator(m_data + y * m_width); }
    const_row_iterator row_begin(size_type y) const
    { return const_row_iterator(m_data + y * m_width); }

    row_iterator row_end(size_type y)
    { return row_iterator(m_data + y * m_width + m_width); }
    const_row_iterator row_end(size_type y) const
    { return const_row_iterator(m_data + y * m_width + m_width); }
    
    column_iterator column_begin(size_type x)
    { return column_iterator(m_data + x, m_width); }
    const_column_iterator column_begin(size_type x) const
    { return const_column_iterator(m_data + x, m_width); }

    column_iterator column_end(size_type x)
    { return column_iterator(m_data + m_height * m_width + x, m_width); }
    const_column_iterator column_end(size_type x) const
    { return const_column_iterator(m_data + m_height * m_width + x, m_width); }
    
    reference index(size_type x, size_type y)
    { return m_data[y * m_width + x]; }
    const_reference index(size_type x, size_type y) const 
    { return m_data[y * m_width + x]; }

    reference operator()(size_type x, size_type y)
    { return m_data[y * m_width + x]; }
    const_reference operator()(size_type x, size_type y) const 
    { return m_data[y * m_width + x]; }

    template <typename... Args>
    iterator emplace(size_type x, size_type y, Args&&... args)
    {
        typedef emplacer<iterator> emplacer;
        pointer pos = m_data + y * m_width + x;
        return emplacer::emplace(pos, std::forward<Args>(args)...);
    }
                 
    template <typename... Args>
    iterator emplace(iterator pos, Args&&... args)
    {
        typedef emplacer<iterator> emplacer;
        return emplacer::emplace(pos._m_pos, std::forward<Args>(args)...);
    }

    template <typename... Args>
    column_iterator emplace(column_iterator pos, Args&&... args)
    {
        typedef emplacer<column_iterator> emplacer;
        return emplacer::emplace(pos._m_pos, std::forward<Args>(args)...);
    }
};


template <typename T, std::size_t Width, std::size_t Height>
class static_array2d
{
  public:
    typedef T                   value_type;
    typedef T*                  pointer;
    typedef const T*            const_pointer;
    typedef T&                  reference;
    typedef const T&            const_reference;
    typedef std::size_t         size_type;
    typedef std::ptrdiff_t      difference_type;

    typedef array2d_iterator<T, T*, T&>             iterator;
    typedef array2d_iterator<T, const T*, const T&> const_iterator;

    //iterates down a column
    typedef static_array2d_column_iterator<T, T*, T&, Width>             column_iterator;
    typedef static_array2d_column_iterator<T, const T*, const T&, Width> const_column_iterator;

    //iterates across a row
    typedef iterator            row_iterator;
    typedef const_iterator      const_row_iterator;

  public:
    static constexpr size_type width = Width;
    static constexpr size_type height = Height;
    static constexpr size_type size = Width * Height * sizeof(T);

  private:
    T m_data[size];

  public:
    static_array2d() { }
    ~static_array2d() { }

    static_array2d(const static_array2d& o)
    {
        std::copy(o.begin(), o.end(), begin());
    }

    static_array2d& operator=(const static_array2d& o)
    {
        std::copy(o.begin(), o.end(), begin());
        return *this;
    }

    iterator begin() { return iterator(m_data); }
    const_iterator begin() const { return const_iterator(m_data); }

    iterator end() { return iterator(m_data +  Width * Height); }
    const_iterator end() const { return const_iterator(m_data + Width * Height); }

    row_iterator row_begin(size_type y) { return row_iterator(m_data + y * Width); }
    const_row_iterator row_begin(size_type y) const
    { return const_row_iterator(m_data + y * Width); }

    row_iterator row_end(size_type y) { return row_iterator(m_data + y * Width + Width); }
    const_row_iterator row_end(size_type y) const
    { return const_row_iterator(m_data + y * Width + Width); }
    
    column_iterator column_begin(size_type x)
    { return column_iterator(m_data + x); }
    const_column_iterator column_begin(size_type x) const
    { return const_column_iterator(m_data + x); }

    column_iterator column_end(size_type x)
    { return column_iterator(m_data + Height * Width + x); }
    const_column_iterator column_end(size_type x) const
    { return const_column_iterator(m_data + Height * Width + x); }

    reference index(size_type x, size_type y)
    { return m_data[y * Width + x]; }
    const_reference index(size_type x, size_type y) const
    { return m_data[y * Width + x]; }

    reference operator()(size_type x, size_type y)
    { return m_data[y * Width + x]; }
    const_reference operator()(size_type x, size_type y) const
    { return m_data[y * Width + x]; }

    template <typename... Args>
    iterator emplace(size_type x, size_type y, Args&&... args)
    {
        typedef emplacer<iterator> emplacer;
        pointer pos = m_data + y * Width + x;
        return emplacer::emplace(pos, std::forward<Args>(args)...);
    }
                 
    template <typename... Args>
    iterator emplace(iterator pos, Args&&... args)
    {
        typedef emplacer<iterator> emplacer;
        return emplacer::emplace(pos._m_pos, std::forward<Args>(args)...);
    }

    template <typename... Args>
    column_iterator emplace(column_iterator pos, Args&&... args)
    {
        typedef emplacer<column_iterator> emplacer;
        return emplacer::emplace(pos._m_pos, std::forward<Args>(args)...);
    }
};

} //array2d

#endif //ARRAY2D_H
