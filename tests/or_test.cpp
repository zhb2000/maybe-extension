#include "prelude.hpp"
#include "maybe_ext/or.hpp"
#include "maybe_ext/contains.hpp"

using namespace maybe_ext;

TEST(or_, rs_example_opt) {
    optional<int> x = 2;
    optional<int> y = nullopt;
    ASSERT_EQ(x | or_(y), optional(2));

    x = nullopt;
    y = 100;
    ASSERT_EQ(x | or_(y), optional(100));

    x = 2;
    y = 100;
    ASSERT_EQ(x | or_(y), optional(2));

    x = nullopt;
    y = nullopt;
    ASSERT_EQ(x | or_(y), nullopt);
}

TEST(or_, rs_example_uptr) {
    unique_ptr<int> x = make_unique<int>(2);
    unique_ptr<int> y = nullptr;
    ASSERT_TRUE(x | or_(y) | contains(2));

    x = nullptr;
    y = make_unique<int>(100);
    ASSERT_TRUE(x | or_(y) | contains(100));

    x = make_unique<int>(2);
    y = make_unique<int>(100);
    ASSERT_TRUE(x | or_(y) | contains(2));

    x = nullptr;
    y = nullptr;
    ASSERT_EQ(x | or_(y), nullptr);
}
