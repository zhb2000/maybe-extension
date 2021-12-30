#include <string>
#include <optional>
#include <cassert>
#include <maybe_ext/maybe_ext.hpp>

using std::string, std::optional;
using namespace maybe_ext;

struct User {
    string name;
    optional<int> age;
};

void example1() {
    optional<User> maybe_user = User{.name = "Bob", .age = 20};

    string *maybe_name = maybe_user | map(&User::name);
    int *maybe_age = maybe_user | and_then(&User::age);
    optional<size_t> maybe_len = maybe_name | map(&string::length);

    assert(maybe_name && *maybe_name == "Bob");
    assert(maybe_age && *maybe_age == 20);
    assert(maybe_len == optional(3));
}

optional<User> make_user() {
    return User{.name = "Bob", .age = 20};
};

void example2() {
    optional<string> maybe_name = make_user()
                                  | map(&User::name)
                                  | cloned();
    assert(maybe_name == optional<string>("Bob"));
}

int main() {
    example1();
    example2();
    return 0;
}