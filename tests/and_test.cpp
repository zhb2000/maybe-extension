#include "prelude.hpp"
#include "maybe_ext/and.hpp"
#include "maybe_ext/contains.hpp"

using namespace maybe_ext;

TEST(and_, rs_example_opt) {
    optional<int> x = 2;
    optional<string> y = nullopt;
    ASSERT_EQ(x | and_(y), nullopt);

    x = nullopt;
    y = "foo";
    ASSERT_EQ(x | and_(y), nullopt);

    x = 2;
    y = "foo";
    ASSERT_EQ(x | and_(y), optional("foo"));

    x = nullopt;
    y = nullopt;
    ASSERT_EQ(x | and_(y), nullopt);
}

TEST(and_, rs_example_uptr) {
    unique_ptr<int> x = make_unique<int>(2);
    unique_ptr<string> y = nullptr;
    ASSERT_EQ(x | and_(std::move(y)), nullptr);

    x = nullptr;
    y = make_unique<string>("foo");
    ASSERT_EQ(x | and_(std::move(y)), nullptr);

    x = make_unique<int>(2);
    y = make_unique<string>("foo");
    ASSERT_TRUE(x | and_(std::move(y)) | contains("foo"));

    x = nullptr;
    y = nullptr;
    ASSERT_EQ(x | and_(std::move(y)), nullptr);
}
