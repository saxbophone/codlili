#include <deque>
#include <list>
#include <typeinfo>
#include <utility>
#include <vector>

#include <catch2/catch_all.hpp>

#include <codlili/list.hpp>


using namespace com::saxbophone;

TEMPLATE_PRODUCT_TEST_CASE(
    "Type satisfies the SequenceContainer named requirement", "",
    (std::deque, std::list, std::vector), (char, int, long, float)
) {
    // alias key definitions to the names used for them in the C++ named requirements
    // using X = TestType;
    // using T = X::value_type;
    // using A = X::allocator_type;
}
