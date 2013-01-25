array2d
==========
(Note: This requires C++11)  

STL inspired implementaion of a two dimensional array.  
There are two main classes, **array2d** and **static\_array2d**.  
Both classes provide three types of iterators, all of which are random access iterators.  
The three types are:  

1. **iterator** and **const\_iterator**, for iterating over the entire data structure.  
2. **row\_iterator** and **const\_row\_iterator**, for iterating across a particular row.  
3. **column\_iterator** and **const\_column\_iterator**, for iterating down a particular column.  

## array2d ##
This is a heap allocated generic two dimensional array.  

`template <typename T>`  
`class array2d`  

T must be default constructible

### Public Typedefs ###

* value_type
* pointer
* const\_pointer
* reference
* const\_reference
* size\_type
* iterator
* const\_iterator
* row\_iterator
* const\_row\_iterator
* column\_iterator
* const\_column\_iterator

### Public Member Functions ###

* `array2d() = delete`  
_array2d is not default constructible_
* `array2d(size_t width, size_t height)`  
* `~array2d()`
* `array2d(const array2d&)`
* `array2d& operator=(const array2d&)`
* `array2d(array2d&&)`
* `array2d& operator=(array2d&&)`
* `size_type width() const`
* `size_type height() const`

##### Iterator Member Functions #####

* `iterator begin()`
* `const_iterator begin() const`
* `iterator end()`
* `const_iterator end() const`
* `row_iterator row_begin(size_type y)`
* `const_row_iterator row_begin(size_type y) const`
* `row_iterator row_end(size_type y)`
* `const_row_iterator row_end(size_type y) const`
* `column_iterator column_begin(size_type x)`
* `const_column_iterator column_begin(size_type x) const`
* `column_iterator column_end(size_type x)`
* `const_column_iterator column_end(size_type x) const`

##### Direct Element Access Functions #####

* `reference index(size_type x, size_type y)`
* `const_reference index(size_type x, size_type y) const`
* `reference operator()(size_type x, size_type y)`
* `const_reference operator()(size_type x, size_type y) const`

##### In Place Element Construction #####

* `template <typename... Args>`  
`iterator emplace(size_type x, size_type y, Args&&... args)`
* `template <typename... Args>`  
`iterator emplace(iterator pos, Args&&... args)`
* `template <typename... Args>`  
`column_iterator emplace(column_iterator pos, Args&&... args)`

----------
## static\_array2d ##

This is a stack allocated generic two dimensional array.

`template <typename T, std::size_t Width, std::size_t Height>`  
`class static_array2d`  

T must be default and copy constructible.

### Public Typedefs ###

* value_type
* pointer
* const\_pointer
* reference
* const\_reference
* size\_type
* iterator
* const\_iterator
* row\_iterator
* const\_row\_iterator
* column\_iterator
* const\_column\_iterator

### Public Member Constants ###

* width
* height
* size

### Public Member Functions ###

* `static_array2d()`  
* `~static_array2d()`
* `static_array2d(const static_array2d&)`
* `static_array2d& operator=(const static_array2d&)`
* `static_array2d(array2d&&)`
* `static_array2d& operator=(array2d&&)`

##### Iterator Member Functions #####

* `iterator begin()`
* `const_iterator begin() const`
* `iterator end()`
* `const_iterator end() const`
* `row_iterator row_begin(size_type y)`
* `const_row_iterator row_begin(size_type y) const`
* `row_iterator row_end(size_type y)`
* `const_row_iterator row_end(size_type y) const`
* `column_iterator column_begin(size_type x)`
* `const_column_iterator column_begin(size_type x) const`
* `column_iterator column_end(size_type x)`
* `const_column_iterator column_end(size_type x) const`

##### Direct Element Access Functions #####

* `reference index(size_type x, size_type y)`
* `const_reference index(size_type x, size_type y) const`
* `reference operator()(size_type x, size_type y)`
* `const_reference operator()(size_type x, size_type y) const`

##### In Place Element Construction #####

* `template <typename... Args>`  
`iterator emplace(size_type x, size_type y, Args&&... args)`
* `template <typename... Args>`  
`iterator emplace(iterator pos, Args&&... args)`
* `template <typename... Args>`  
`column_iterator emplace(column_iterator pos, Args&&... args)`
