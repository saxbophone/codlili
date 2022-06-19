#include <cstddef>

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
}
