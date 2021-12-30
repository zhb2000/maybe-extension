#include "prelude.hpp"
#include "maybe_ext/and_then.hpp"
#include "maybe_ext/contains.hpp"
#include "maybe_ext/cloned.hpp"

using namespace maybe_ext;

TEST(and_then, rs_example_opt) {
    auto sq = [](int x) -> optional<int> { return {x * x}; };
    auto nope = [](int) -> optional<int> { return {}; };

    ASSERT_EQ(optional(2) | and_then(sq) | and_then(sq), optional(16));
    ASSERT_EQ(optional(2) | and_then(sq) | and_then(nope), nullopt);
    ASSERT_EQ(optional(2) | and_then(nope) | and_then(sq), nullopt);
    ASSERT_EQ(optional<int>() | and_then(sq) | and_then(sq), nullopt);
}

TEST(and_then, rs_example_ptr) {
    auto sq = [](int x) -> optional<int> { return {x * x}; };
    auto nope = [](int) -> optional<int> { return {}; };

    int number_two = 2;
    int *two = &number_two;
    int *null = nullptr;
    ASSERT_EQ(two | and_then(sq) | and_then(sq), optional(16));
    ASSERT_EQ(two | and_then(sq) | and_then(nope), nullopt);
    ASSERT_EQ(two | and_then(nope) | and_then(sq), nullopt);
    ASSERT_EQ(null | and_then(sq) | and_then(sq), nullopt);
}

TEST(and_then, rs_example_uptr) {
    auto sq = [](int x) -> optional<int> { return {x * x}; };
    auto nope = [](int) -> optional<int> { return {}; };

    auto two = make_unique<int>(2);
    ASSERT_EQ(two | and_then(sq) | and_then(sq), optional(16));
    ASSERT_EQ(two | and_then(sq) | and_then(nope), nullopt);
    ASSERT_EQ(two | and_then(nope) | and_then(sq), nullopt);
    ASSERT_EQ(unique_ptr<int>() | and_then(sq) | and_then(sq), nullopt);
}

TEST(and_then, and_then_with_field) {
    struct User {
        optional<int> age;
    };
    optional<User> user = User{.age = 233};
    int *age = user | and_then(&User::age);
    ASSERT_TRUE(age | contains(233));

    user->age = nullopt;
    ASSERT_EQ(user | and_then(&User::age), nullptr);

    user = nullopt;
    ASSERT_EQ(user | and_then(&User::age), nullptr);
}

TEST(and_then, and_then_with_field_cloned) {
    struct User {
        optional<int> age;
    };
    auto make_user = [] {
        return optional(User{.age=233});
    };
    optional<int> age = make_user()
                        | and_then(&User::age)
                        | cloned();
    ASSERT_EQ(age, optional(233));
}
