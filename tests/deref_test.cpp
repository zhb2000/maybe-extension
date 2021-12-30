#include "prelude.hpp"
#include "maybe_ext/deref.hpp"

using namespace maybe_ext;

TEST(deref, rs_example_opt) {
    optional<string> x = "air";
    static_assert(std::is_same_v<decltype(x | deref()), string &>);
    ASSERT_EQ(x | deref(), "air");

    static_assert(std::is_same_v<decltype(move(x) | deref()), string &&>);
    ASSERT_EQ(move(x) | deref(), "air");
}

TEST(deref, rs_example_uptr) {
    unique_ptr<string> x = make_unique<string>("air");
    ASSERT_EQ(x | deref(), "air");
    static_assert(std::is_same_v<decltype(x | deref()), string &>);
}
