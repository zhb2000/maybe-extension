#include <string>
#include <optional>
#include <cassert>
#include <maybe_ext/maybe_ext.hpp>

using std::string, std::optional, std::nullopt;
using namespace maybe_ext;

struct Food { string data; };
struct Peeled { string data; };
struct Chopped { string data; };
struct Cooked { string data; };

// A series of fallible operations
// Returns nullopt if the operation fails.
optional<Peeled> peel(Food food) {
    return Peeled{std::move(food.data)};
}
optional<Chopped> chop(Peeled peeled) {
    return Chopped{std::move(peeled.data)};
}
optional<Cooked> cook(Chopped chopped) {
    return Cooked{std::move(chopped.data)};
}

// Returns nullopt if food is empty or any operations fails.
optional<Cooked> process(optional<Food> food) {
    return std::move(food)
           | and_then(peel)
           | and_then(chop)
           | and_then(cook);
}

int main() {
    assert(process(Food{"abc"}) != nullopt);
    assert(process(nullopt) == nullopt);
    return 0;
}
