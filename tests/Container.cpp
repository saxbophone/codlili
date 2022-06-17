#include <deque>
#include <list>
#include <typeinfo>
#include <utility>
#include <vector>

#include <catch2/catch_all.hpp>

#include <codlili/list.hpp>


using namespace com::saxbophone;

TEMPLATE_PRODUCT_TEST_CASE(
    "Type satisfies the Container named requirement", "",
    (std::deque, std::list, std::vector), (char, int, long, float)
) {
    // alias key definitions to the names used for them in the C++ named requirements
    using C = TestType;
    // using T = C::value_type;
    // using reference = C::reference;
    // using const_reference = C::const_reference;
    using iterator = C::iterator;
    using const_iterator = C::const_iterator;
    // using difference_type = C::difference_type;
    using size_type = C::size_type;

    C a = {1, 2, 3}, b = {4, 5, 6}; // a, b --objects of type C

    SECTION("C() creates an empty container") {
        CHECK(C().empty());
    }
    SECTION("C(a) creates a copy of a") {
        CHECK(a == C(a));
    }
    SECTION("C(rv) moves rv") {
        CHECK(C(C({7, 8, 9})) == C({7, 8, 9}));
    }
    SECTION("a = b destroys or copy-assigns all elements of a from elements of b") {
        CHECK(typeid(a = b) == typeid(C&)); // typeid has no side-effects
        a = b;

        CHECK(a == b);
    }
    SECTION("a = rv destroys or move-assigns all elements of a from elements of rv") {
        CHECK(typeid(a = C({7, 8, 9})) == typeid(C&));
        a = C({7, 8, 9});

        CHECK(a == C({7, 8, 9}));
    }
    SECTION("a.begin() returns Iterator to the first element of a") {
        CHECK(
            (
                typeid(a.begin()) == typeid(iterator) or
                typeid(a.begin()) == typeid(const_iterator)
            )
        );
    }
    SECTION("a.end() returns Iterator to one past the last element of a") {
        CHECK(
            (
                typeid(a.end()) == typeid(iterator) or
                typeid(a.end()) == typeid(const_iterator)
            )
        );
    }
    SECTION("a.cbegin() returns const_cast<const C&>(a).begin()") {
        CHECK(a.cbegin() == const_cast<const C&>(a).begin());
    }
    SECTION("a.cend() returns const_cast<const C&>(a).end()") {
        CHECK(a.cend() == const_cast<const C&>(a).end());
    }
    SECTION("a == b") {
        CHECK_FALSE(a == b);
        CHECK(a == C({1, 2, 3}));
        CHECK(a == a);
        CHECK(b == b);
    }
    SECTION("a != b") {
        CHECK(a != b);
        CHECK_FALSE(a != C({1, 2, 3}));
        CHECK_FALSE(a != a);
        CHECK_FALSE(b != b);
    }
    SECTION("a.swap(b) exchanges the values of a and b") {
        a.swap(b);

        CHECK(a == C({4, 5, 6}));
        CHECK(b == C({1, 2, 3}));
    }
    SECTION("swap(a, b) is equivalent to a.swap(b)") {
        std::swap(a, b);

        CHECK(a == C({4, 5, 6}));
        CHECK(b == C({1, 2, 3}));
    }
    SECTION("a.size()") {
        CHECK(a.size() == (size_type)3);
    }
    SECTION("a.max_size()") {
        CHECK(typeid(a.max_size()) == typeid(size_type));
    }
    SECTION("a.empty()") {
        CHECK_FALSE(a.empty());
        CHECK_FALSE(b.empty());
        CHECK(C().empty());
    }
}
