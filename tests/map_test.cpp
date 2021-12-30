#include "prelude.hpp"
#include "maybe_ext/map.hpp"
#include "maybe_ext/contains.hpp"
#include "maybe_ext/cloned.hpp"

using namespace maybe_ext;

TEST(map, rs_example_opt) {
    optional<string> maybe_string = "Hello, World!";
    auto maybe_len = maybe_string | map(&string::length);

    ASSERT_EQ(maybe_len, optional(13));
}

TEST(map, rs_example_uptr) {
    auto maybe_string = make_unique<string>("Hello, World!");
    auto maybe_len = maybe_string | map(&string::length);

    ASSERT_EQ(maybe_len, optional(13));
}

TEST(map, map_with_field) {
    struct User {
        int age;
    };
    optional<User> user = User{.age = 233};
    int *age = user | map(&User::age);
    ASSERT_TRUE(age | contains(233));

    user = nullopt;
    age = user | map(&User::age);
    ASSERT_EQ(age, nullptr);
}

TEST(map, map_with_field_cloned) {
    struct User {
        int age;
    };
    auto make_user = [] {
        return optional(User{.age=233});
    };
    optional<int> age = make_user()
                        | map(&User::age)
                        | cloned();
    ASSERT_EQ(age, optional(233));
}
