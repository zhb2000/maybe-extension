#include "prelude.hpp"
#include "maybe_ext/utils.hpp"
#include "maybe_ext/map.hpp"

using namespace maybe_ext;
using utils::mut_fn, utils::const_fn, utils::rref_fn, utils::const_rref_fn;

TEST(method_ptr_overload, mut_const) {
    struct Object {
        string get() { return "get()"; }
        string get() const { return "get() const"; }
    };
    optional<Object> maybe = Object();
    ASSERT_EQ(maybe | map(mut_fn(&Object::get)), "get()");
    ASSERT_EQ(maybe | map(const_fn(&Object::get)), "get() const");
}

TEST(method_ptr_overload, ref_qualify) {
    struct Object {
        string get() &{ return "get() &"; }
        string get() const &{ return "get() const &"; }
        string get() &&{ return "get() &&"; }
        string get() const &&{ return "get() const &&"; }
    };
    optional<Object> maybe = Object();
    ASSERT_EQ(maybe | map(mut_fn(&Object::get)), "get() &");
    ASSERT_EQ(maybe | map(const_fn(&Object::get)), "get() const &");

    optional<Object> m1 = Object();
    optional<Object> m2 = Object();
    ASSERT_EQ(move(m1) | map(rref_fn(&Object::get)), "get() &&");
    ASSERT_EQ(move(m2) | map(const_rref_fn(&Object::get)), "get() const &&");
}

TEST(method_ptr_overload, method_noexcept) {
    struct Object {
        string get() noexcept { return "get()"; }
        string get() const noexcept { return "get() const"; }
    };
    optional<Object> maybe = Object();
    ASSERT_EQ(maybe | map(mut_fn(&Object::get)), "get()");
    ASSERT_EQ(maybe | map(const_fn(&Object::get)), "get() const");
}

TEST(method_ptr_overload, different_param_num) {
    struct Object {
        string get() { return "get()"; }
        string get(int) { return "get(int)"; }
    };
    optional<Object> maybe = Object();
    ASSERT_EQ(maybe | map(&Object::get), "get()");
}
