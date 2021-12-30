#include "prelude.hpp"
#include "maybe_ext/map_or.hpp"

using namespace maybe_ext;

TEST(map_or, rs_example_opt) {
    optional<string> x = "foo";
    ASSERT_EQ(x | map_or(42, &string::length), 3);

    x = nullopt;
    ASSERT_EQ(x | map_or(42, &string::length), 42);
}

TEST(map_or, rs_example_uptr) {
    auto x = make_unique<string>("foo");
    ASSERT_EQ(x | map_or(42, &string::length), 3);

    x = nullptr;
    ASSERT_EQ(x | map_or(42, &string::length), 42);
}
