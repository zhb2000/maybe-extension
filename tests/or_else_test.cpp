#include "prelude.hpp"
#include "maybe_ext/or_else.hpp"
#include "maybe_ext/contains.hpp"

using namespace maybe_ext;

TEST(or_else, rs_example_opt) {
    auto nobody = []() -> optional<string> { return nullopt; };
    auto vikings = []() -> optional<string> { return {"vikings"}; };

    ASSERT_EQ(optional("barbarians"s) | or_else(vikings), optional("barbarians"s));
    ASSERT_EQ(optional<string>() | or_else(vikings), optional("vikings"s));
    ASSERT_EQ(optional<string>() | or_else(nobody), nullopt);
}

TEST(or_else, rs_example_uptr) {
    auto nobody = []() -> unique_ptr<string> { return nullptr; };
    auto vikings = [] { return make_unique<string>("vikings"); };

    ASSERT_TRUE(make_unique<string>("barbarians") | or_else(vikings) | contains("barbarians"));
    ASSERT_TRUE(unique_ptr<string>() | or_else(vikings) | contains("vikings"s));
    ASSERT_EQ(unique_ptr<string>() | or_else(nobody), nullptr);
}
