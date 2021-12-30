#include <string>
#include <optional>
#include <cassert>
#include <maybe_ext/maybe_ext.hpp>

using std::string, std::optional;
using namespace maybe_ext;

int main() {
    struct Object {
        string method() { return "method()"; }
        string method() const { return "method() const"; }
    };

    using maybe_ext::utils::mut_fn;
    optional<Object> maybe = Object();
    optional<string> m = maybe | map(mut_fn(&Object::method));
    assert(m == optional<string>("method()"));

    return 0;
}