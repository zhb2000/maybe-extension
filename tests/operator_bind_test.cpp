#include "prelude.hpp"
#include "maybe_ext/operator_bind.hpp"
#include "maybe_ext/contains.hpp"
#include "maybe_ext/cloned.hpp"

using namespace maybe_ext;
using utils::operator>>;

TEST(operator_bind, rs_example_opt) {
    auto sq = [](int x) -> optional<int> { return {x * x}; };
    auto nope = [](int) -> optional<int> { return {}; };

    ASSERT_EQ(optional(2) >> sq >> sq, optional(16));
    ASSERT_EQ(optional(2) >> sq >> nope, nullopt);
    ASSERT_EQ(optional(2) >> nope >> sq, nullopt);
    ASSERT_EQ(optional<int>() >> sq >> sq, nullopt);
}

TEST(operator_bind, rs_example_ptr) {
    auto sq = [](int x) -> optional<int> { return {x * x}; };
    auto nope = [](int) -> optional<int> { return {}; };

    int number_two = 2;
    int *two = &number_two;
    int *null = nullptr;
    ASSERT_EQ(two >> sq >> sq, optional(16));
    ASSERT_EQ(two >> sq >> nope, nullopt);
    ASSERT_EQ(two >> nope >> sq, nullopt);
    ASSERT_EQ(null >> sq >> sq, nullopt);
}

TEST(operator_bind, rs_example_uptr) {
    auto sq = [](int x) -> optional<int> { return {x * x}; };
    auto nope = [](int) -> optional<int> { return {}; };

    auto two = make_unique<int>(2);
    ASSERT_EQ(two >> sq >> sq, optional(16));
    ASSERT_EQ(two >> sq >> nope, nullopt);
    ASSERT_EQ(two >> nope >> sq, nullopt);
    ASSERT_EQ(unique_ptr<int>() >> sq >> sq, nullopt);
}

TEST(operator_bind, and_then_with_field) {
    struct User {
        optional<int> age;
    };
    optional<User> user = User{.age = 233};
    int *age = user >> &User::age;
    ASSERT_TRUE(age | contains(233));

    user->age = nullopt;
    ASSERT_EQ(user >> &User::age, nullptr);

    user = nullopt;
    ASSERT_EQ(user >> &User::age, nullptr);
}

TEST(operator_bind, and_then_with_field_cloned) {
    struct User {
        optional<int> age;
    };
    auto make_user = [] {
        return optional(User{.age=233});
    };
    optional<int> age = (make_user() >> &User::age)
                        | cloned();
    ASSERT_EQ(age, optional(233));
}
