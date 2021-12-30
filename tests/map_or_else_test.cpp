#include "prelude.hpp"
#include "maybe_ext/map_or_else.hpp"

using namespace maybe_ext;

TEST(map_or_else, rs_example_opt) {
    int k = 21;

    optional<string> x = "foo";
    ASSERT_EQ(x | map_or_else([k] { return 2 * k; }, &string::length), 3);

    x = nullopt;
    ASSERT_EQ(x | map_or_else([k] { return 2 * k; }, &string::length), 42);
}

TEST(map_or_else, rs_example_uptr) {
    int k = 21;

    auto x = make_unique<string>("foo");
    ASSERT_EQ(x | map_or_else([k] { return 2 * k; }, &string::length), 3);

    x = nullptr;
    ASSERT_EQ(x | map_or_else([k] { return 2 * k; }, &string::length), 42);
}
