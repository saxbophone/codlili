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

#include <cstddef> // size_t


namespace com::saxbophone::codlili {
    /**
     * @brief A contiguous container type with both vector<> and deque<> semantics
     * @details Sharray supports insertion at both the front and end, like `std::deque<>`.
     * Like `std::vector<>`, Sharray also stores its elements contiguously.
     * @tparam T the type of elements to store
     */
    template <typename T>
    class Sharray {
    public:
        using reference = T&;
        using const_reference = const T&;
        // NOTE: not all public methods intended for implementation are shown
        constexpr Sharray();
        constexpr ~Sharray();
        constexpr reference operator[](std::size_t pos);
        constexpr const_reference operator[](std::size_t pos) const;
        constexpr bool empty() const noexcept;
        constexpr std::size_t size() const noexcept;
        constexpr std::size_t max_size() const noexcept;
        constexpr void reserve(std::size_t new_cap);
        constexpr std::size_t capacity() const noexcept;
        constexpr void shrink_to_fit();
        constexpr void push_front(const_reference value);
        constexpr void push_back(const_reference value);
        constexpr void pop_front();
        constexpr void pop_back();
        constexpr void resize(std::size_t count);
        constexpr void resize(std::size_t count, const_reference value);
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
