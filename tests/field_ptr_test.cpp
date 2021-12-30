#include "prelude.hpp"
#include "maybe_ext/and_then.hpp"
#include "maybe_ext/map.hpp"
#include "maybe_ext/contains.hpp"

using namespace maybe_ext;

TEST(field_ptr, field_ptr_map_opt) {
    struct Object {
        int a;
    };
    optional<Object> x = Object{233};
    int *maybe_a = x | map(&Object::a);
    ASSERT_TRUE(maybe_a | contains(233));

    x = nullopt;
    maybe_a = x | map(&Object::a);
    ASSERT_EQ(maybe_a, nullptr);
}

TEST(field_ptr, field_ptr_and_then_opt) {
    struct Object {
        optional<int> a;
    };
    optional<Object> x = Object{233};
    int *maybe_a = x | and_then(&Object::a);
    ASSERT_TRUE(maybe_a | contains(233));

    x->a = nullopt;
    maybe_a = x | and_then(&Object::a);
    ASSERT_EQ(maybe_a, nullptr);

    x = nullopt;
    maybe_a = x | and_then(&Object::a);
    ASSERT_EQ(maybe_a, nullptr);
}

TEST(field_ptr, field_ptr_map_uptr) {
    struct Object {
        int a;
    };
    auto x = make_unique<Object>(Object{233});
    int *maybe_a = x | map(&Object::a);
    ASSERT_TRUE(maybe_a | contains(233));

    x = nullptr;
    maybe_a = x | map(&Object::a);
    ASSERT_EQ(maybe_a, nullptr);
}

TEST(field_ptr, field_ptr_and_then_uptr) {
    struct Object {
        unique_ptr<int> a;
    };
    auto x = make_unique<Object>(Object{
            make_unique<int>(233)}
    );
    int *maybe_a = x | and_then(&Object::a);
    ASSERT_TRUE(maybe_a | contains(233));

    x->a = nullptr;
    maybe_a = x | and_then(&Object::a);
    ASSERT_EQ(maybe_a, nullptr);

    x = nullptr;
    maybe_a = x | and_then(&Object::a);
    ASSERT_EQ(maybe_a, nullptr);
}
