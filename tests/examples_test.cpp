#include "prelude.hpp"
#include "maybe_ext/maybe_ext.hpp"

using namespace maybe_ext;

TEST(examples, convertor) {
    using utils::Convertor;
    optional<string> maybe_string = "whu";
    optional<int> maybe_len = maybe_string
                              | map(&string::length)
                              | map(Convertor<int>()); // string::size_type -> int
    ASSERT_EQ(maybe_len, optional(3));
}

TEST(examples, fallible_operations) {
    struct Food { string data; };
    struct Peeled { string data; };
    struct Chopped { string data; };
    struct Cooked { string data; };
    auto peel = [](Food food) -> optional<Peeled> {
        return Peeled{std::move(food.data)};
    };
    auto chop = [](Peeled peeled) -> optional<Chopped> {
        return Chopped{std::move(peeled.data)};
    };
    auto cook = [](Chopped chopped) -> optional<Cooked> {
        return Cooked{std::move(chopped.data)};
    };

    auto process = [=](optional<Food> food) -> optional<Cooked> {
        return std::move(food)
               | and_then(peel)
               | and_then(chop)
               | and_then(cook);
    };

    ASSERT_NE(process(Food{"abc"}), nullopt);
    ASSERT_EQ(process(nullopt), nullopt);
}

TEST(examples, member_pointer_sugar1) {
    struct User {
        string name;
        optional<int> age;
    };

    optional<User> maybe_user = User{.name = "Bob", .age = 20};

    string *maybe_name = maybe_user | map(&User::name);
    int *maybe_age = maybe_user | and_then(&User::age);
    optional<size_t> maybe_len = maybe_name | map(&string::length);

    ASSERT_TRUE(maybe_name | contains("Bob"));
    ASSERT_TRUE(maybe_age | contains(20));
    ASSERT_EQ(maybe_len, optional(3));
}

TEST(examples, member_pointer_sugar2) {
    struct User {
        string name;
        optional<int> age;
    };
    auto make_user = []() -> optional<User> {
        return User{.name = "Bob", .age = 20};
    };

    optional<string> maybe_name = make_user()
                                  | map(&User::name)
                                  | cloned();
    ASSERT_EQ(maybe_name, optional<string>("Bob"));
}

TEST(examples, method_overload) {
    using maybe_ext::utils::mut_fn;
    struct Object {
        string method() { return "method()"; }
        string method() const { return "method() const"; }
    };

    optional<Object> maybe = Object();
    optional<string> m = maybe | map(mut_fn(&Object::method));
    ASSERT_EQ(m, optional<string>("method()"));
}

TEST(examples, operator_bind) {
    auto sq = [](int x) -> optional<int> { return x * x; };
    auto nope = [](int) -> optional<int> { return nullopt; };

    using maybe_ext::utils::operator>>;
    ASSERT_EQ(optional(2) >> sq >> sq, optional(16));
    ASSERT_EQ(optional(2) >> sq >> nope, nullopt);
    ASSERT_EQ(optional(2) >> nope >> sq, nullopt);
    ASSERT_EQ(optional<int>() >> sq >> sq, nullopt);
}

TEST(examples, optional_chaining) {
    struct User {
        optional<string> name;
    };
    struct Post {
        int id;
        optional<User> author;
    };
    auto get_post_by_id = [](int id) -> optional<Post> {
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
    };
    auto author_name_len = [get_post_by_id](int post_id) {
        return get_post_by_id(post_id)
               | and_then(&Post::author)
               | and_then(&User::name)
               | map(&string::length);
    };

    ASSERT_EQ(author_name_len(0), optional(3));
    ASSERT_EQ(author_name_len(1), nullopt);
    ASSERT_EQ(author_name_len(2), nullopt);
    ASSERT_EQ(author_name_len(3), nullopt);
}

TEST(examples, pointers) {
    struct Node {
        Node *lch = nullptr;
        Node *rch = nullptr;
        int height() const {
            int lh = lch | map_or(0, &Node::height);
            int rh = rch | map_or(0, &Node::height);
            return 1 + std::max(lh, rh);
        }
    };

    /*    a
         / \
        b   c
       /
      d       */
    Node *a = new Node(), *b = new Node(), *c = new Node(), *d = new Node();
    a->lch = b;
    a->rch = c;
    b->lch = d;
    assert(a->height() == 3);
    assert(b->height() == 2);
    assert(c->height() == 1);
    assert(d->height() == 1);
    delete a;
    delete b;
    delete c;
    delete d;
}
