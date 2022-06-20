#include <cstddef>

#include <vector>

#include <catch2/catch_all.hpp>

#include <codlili/sharray.hpp>


using namespace com::saxbophone::codlili;

TEST_CASE("sharray mirrors the API of vector and deque") {
    SECTION("default constructor") {
        auto array = sharray<int>();

        CHECK(array.size() == 0);
        CHECK(array.capacity() == 0); // not sure if this is standard-compliant
    }
    SECTION("size constructor") {
        auto array = sharray<int>(32);
        CHECK(array.size() == 32);
        CHECK(array.capacity() >= 32);
    }
    SECTION("list constructor") {
        sharray<int> array = {9, 6, 7, 2, 3, 5, 8, 99};

        int expected[] = {9, 6, 7, 2, 3, 5, 8, 99};

        for (std::size_t i = 0; i < array.size(); i++) {
            CHECK(array[i] == expected[i]);
        }
    }
    SECTION("copy constructor") {
        sharray<int> array = {1, 2, 3 ,13, 4, 7, 9, 15};

        auto other = array;

        CHECK(other == array);
    }
    SECTION("move constructor") {
        sharray<int> array = sharray<int>({9, 5, 7, 1, 2, 2, 4, 2});

        CHECK(array == sharray<int>({9, 5, 7, 1, 2, 2, 4, 2}));
    }
    // TODO: allocator-aware ctors
    SECTION("copy assignment") {
        sharray<int> array;
        sharray<int> other = {9, 5, 7, 1, 2, 1, 6, 3, 41, 13};

        array = other;

        CHECK(array == other);
    }
    SECTION("move assignment") {
        sharray<int> array;

        array = sharray<int>({9, 5, 7, 1, 2, 1, 6, 3, 41, 13});

        CHECK(array == sharray<int>({9, 5, 7, 1, 2, 1, 6, 3, 41, 13}));
    }
    SECTION("list assignment") {
        sharray<int> array;

        array = {1, 2, 3, 4};

        CHECK(array == sharray<int>({1, 2, 3, 4}));
    }
    // TODO: .assign() methods
    // TODO: .get_allocator()
    // TODO: .at()
    SECTION("[] operator (read)") {
        const sharray<int> array = {9, 5, 6, 7, 2, 1, 3, 4, 1, 1, 17, 0};

        CHECK(array[5] == 1);
    }
    SECTION("[] operator (read+write)") {
        sharray<int> array = {9, 5, 6, 7, 2, 1, 3, 4, 1, 1, 17, 0};

        array[7] = array[3] * 4;

        CHECK(array[7] == 28);
    }
    SECTION(".front() read") {
        const sharray<int> array = {1, 2, 3, 4};

        CHECK(array.front() == 1);
    }
    SECTION(".front() write") {
        sharray<int> array = {1, 2, 3, 4};

        array.front() = 62;

        CHECK(array.front() == 62);
    }
    SECTION(".back() read") {
        const sharray<int> array = {1, 2, 3, 4};

        CHECK(array.back() == 4);
    }
    SECTION(".back() write") {
        sharray<int> array = {1, 2, 3, 4};

        array.back() = 13;

        CHECK(array.back() == 13);
    }
    SECTION(".begin() and .end() iterators") {
        sharray<int> array = {1, 2, 3, 4, 5, 6, 7, 8};

        std::vector<int> other(array.begin(), array.end());

        CHECK(other == std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8}));
    }
    SECTION(".cbegin() and .cend() const iterators") {
        sharray<int> array = {1, 2, 3, 4, 5, 6, 7, 8};

        std::vector<int> other(array.cbegin(), array.cend());

        CHECK(other == std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8}));
    }
    SECTION(".rbegin() and .rend() reverse iterators") {
        sharray<int> array = {1, 2, 3, 4, 5, 6, 7, 8};

        std::vector<int> other(array.rbegin(), array.rend());

        CHECK(other == std::vector<int>({8, 7, 6, 5, 4, 3, 2, 1}));
    }
    SECTION(".crbegin() and .crend() const reverse iterators") {
        sharray<int> array = {1, 2, 3, 4, 5, 6, 7, 8};

        std::vector<int> other(array.crbegin(), array.crend());

        CHECK(other == std::vector<int>({8, 7, 6, 5, 4, 3, 2, 1}));
    }
    SECTION(".empty()") {
        sharray<int> empty;
        sharray<int> non_empty(42);

        CHECK(empty.empty());
        CHECK_FALSE(non_empty.empty());
    }
    SECTION(".size()") {
        std::size_t size = (std::size_t)GENERATE(0, 1, 2, 3, 4, 5);
        sharray<int> array(size);

        CHECK(array.size() == size);
    }
    SECTION(".max_size()") {
        CHECK(typeid(sharray<int>().max_size()) == typeid(sharray<int>::size_type));
    }
    SECTION(".reserve()") {
        std::size_t size = (std::size_t)GENERATE(0, 10, 20, 300, 4000, 50000);
        sharray<int> array(size);
        std::size_t to_reserve = (std::size_t)GENERATE(0, 10, 20, 300, 4000, 50000);
        std::size_t old_capacity = array.capacity();

        array.reserve(to_reserve);

        CHECK(array.size() == size); // size must be unchanged
        // when to_reserve is less than old_capacity, capacity is unchanged
        if (to_reserve < old_capacity) {
            CHECK(array.capacity() == old_capacity);
        } else { // otherwise, should be greater than or equal to to_reserve
            CHECK(array.capacity() >= to_reserve);
        }
    }
    SECTION(".capacity()") {
        std::size_t size = (std::size_t)GENERATE(0, 10, 20, 300, 4000, 50000);
        sharray<int> array(size);

        CHECK(array.capacity() >= size);
    }
    SECTION(".shrink_to_fit()") {
        WARN("sharray.shrink_to_fit() does nothing. This is standard-compliant.");
    }
    SECTION(".clear()") {
        std::size_t size = (std::size_t)GENERATE(0, 1, 2, 3, 4, 5);
        sharray<int> array(size);
        std::size_t old_capacity = array.capacity();

        array.clear();

        CHECK(array.size() == 0);
        // as per the standard, .clear() must not change capacity (for vector)
        CHECK(array.capacity() == old_capacity);
    }
    SECTION(".push_back()") {
        sharray<int> array = {97, 31};

        SECTION("lvalue") {
            int v = 42;

            array.push_back(v);

            CHECK(array.size() == 3);
            CHECK(array == sharray<int>({97, 31, 42}));
        }
        SECTION("rvalue") {
            array.push_back(42);

            CHECK(array.size() == 3);
            CHECK(array == sharray<int>({97, 31, 42}));
        }
    }
    SECTION(".pop_back()") {
        sharray<int> array = {1, 2, 3, 4};

        array.pop_back();

        CHECK(array.size() == 3);
        CHECK(array == sharray<int>({1, 2, 3}));
    }
    SECTION(".push_front()") {
        sharray<int> array = {97, 31};

        SECTION("lvalue") {
            int v = 42;

            array.push_front(v);

            CHECK(array.size() == 3);
            CHECK(array == sharray<int>({42, 97, 31}));
        }
        SECTION("rvalue") {
            array.push_front(42);

            CHECK(array.size() == 3);
            CHECK(array == sharray<int>({42, 97, 31}));
        }
    }
    SECTION(".pop_front()") {

        sharray<int> array = {1, 2, 3, 4};

        array.pop_front();

        CHECK(array.size() == 3);
        CHECK(array == sharray<int>({2, 3, 4}));
    }
}
