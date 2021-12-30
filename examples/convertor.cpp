#include <string>
#include <optional>
#include <cassert>
#include <maybe_ext/maybe_ext.hpp>

using std::string, std::optional;
using namespace maybe_ext;

int main() {
    using maybe_ext::utils::Convertor;
    optional<string> maybe_string = "whu";
    optional<int> maybe_len = maybe_string
                              | map(&string::length)
                              | map(Convertor<int>()); // string::size_type -> int
    assert(maybe_len == optional(3));

    return 0;
}
