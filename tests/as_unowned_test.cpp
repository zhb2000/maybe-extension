#include "prelude.hpp"
#include "maybe_ext/as_unowned.hpp"

using namespace maybe_ext;

TEST(as_unowned, as_unowned_opt) {
    optional<int> a = 123;
    int *p = as_unowned(a);
    ASSERT_EQ(p, &(*a));
    ASSERT_EQ(*p, 123);

    *a = 100;
    ASSERT_EQ(*p, 100);
}

TEST(as_unowned, as_unowned_uptr) {
    auto a = make_unique<int>(123);
    int *p = as_unowned(a);
    ASSERT_EQ(p, &(*a));
    ASSERT_EQ(*p, 123);

    *a = 100;
    ASSERT_EQ(*p, 100);
}
