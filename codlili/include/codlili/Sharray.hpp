/*
 * Created by Joshua Saxby <joshua.a.saxby@gmail.com>, June 2022
 * Copyright Joshua Saxby <joshua.a.saxby@gmail.com> 2022
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef COM_SAXBOPHONE_CODLILI_SHARRAY_HPP
#define COM_SAXBOPHONE_CODLILI_SHARRAY_HPP

#include <cstddef>          // size_t

#include <initializer_list> // initializer_list
#include <memory>           // allocator, allocator_traits


namespace com::saxbophone::codlili {
    /**
     * @brief A contiguous container type with both vector<> and deque<> semantics
     * @details sharray supports insertion at both the front and end, like
     * `std::deque<>`.
     * Like `std::vector<>`, sharray also stores its elements contiguously.
     * @tparam T the type of elements to store
     */
    template <typename T, class Allocator = std::allocator<T>>
    class sharray {
    public:
        using value_type = T;
        using allocator_type = Allocator;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using reference = T&;
        using const_reference = const T&;
        using pointer = typename std::allocator_traits<Allocator>::pointer;
        using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
        // using iterator = ...;
        // using const_iterator = ...;
        // using reverse_iterator = ...;
        // using const_reverse_iterator = ...;
        // member functions
        constexpr sharray();
        constexpr explicit sharray(const Allocator& alloc);
        constexpr sharray(
            size_type count,
            const T& value = T(),
            const Allocator& alloc = Allocator()
        );
        constexpr explicit sharray(
            size_type count, const Allocator& alloc = Allocator()
        );
        template<class InputIt>
        constexpr sharray(
            InputIt first, InputIt last, const Allocator& alloc = Allocator()
        );
        constexpr sharray(const sharray& other);
        constexpr sharray(const sharray& other, const Allocator& alloc);
        constexpr sharray(sharray&& other);
        constexpr sharray(sharray&& other, const Allocator& alloc);
        constexpr sharray(
            std::initializer_list<T> init, const Allocator& alloc = Allocator()
        );
        constexpr ~sharray();
        constexpr sharray& operator=(const sharray& other);
        constexpr sharray& operator=(sharray&& other) noexcept;
        constexpr sharray& operator=(std::initializer_list<T> ilist);
        constexpr void assign(size_type count, const T& value);
        template<class InputIt>
        constexpr void assign(InputIt first, InputIt last);
        constexpr void assign(std::initializer_list<T> ilist);
        constexpr allocator_type get_allocator() const noexcept;
        // element access
        constexpr reference at(size_type pos);
        constexpr const_reference at(size_type pos) const;
        constexpr reference operator[](size_type pos);
        constexpr const_reference operator[](size_type pos) const;
        constexpr reference front();
        constexpr const_reference front() const;
        constexpr reference back();
        constexpr const_reference back() const;
        constexpr const T* data() const noexcept;
        // iterators
        constexpr iterator begin() noexcept;
        constexpr const_iterator begin() const noexcept;
        constexpr const_iterator cbegin() const noexcept;
        constexpr iterator end() noexcept;
        constexpr const_iterator end() const noexcept;
        constexpr const_iterator cend() const noexcept;
        constexpr reverse_iterator rbegin() noexcept;
        constexpr const_reverse_iterator rbegin() const noexcept;
        constexpr const_reverse_iterator crbegin() const noexcept;
        constexpr reverse_iterator rend() noexcept;
        constexpr const_reverse_iterator rend() const noexcept;
        constexpr const_reverse_iterator crend() const noexcept;
        // capacity
        [[nodiscard]] constexpr bool empty() const noexcept;
        constexpr size_type size() const noexcept;
        constexpr size_type max_size() const noexcept;
        constexpr void reserve(size_type new_cap);
        constexpr size_type capacity() const noexcept;
        constexpr void shrink_to_fit();
        // modifiers
        constexpr void clear() noexcept;
        constexpr iterator insert(const_iterator pos, const T& value);
        constexpr iterator insert(const_iterator pos, T&& value);
        constexpr iterator insert(
            const_iterator pos, size_type count, const T& value
        );
        template<class InputIt>
        constexpr iterator insert(
            const_iterator pos, InputIt first, InputIt last
        );
        constexpr iterator insert(
            const_iterator pos, std::initializer_list<T> ilist
        );
        template<class... Args>
        constexpr iterator emplace(const_iterator pos, Args&&... args);
        constexpr iterator erase(const_iterator pos);
        constexpr iterator erase(const_iterator first, const_iterator last);
        constexpr void push_back(const T& value);
        constexpr void push_back(T&& value);
        template<class... Args>
        constexpr reference emplace_back(Args&&... args);
        constexpr void pop_back();
        constexpr void push_front(const T& value);
        constexpr void push_front(T&& value);
        template<class... Args>
        constexpr reference emplace_front(Args&&... args);
        constexpr void pop_front();
        constexpr void resize(size_type count);
        constexpr void resize(size_type count, const value_type& value);
        constexpr void swap(sharray& other) noexcept;
        // TODO: non-member functions
    private:
        /*
         * NOTES:
         * - _storage_size can be greater than _length
         * - _storage_size MUST NOT be less than _base_index + _length
         * - we initialise _base_index to be halfway through _storage
         * - every time we need to re-allocate storage to resize, we maintain
         * _base_index to be halfway through _storage
         * - we avoid the need to always resize on push_front by shifting
         * _base_index down to move the "start" of the array down by one element
         * and hence also avoid needing to move the other elements
         * IMPLEMENTATION NOTES:
         * - if we really want to, we can use std::span to simplify element
         * access by making an "elements" span for the subset of storage that is
         * actually in use, as `std::span<T>(_storage + _base_index, _length)`
         * - this "elements" span would need to be overwritten every time the
         * size of the container changes, whether that be _length, _storage_size
         * or _base_index that change.
         * - using std::span might not actually simplify the implementation, but
         * it is worth considering.
         * - IMPORTANT: we might need to have _storage as an array of bytes instead
         * and use placement new to construct new objects of T into it --otherwise
         * when we allocate space for new T objects (but without setting them),
         * instances of T that are default-constructed will be created, which
         * violates the implication that unused allocated storage is... well,
         * allocated but unused (i.e. allocated but not constructed).
         */
        T* _storage; // heap-allocated array for storing elements in
        std::size_t _storage_size; // the size of the heap-allocated array
        std::size_t _base_index; // 0-based index of first element in _storage to use
        std::size_t _length; // length of stored items
    };
}

#endif
