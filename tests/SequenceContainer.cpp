#include <array>
#include <deque>
#include <list>
#include <typeinfo>
#include <utility>
#include <vector>

#include <catch2/catch_all.hpp>

#include <codlili/sharray.hpp>


using namespace com::saxbophone;

TEMPLATE_PRODUCT_TEST_CASE(
    "Type satisfies the SequenceContainer named requirement", "",
    (std::deque, std::list, std::vector, codlili::sharray), (char, int, long, float)
) {
    // alias key definitions to the names used for them in the C++ named requirements
    using X = TestType;
    using T = X::value_type;
    // using A = X::allocator_type;
    using iterator = X::iterator;
    using const_iterator = X::const_iterator;
    X a = X({1, 2, 3, 4, 5, 6, 7});
    const_iterator p = a.end(); // one past-the-end, valid but not dereferenceable
    const_iterator q = a.begin()++++++; // middle of the container, dereferenceable
    const_iterator q1 = a.begin(), q2 = a.end(); // two const iterators into a such that [q1, q2) is a valid range
    T other[] = {3, 4, 1, 2, 3, 6, 8, 9, 2, 4};
    auto i = other, j = other + 4; // LegacyInputIterators such that [i, j) is a valid range and that the iterators
                                   // refer to elements implicitly convertible to value_type
    std::initializer_list<T> il = {3, 6, 9, 12, 14, 16, 18, 23}; // an object of type std::initializer_list<value_type>
    typename X::size_type n = 3; // a value of type X::size_type
    T t = 13; // an lvalue or const rvalue of type X::value_type

    SECTION("X(n, t) Constructs the sequence container holding n copies of t") {
        CHECK(X(n, t) == X({13, 13, 13}));
    }
    SECTION("X(i, j) Constructs the sequence container equal, element-wise, to the range [i,j)") {
        CHECK(X(i, j) == X({3, 4, 1, 2}));
    }
    SECTION("X(il) initializer_list constructor") {
        CHECK(X(il) == X({3, 6, 9, 12, 14, 16, 18, 23}));
    }
    SECTION("a = il Assigns the range represented by il into a") {
        CHECK(typeid(a = il) == typeid(X&));
        a = il;

        CHECK(a == X({3, 6, 9, 12, 14, 16, 18, 23}));
    }
    SECTION("a.emplace(p, args) Insert an object of type T, constructed with std::forward<Args>(args) before p") {
        CHECK(typeid(a.emplace(p, 33)) == typeid(iterator));
    }
    SECTION("a.insert(p, t) Inserts a copy of t before p") {
        CHECK(typeid(a.insert(p, t)) == typeid(iterator));
    }
}
