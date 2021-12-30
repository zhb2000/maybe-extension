#include "prelude.hpp"
#include "maybe_ext/contains.hpp"

using namespace maybe_ext;

TEST(contains, rs_example_opt) {
    optional<int> x = 2;
    ASSERT_TRUE(x | contains(2));

    x = 3;
    ASSERT_FALSE(x | contains(2));

    x = nullopt;
    ASSERT_FALSE(x | contains(2));
}

TEST(contains, rs_example_uptr) {
    unique_ptr<int> x = make_unique<int>(2);
    ASSERT_TRUE(x | contains(2));

    x = make_unique<int>(3);
    ASSERT_FALSE(x | contains(2));

    x = nullptr;
    ASSERT_FALSE(x | contains(2));
}
