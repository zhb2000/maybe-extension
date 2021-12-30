#include "prelude.hpp"
#include "maybe_ext/value_or.hpp"
#include "maybe_ext/contains.hpp"

using namespace maybe_ext;

TEST(value_or, rs_example_opt) {
    ASSERT_EQ(optional("car"s) | value_or("bike"), "car");
    ASSERT_EQ(optional<string>() | value_or("bike"), "bike");
}

TEST(value_or, rs_example_uptr) {
    ASSERT_EQ(make_unique<string>("car") | value_or("bike"), "car");
    ASSERT_EQ(unique_ptr<string>() | value_or("bike"), "bike");
}
