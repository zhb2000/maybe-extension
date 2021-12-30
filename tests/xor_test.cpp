#include "prelude.hpp"
#include "maybe_ext/xor.hpp"
#include "maybe_ext/contains.hpp"

using namespace maybe_ext;

TEST(xor_, rs_example_opt) {
    optional<int> x = 2;
    optional<int> y = nullopt;

    ASSERT_EQ(x | xor_(y), optional(2));

    x = nullopt;
    y = 2;
    ASSERT_EQ(x | xor_(y), optional(2));

    x = 2;
    y = 2;
    ASSERT_EQ(x | xor_(y), nullopt);

    x = nullopt;
    y = nullopt;
    ASSERT_EQ(x | xor_(y), nullopt);
}

TEST(xor_, rs_example_uptr) {
    unique_ptr<int> x = make_unique<int>(2);
    unique_ptr<int> y = nullptr;

    ASSERT_TRUE(std::move(x) | xor_(std::move(y)) | contains(2));

    x = nullptr;
    y = make_unique<int>(2);
    ASSERT_TRUE(std::move(x) | xor_(std::move(y)) | contains(2));

    x = make_unique<int>(2);
    y = make_unique<int>(2);
    ASSERT_EQ(std::move(x) | xor_(std::move(y)), nullptr);

    x = nullptr;
    y = nullptr;
    ASSERT_EQ(std::move(x) | xor_(std::move(y)), nullptr);
}
