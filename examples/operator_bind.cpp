#include <string>
#include <optional>
#include <cassert>
#include <maybe_ext/maybe_ext.hpp>

#define assert_eq(lexpr, rexpr) assert((lexpr) == (rexpr))
using std::string, std::optional, std::nullopt;

optional<int> sq(int x) { return x * x; }
optional<int> nope(int) { return nullopt; }

int main() {
    using maybe_ext::utils::operator>>;
    assert_eq(optional(2) >> sq >> sq, optional(16));
    assert_eq(optional(2) >> sq >> nope, nullopt);
    assert_eq(optional(2) >> nope >> sq, nullopt);
    assert_eq(optional<int>() >> sq >> sq, nullopt);

    return 0;
}