#include "prelude.hpp"
#include "maybe_ext/and_then.hpp"
#include "maybe_ext/map.hpp"
#include "maybe_ext/contains.hpp"

using namespace maybe_ext;

TEST(method_ptr_return_ref, return_ref_map_opt) {
    struct Container {
        int a;
        int &get() { return a; };
    };
    optional<Container> x = Container{233};
    int *maybe_a = x | map(&Container::get);
    ASSERT_TRUE(maybe_a | contains(233));

    x = nullopt;
    maybe_a = x | map(&Container::get);
    ASSERT_EQ(maybe_a, nullptr);
}

TEST(method_ptr_return_ref, return_ref_and_then_opt) {
    struct Container {
        optional<int> a;
        optional<int> &get() { return a; };
    };
    optional<Container> x = Container{233};
    int *maybe_a = x | and_then(&Container::get);
    ASSERT_TRUE(maybe_a | contains(233));

    x->a = nullopt;
    maybe_a = x | and_then(&Container::get);
    ASSERT_EQ(maybe_a, nullptr);

    x = nullopt;
    maybe_a = x | and_then(&Container::get);
    ASSERT_EQ(maybe_a, nullptr);
}

TEST(method_ptr_return_ref, return_ref_map_uptr) {
    struct Container {
        int a;
        int &get() { return a; };
    };
    auto x = make_unique<Container>(Container{233});
    int *maybe_a = x | map(&Container::get);
    ASSERT_TRUE(maybe_a | contains(233));

    x = nullptr;
    maybe_a = x | map(&Container::get);
    ASSERT_EQ(maybe_a, nullptr);
}

TEST(method_ptr_return_ref, return_ref_and_then_uptr) {
    struct Container {
        unique_ptr<int> a;
        unique_ptr<int> &get() { return a; };
    };
    auto x = make_unique<Container>(Container{
            make_unique<int>(233)
    });
    int *maybe_a = x | and_then(&Container::get);
    ASSERT_TRUE(maybe_a | contains(233));

    x->a = nullptr;
    maybe_a = x | and_then(&Container::get);
    ASSERT_EQ(maybe_a, nullptr);

    x = nullptr;
    maybe_a = x | and_then(&Container::get);
    ASSERT_EQ(maybe_a, nullptr);
}
