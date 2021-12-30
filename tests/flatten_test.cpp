#include "prelude.hpp"
#include "maybe_ext/flatten.hpp"
#include "maybe_ext/contains.hpp"

using namespace maybe_ext;

TEST(flatten, rs_example_opt_opt) {
    optional<optional<int>> x = optional(optional(6));
    optional<int> y = flatten(x);
    ASSERT_EQ(y, optional(6));

    x = optional(optional<int>());
    ASSERT_TRUE(x);
    ASSERT_FALSE(*x);
    y = flatten(x);
    ASSERT_EQ(y, nullopt);

    x = nullopt;
    ASSERT_FALSE(x);
    y = flatten(x);
    ASSERT_EQ(y, nullopt);
}

TEST(flatten, rs_example_ptr_ptr) {
    int number = 6;
    int *p1 = &number;
    int **p2 = &p1;
    int *y = flatten(p2);
    ASSERT_TRUE(y | contains(6));

    p1 = nullptr;
    ASSERT_TRUE(p2);
    ASSERT_FALSE(*p2);
    y = flatten(p2);
    ASSERT_EQ(y, nullptr);

    p2 = nullptr;
    ASSERT_FALSE(p2);
    y = flatten(p2);
    ASSERT_EQ(y, nullptr);
}

TEST(flatten, ptr_ptr2) {
    const char *p1 = "lalala";
    const char **p2 = &p1;
    const char *y = flatten(p2);
    ASSERT_TRUE(y);
    ASSERT_EQ(strcmp(y, p1), 0);

    p1 = nullptr;
    y = flatten(p2);
    ASSERT_EQ(y, nullptr);

    p2 = nullptr;
    y = flatten(p2);
    ASSERT_EQ(y, nullptr);
}

TEST(flatten, rs_example_opt_uptr) {
    optional<unique_ptr<int>> x = {make_unique<int>(6)};
    unique_ptr<int> y = flatten(std::move(x));
    ASSERT_TRUE(y | contains(6));

    x = {nullptr};
    ASSERT_TRUE(x);
    ASSERT_FALSE(*x);
    y = flatten(std::move(x));
    ASSERT_EQ(y, nullptr);

    x = nullopt;
    ASSERT_FALSE(x);
    y = flatten(std::move(x));
    ASSERT_EQ(y, nullptr);
}

TEST(flatten, rs_example_uptr_opt) {
    unique_ptr<optional<int>> x = make_unique<optional<int>>(6);
    optional<int> y = flatten(x);
    ASSERT_EQ(y, optional(6));

    x = make_unique<optional<int>>(nullopt);
    ASSERT_TRUE(x);
    ASSERT_FALSE(*x);
    y = flatten(x);
    ASSERT_EQ(y, nullopt);

    x = nullptr;
    ASSERT_FALSE(x);
    y = flatten(x);
    ASSERT_EQ(y, nullopt);
}
