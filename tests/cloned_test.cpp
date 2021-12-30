#include "prelude.hpp"
#include "maybe_ext/cloned.hpp"

using namespace maybe_ext;

TEST(cloned, cloned_ptr) {
    int number = 123;

    int *p1 = &number;
    optional<int> x1 = p1 | cloned();
    ASSERT_EQ(x1, optional(123));

    const int *p2 = &number;
    optional<int> x2 = p2 | cloned();
    ASSERT_EQ(x2, optional(123));

    int *p3 = nullptr;
    optional<int> x3 = p3 | cloned();
    ASSERT_EQ(x3, nullopt);
}

TEST(cloned, cloned_uptr) {
    auto p1 = make_unique<int>(123);
    optional<int> x1 = p1 | cloned();
    ASSERT_EQ(x1, optional(123));

    unique_ptr<int> p2 = nullptr;
    optional<int> x2 = p2 | cloned();
    ASSERT_EQ(x2, nullopt);
}
