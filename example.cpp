#include <iostream>
#include <optional>
#include "maybe_ext/maybe_ext.hpp"

using std::string, std::cout, std::endl;
using std::optional, std::nullopt;
using namespace maybe_ext;

int main() {
    optional<string> u = "abc";
    cout << (u == optional("abc")) << endl;
    return 0;
}
