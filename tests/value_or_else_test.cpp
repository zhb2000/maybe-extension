#include "prelude.hpp"
#include "maybe_ext/value_or_else.hpp"

using namespace maybe_ext;

TEST(value_or_else, rs_example_opt) {
    int k = 10;
    ASSERT_EQ(optional(4) | value_or_else([k] { return 2 * k; }), 4);
    ASSERT_EQ(optional<int>() | value_or_else([k] { return 2 * k; }), 20);
}

TEST(value_or_else, rs_example_uptr) {
    int k = 10;
    ASSERT_EQ(make_unique<int>(4) | value_or_else([k] { return 2 * k; }), 4);
    ASSERT_EQ(unique_ptr<int>() | value_or_else([k] { return 2 * k; }), 20);
}
