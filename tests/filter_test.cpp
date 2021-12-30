#include "prelude.hpp"
#include "maybe_ext/filter.hpp"
#include "maybe_ext/contains.hpp"

using namespace maybe_ext;

TEST(filter, rs_example_opt) {
    auto is_even = [](int n) { return n % 2 == 0; };

    ASSERT_EQ(optional<int>() | filter(is_even), nullopt);
    ASSERT_EQ(optional(3) | filter(is_even), nullopt);
    ASSERT_EQ(optional(4) | filter(is_even), optional(4));
}

TEST(filter, rs_example_uptr) {
    auto is_even = [](int n) { return n % 2 == 0; };

    ASSERT_EQ(unique_ptr<int>() | filter(is_even), nullptr);
    ASSERT_EQ(make_unique<int>(3) | filter(is_even), nullptr);
    ASSERT_TRUE(make_unique<int>(4) | filter(is_even) | contains(4));
}
