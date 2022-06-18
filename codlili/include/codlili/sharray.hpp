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
#include <limits>           // numeric_limits
#include <memory>           // allocator, allocator_traits
#include <span>             // span
#include <utility>          // pair


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
        using iterator = std::span<T>::iterator;
        using const_iterator = std::span<const T>::iterator;
        using reverse_iterator = std::span<T>::reverse_iterator;
        using const_reverse_iterator = std::span<const T>::reverse_iterator;
        // member functions
        constexpr sharray() {}
        constexpr explicit sharray(const Allocator& alloc) : _allocator(alloc) {}

        constexpr sharray(
            size_type count,
            const T& value = T(),
            const Allocator& alloc = Allocator()
        )
          : _allocator(alloc)
          , _storage(TAllocator::allocate(_allocator, count), count)
          , _size(count)
          {
            for (size_type i = 0; i < _size; i++) {
                TAllocator::construct(_allocator, _storage.data() + i, value);
            }
        }

        constexpr explicit sharray(
            size_type count, const Allocator& alloc = Allocator()
        )
          : sharray(count, T(), alloc) {} // default-inserted elements of T

        template<class InputIt>
        constexpr sharray(
            InputIt first, InputIt last, const Allocator& alloc = Allocator()
        )
          : _allocator(alloc) {}

        constexpr sharray(const sharray& other) {}
        constexpr sharray(const sharray& other, const Allocator& alloc)
          : _allocator(alloc) {}

        constexpr sharray(sharray&& other) {}
        constexpr sharray(sharray&& other, const Allocator& alloc)
          : _allocator(alloc) {}

        constexpr sharray(
            std::initializer_list<T> init, const Allocator& alloc = Allocator()
        )
          : _allocator(alloc)
          , _storage(TAllocator::allocate(_allocator, init.size()), init.size())
          , _size(init.size())
          {
            auto it = init.begin();
            for (size_type i = 0; i < _size; i++) {
                TAllocator::construct(_allocator, _storage.data() + i, *it);
                it++;
            }
        }

        constexpr ~sharray() {
            // destroy any constructed objects first
            for (size_type i = _base_index; i < _storage.size(); i++) {
                TAllocator::destroy(_allocator, _storage.data() + i);
            }
            // then deallocate all memory
            TAllocator::deallocate(_allocator, _storage.data(), _storage.size());
        }

        constexpr sharray& operator=(const sharray& other) { return *this; }
        constexpr sharray& operator=(sharray&& other) noexcept { return *this; }
        constexpr sharray& operator=(std::initializer_list<T> ilist) { return *this; }
        constexpr void assign(size_type count, const T& value) {}
        template<class InputIt>
        constexpr void assign(InputIt first, InputIt last) {}
        constexpr void assign(std::initializer_list<T> ilist) {}
        constexpr allocator_type get_allocator() const noexcept { return _allocator; }
        // element access
        constexpr reference at(size_type pos);
        constexpr const_reference at(size_type pos) const;
        constexpr reference operator[](size_type pos) {
            return _elements()[pos];
        }
        constexpr const_reference operator[](size_type pos) const {
            return _elements()[pos];
        }
        constexpr reference front() { return _elements().front(); }
        constexpr const_reference front() const { return _elements().front(); }
        constexpr reference back() { return _elements().back(); }
        constexpr const_reference back() const { return _elements().back(); }
        constexpr const T* data() const noexcept { return _elements().data(); }
        // iterators
        constexpr iterator begin() noexcept { return _elements().begin(); }
        constexpr const_iterator begin() const noexcept { return _elements().begin(); }
        constexpr const_iterator cbegin() const noexcept { return _elements().begin(); }
        constexpr iterator end() noexcept { return _elements().end(); }
        constexpr const_iterator end() const noexcept { return _elements().end(); }
        constexpr const_iterator cend() const noexcept { return _elements().end(); }
        constexpr reverse_iterator rbegin() noexcept { return _elements().rbegin(); }
        constexpr const_reverse_iterator rbegin() const noexcept { return _elements().rbegin(); }
        constexpr const_reverse_iterator crbegin() const noexcept { return _elements().rbegin(); }
        constexpr reverse_iterator rend() noexcept { return _elements().rend(); }
        constexpr const_reverse_iterator rend() const noexcept { return _elements().rend(); }
        constexpr const_reverse_iterator crend() const noexcept { return _elements().rend(); }
        // capacity
        [[nodiscard]] constexpr bool empty() const noexcept {
            return _elements().empty();
        }
        constexpr size_type size() const noexcept { return _elements().size(); }
        constexpr size_type max_size() const noexcept {
            return std::numeric_limits<difference_type>::max();
        }
        constexpr void reserve(size_type new_cap) {}
        // pair of sizes for cap denotes elements to reserve before and after front
        constexpr void reserve(std::pair<size_type, size_type> bidir_cap) {}
        constexpr size_type capacity() const noexcept { return _storage.size(); }
        constexpr void shrink_to_fit() {}
        // modifiers
        constexpr void clear() noexcept {}
        constexpr iterator insert(const_iterator pos, const T& value) { return _elements().begin(); } // XXX: stub
        constexpr iterator insert(const_iterator pos, T&& value) { return _elements().begin(); } // XXX: stub
        constexpr iterator insert(
            const_iterator pos, size_type count, const T& value
        ) { return _elements().begin(); } // XXX: stub
        template<class InputIt>
        constexpr iterator insert(
            const_iterator pos, InputIt first, InputIt last
        ) { return _elements().begin(); } // XXX: stub
        constexpr iterator insert(
            const_iterator pos, std::initializer_list<T> ilist
        ) { return _elements().begin(); } // XXX: stub
        template<class... Args>
        constexpr iterator emplace(const_iterator pos, Args&&... args) { return _elements().begin(); } // XXX: stub
        constexpr iterator erase(const_iterator pos);
        constexpr iterator erase(const_iterator first, const_iterator last);
        constexpr void push_back(const T& value) {}
        constexpr void push_back(T&& value) {}
        template<class... Args>
        constexpr reference emplace_back(Args&&... args);
        constexpr void pop_back() {}
        constexpr void push_front(const T& value) {}
        constexpr void push_front(T&& value) {}
        template<class... Args>
        constexpr reference emplace_front(Args&&... args);
        constexpr void pop_front() {}
        constexpr void resize(size_type count) {}
        constexpr void resize(size_type count, const value_type& value) {}
        // pair of counts is defined as number to have before the front of the array and the number to have after it
        constexpr void resize(std::pair<size_type, size_type> count) {}
        constexpr void resize(std::pair<size_type, size_type> count, const value_type& value) {}
        constexpr void swap(sharray& other) noexcept {}
        // comparison
        constexpr bool operator==(const sharray& other) const {
            if (_size != other._size) { return false; }
            for (size_type i = 0; i < _size; i++) {
                if ((*this)[i] != other[i]) { return false; }
            }
            return true;
        }
    private:
        // type used for allocating storage for T (in case the Allocator passed is for a different type)
        using TAllocator = std::allocator_traits<Allocator>::template rebind_traits<T>;
        // accessors to the actual elements in the sharray, using span as a shortcut
        constexpr std::span<T> _elements() {
            return _storage.subspan(_base_index, _size);
        }
        constexpr std::span<const T> _elements() const {
            return _storage.subspan(_base_index, _size);
        }
        allocator_type _allocator = Allocator();
        /*
         * NOTES:
         * - _storage_size can be greater than _size
         * - _storage_size MUST NOT be less than _base_index + _size
         * - we initialise _base_index to be halfway through _storage
         * - every time we need to re-allocate storage to resize, we maintain
         * _base_index to be halfway through _storage
         * - we avoid the need to always resize on push_front by shifting
         * _base_index down to move the "start" of the array down by one element
         * and hence also avoid needing to move the other elements
         * IMPLEMENTATION NOTES:
         * - if we really want to, we can use std::span to simplify element
         * access by making an "elements" span for the subset of storage that is
         * actually in use, as `std::span<T>(_storage + _base_index, _size)`
         * - this "elements" span would need to be overwritten every time the
         * size of the container changes, whether that be _size, _storage_size
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
        std::span<T> _storage = {}; // heap-allocated array for storing elements in and its size, wrapped in a span
        std::size_t _base_index = 0; // 0-based index of first element in _storage to use
        std::size_t _size = 0; // number of stored items
    };
}

#endif
