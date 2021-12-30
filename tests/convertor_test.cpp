#include "prelude.hpp"
#include "maybe_ext/and_then.hpp"
#include "maybe_ext/map.hpp"
#include "maybe_ext/utils.hpp"

using namespace maybe_ext;
using utils::Convertor;

TEST(convertor, convertor_test) {
    struct User {
        optional<string> name;
    };
    optional<User> maybe_user = User{"Alice"};
    optional<int> maybe_len = maybe_user
                              | and_then(&User::name)
                              | map(&string::length)
                              | map(Convertor<int>());
    ASSERT_EQ(maybe_len, optional(5));
}
