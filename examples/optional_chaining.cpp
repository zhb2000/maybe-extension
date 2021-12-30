#include <string>
#include <optional>
#include <cassert>
#include <maybe_ext/maybe_ext.hpp>

using std::string, std::optional, std::nullopt;
using namespace maybe_ext;

struct User {
    optional<string> name;
};

struct Post {
    int id;
    optional<User> author;
};

// Returns nullopt if no such post.
optional<Post> get_post_by_id(int id) {
    switch (id) {
        case 0:
            return Post{.id = 0, .author = User{"Bob"}};
        case 1:
            return Post{.id = 1, .author = User{nullopt}};
        case 2:
            return Post{.id=2, .author = nullopt};
        default:
            return nullopt;
    }
}

// Returns nullopt if not found.
optional<size_t> author_name_len(int post_id) {
    return get_post_by_id(post_id)
           | and_then(&Post::author)
           | and_then(&User::name)
           | map(&string::length);
}

int main() {
    assert(author_name_len(0) == optional(3));
    assert(author_name_len(1) == nullopt);
    assert(author_name_len(2) == nullopt);
    assert(author_name_len(3) == nullopt);
    return 0;
}