#include <list>
#include <typeinfo>
#include <utility>

#include <catch2/catch_all.hpp>

#include <codlili/list.hpp>


using namespace com::saxbophone;

TEMPLATE_PRODUCT_TEST_CASE(
    "list types satisfy the Container named requirement", "",
    (std::list/*, codlili::list*/), (char, int, long, float)
) {
    // alias key definitions to the names used for them in the C++ named requirements
    using C = TestType;
    // using T = C::value_type;
    // using reference = C::reference;
    // using const_reference = C::const_reference;
    using iterator = C::iterator;
    using const_iterator = C::const_iterator;
    // using difference_type = C::difference_type;
    // using size_type = C::size_type;

    C a = {1, 2, 3}, b = {4, 5, 6}; // a, b --objects of type C
    auto rv = std::move(C({7, 8, 9})); // rv --a prvalue expression of type C --XXX: It's actually an xvalue here...
    C old_rvalue = { 7, 8, 9 };

    SECTION("C() creates an empty container") {
        CHECK(C().empty());
    }
    SECTION("C(a) creates a copy of a") {
        CHECK(a == C(a));
    }
    SECTION("C(rv) moves rv") {
        CHECK(C(rv) == old_rvalue);
    }
    SECTION("a = b destroys or copy-assigns all elements of a from elements of b") {
        CHECK(typeid(a = b) == typeid(C&)); // typeid has no side-effects
        a = b;

        CHECK(a == b);
    }
    SECTION("a = rv destroys or move-assigns all elements of a from elements of rv") {
        CHECK(typeid(a = rv) == typeid(C&));
        a = rv;

        CHECK(a == old_rvalue);
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
}
