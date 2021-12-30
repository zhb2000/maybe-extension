<h1 align="center">Maybe Extension for C++</h1>

<p align="center"><b>English</b> | <a href="./README-zh.md">中文</a></p>

# Introduction
C++ programmers usually use `std::optional<T>` as nullable values and `T *` as nullable references. However, the standard library doesn't provide a uniform interface to operate them. This library provides some extension methods for these "Maybe types". APIs are similar to Rust `Option`.

Since C++ has no language support for extension methods yet, these "extension methods" are implemented by overloading operator `|`. The code `maybe | and_then(...) | map(...)` means `maybe.and_then(...).map(...)`.

# How to use
Language version: C++20

This is a single header library, you only need to include `single_include/maybe_ext/maybe_ext.hpp`.

Import all extension methods:

```cpp
using namespace maybe_ext;
```

Only import a part of them:

```cpp
using maybe_ext::operator|,
      maybe_ext::and_then,
      maybe_ext::map;
```

# API list
Monad and functor operations:

- `and_then` (monadic bind, a.k.a. flatmap)
- `map` (functor map, known as `transform` in C++23)
- `or_else`
- `flatten`
- `filter`

Extract or observe the contained value:

- `deref` (use operator `*`, similar to `unwrap_unchecked` in Rust)
- `value_or`
- `value_or_else`
- `contains`
- `map_or`
- `map_or_else`

Ownership:

- `as_unowned` (`as_ref` and `as_mut` in Rust)
- `cloned`

Boolean operations:

- `and_`
- `or_`
- `xor_`

Utilities (in namespace `maybe_ext::utils`):

- `operator>>`
- `Convertor`
- `mut_fn`
- `const_fn`
- `rref_fn`
- `const_rref_fn`

# Examples
You can find these examples in `examples` directory.

## Combine fallible operations
Monadic operations are quite useful when you want to combine a series of fallible operations.

```cpp
// A series of fallible operations
// Returns nullopt if the operation fails.
optional<Peeled> peel(Food food) { ... }
optional<Chopped> chop(Peeled peeled) { ... }
optional<Cooked> cook(Chopped chopped) { ... }
```

```cpp
// Returns nullopt if food is empty or any operations fails.
optional<Cooked> process(optional<Food> food) {
    return std::move(food)
           | and_then(peel)
           | and_then(chop)
           | and_then(cook);
}
```

If-else version:

```cpp
optional<Cooked> process(optional<Food> food) {
    if (!food) {
        return nullopt;
    }
    optional<Peeled> peeled = peel(std::move(*food));
    if (!peeled) {
        return nullopt;
    }
    optional<Chopped> chopped = chop(std::move(*peeled));
    if (!chopped) {
        return nullopt;
    }
    return cook(std::move(*chopped));
}
```

## Optional chaining
In C# and Javascript, programmers use operator `?.` to get properties from nullable objects. Without such syntax sugar in C++, we can do the same thing with `and_then` and `map` method.

```cpp
struct User {
    optional<string> name;
};

struct Post {
    int id;
    optional<User> author;
};

// Returns nullopt if post not found.
optional<Post> get_post_by_id(int id) { ... }
```

Get the length of author's name from a post, using `and_then` and `map`:

```cpp
// Returns nullopt if not found.
optional<size_t> author_name_len(int post_id) {
    return get_post_by_id(post_id)
           | and_then(&Post::author)
           | and_then(&User::name)
           | map(&string::length);
}
```

If-else version:

```cpp
optional<size_t> author_name_len(int post_id) {
    optional<Post> post = get_post_by_id(post_id);
    if (post && post->author && post->author->name) {
        return post->author->name->length();
    }
    return nullopt;
}
```

## Example of pointers
Besides `std::optional`, pointers can also use these extension methods. The following example uses `map_or` to calculate the height of binary trees.

```cpp
struct Node {
    Node *lch = nullptr;
    Node *rch = nullptr;
    int height() const {
        int lh = lch | map_or(0, &Node::height);
        int rh = rch | map_or(0, &Node::height);
        return 1 + std::max(lh, rh);
    }
};
```

# Member pointer as syntax sugar
Methods like `and_then` and `map` accept not only traditional callable objects but also member pointers. This aims to simplify the code.

```cpp
struct User {
    string name;
    optional<int> age;
};

optional<User> maybe_user = User{.name = "Bob", .age = 20};

// use member object pointer &User::name
string *maybe_name = maybe_user | map(&User::name);
// use member object pointer &User::age
int *maybe_age = maybe_user | and_then(&User::age);
// use member function pointer &string::length
optional<size_t> maybe_len = maybe_name | map(&string::length);

assert(maybe_name && *maybe_name == "Bob");
assert(maybe_age && *maybe_age == 20);
assert(maybe_len == optional(3));
```

Equivalent lambda version. As you see, lambdas in C++ are very, very long.

```cpp
string *maybe_name = maybe_user | map([](User &user) -> string& { return user.name; })
int *maybe_age = maybe_user | and_then([](User &user) { return as_unowned(user.age); })
optional<size_t> maybe_len = maybe_name | map([](string &s) { return s.length(); });
```

When using member object pointers with `and_then` and `map`, please be aware of the lifetime of temporary objects to avoid dangling pointers.

The following code causes undefined behavior. `maybe_name` becomes a dangling pointer, because the return value of `make_user()` is a temporary object and it will be destroyed after this statement.

```cpp
optional<User> make_user() {
    return User{.name = "Bob", .age = 20};
}

string *maybe_name = make_user() | map(&User::name); // Wrong!
```

Adding a `cloned()` to the end will fix this. Please notice that `cloned()` produces an `optional<string>` (nullable value) instead of a `string *` (nullable reference). This indicates that the variable `maybe_name` owns the string.

```cpp
optional<string> maybe_name = make_user()
                              | map(&User::name)
                              | cloned(); // OK
```

# Utilities
## `>>` operator
`>>` operator is same as `and_then`.

```cpp
using maybe_ext::utils::operator>>;

optional<int> sq(int x) { return x * x; }
optional<int> nope(int) { return nullopt; }

assert_eq(optional(2) >> sq >> sq, optional(16));
assert_eq(optional(2) >> sq >> nope, nullopt);
assert_eq(optional(2) >> nope >> sq, nullopt);
assert_eq(optional<int>() >> sq >> sq, nullopt);
```

P.S. `>>=` can be overloaded in C++, but unfortunately it's a right associative operator and the code will looks like `(optional(2) >>= sq) >>= nope`. To avoid these parentheses, the library overloads `>>` instead.

## Convertor
The following example uses a `Convertor<int>` to convert the length of string into an `int`.

```cpp
using maybe_ext::utils::Convertor;
optional<string> maybe_string = "whu";
optional<int> maybe_len = maybe_string
                          | map(&string::length)
                          | map(Convertor<int>()); // string::size_type -> int
assert(maybe_len == optional(3));
```

## Method overload selector
When using member function pointers, compilation errors may occur because of multiple overloaded functions.

```cpp
struct Object {
    string method() { return "method()"; }
    string method() const { return "method() const"; }
};

optional<Object> maybe = Object();
optional<string> m = maybe | map(&Object::method);
// Raise compilation error, because compiler can't decide which `method` to choose.
```

Helper function `mut_fn` selects the non-const version of `method`.

```cpp
using maybe_ext::utils::mut_fn;
optional<string> m = maybe | map(mut_fn(&Object::method));
assert(m == optional<string>("method()"));
```

Using a lambda in this situation is also a solution.

```cpp
optional<string> m = maybe | map([](Object &x) { return x.method(); });
assert(m == optional<string>("method()"));
```

# See also
`Option` type in Rust: [Module std::option](https://doc.rust-lang.org/std/option/index.html), [Option in std::option](https://doc.rust-lang.org/std/option/enum.Option.html)

C++23 provides [and_then](https://en.cppreference.com/w/cpp/utility/optional/and_then), [transform](https://en.cppreference.com/w/cpp/utility/optional/transform) and [or_else](https://en.cppreference.com/w/cpp/utility/optional/or_else) for [std::optional](https://en.cppreference.com/w/cpp/utility/optional). Here is the related proposal: [p0798R6 Monadic operations for std::optional](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2021/p0798r6.html).
