<h1 style="text-align: center">Maybe Extension for C++</h1>

<p style="text-align: center"><a href="./README.md">English</a> | <b>中文</b></p>

# 简介
在 C++ 中通常用 `std::optional<T>` 表示可空的值，用 `T*` 表示可空引用。然而，标准库并没有为操作这些类型提供统一的接口。Maybe Extension 为这些 C++ 中的“可空类型”提供了一组扩展方法，API 跟 Rust 的 `Option` 非常相似。

鉴于 C++ 尚未为扩展方法提供语言支持，这里的“扩展方法”是通过重载 `|` 运算符实现的。`maybe | and_then(...) | map(...)` 就相当于 `maybe.and_then(...).map(...)`。

# 如何使用
语言版本：C++20

这是个单头文件的库，只需要 include `single_include/maybe_ext/maybe_ext.hpp` 这个头文件。

导入所有扩展方法：

```cpp
using namespace maybe_ext;
```

只导入一部分方法：

```cpp
using maybe_ext::operator|,
      maybe_ext::and_then,
      maybe_ext::map;
```

# API 列表
Monad 和 functor 操作：

- `and_then`（monadic bind，又名 flatmap）
- `map`（functor map，C++23 中称为 `transform`）
- `or_else`
- `flatten`
- `filter`

提取或观察所含的值：

- `deref`（使用 `*` 运算符，类似 Rust 的 `unwrap_unchecked`）
- `value_or`
- `value_or_else`
- `contains`
- `map_or`
- `map_or_else`

所有权：

- `as_unowned` (Rust 里的 `as_ref` 和 `as_mut`)
- `cloned`

布尔操作：

- `and_`
- `or_`
- `xor_`

其他工具（在 `maybe_ext::utils` 命名空间中）：

- `operator>>`
- `Convertor`
- `mut_fn`
- `const_fn`
- `rref_fn`
- `const_rref_fn`

# 示例
你可以在 `examples` 文件夹中找到这些示例。

## 串联可能失败的操作
如果需要串联一系列可能失败的操作，那么 monadic operation 将派上用场。

```cpp
// 一系列可能失败的操作
// 若该操作失败则返回 nullopt
optional<Peeled> peel(Food food) { ... }
optional<Chopped> chop(Peeled peeled) { ... }
optional<Cooked> cook(Chopped chopped) { ... }
```

```cpp
// 若 food 为空或任意操作失败则返回 nullopt
optional<Cooked> process(optional<Food> food) {
    return std::move(food)
           | and_then(peel)
           | and_then(chop)
           | and_then(cook);
}
```

If-else 版本：

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

## 可选链
在 C# 和 Javascript 中可以用 `?.` 运算符从可空对象中提取属性。虽然 C++ 里没有这样的语法糖，但是可以用 `and_then` 和 `map` 实现同样的操作：

```cpp
struct User {
    optional<string> name;
};

struct Post {
    int id;
    optional<User> author;
};

// 若未找到则返回 nullopt
optional<Post> get_post_by_id(int id) { ... }
```

使用 `and_then` 和 `map` 获取帖子作者的姓名长度：

```cpp
// 若未找到则返回 nullopt
optional<size_t> author_name_len(int post_id) {
    return get_post_by_id(post_id)
           | and_then(&Post::author)
           | and_then(&User::name)
           | map(&string::length);
}
```

If-else 版本：

```cpp
optional<size_t> author_name_len(int post_id) {
    optional<Post> post = get_post_by_id(post_id);
    if (post && post->author && post->author->name) {
        return post->author->name->length();
    }
    return nullopt;
}
```

## 指针类型的例子
除 `std::optional` 外，指针类型也可以使用这些扩展方法。下面这个例子使用 `map_or` 计算二叉树的高度。

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

# 作为“语法糖”的成员指针
对于像 `and_then` 和 `map` 这样的方法，不仅可以传入传统的可调用对象，还可以传入成员指针。这旨在简化代码。

```cpp
struct User {
    string name;
    optional<int> age;
};

optional<User> maybe_user = User{.name = "Bob", .age = 20};

// 使用成员对象指针 &User::name
string *maybe_name = maybe_user | map(&User::name);
// 使用成员对象指针 &User::age
int *maybe_age = maybe_user | and_then(&User::age);
// 使用成员函数指针 &string::length
optional<size_t> maybe_len = maybe_name | map(&string::length);

assert(maybe_name && *maybe_name == "Bob");
assert(maybe_age && *maybe_age == 20);
assert(maybe_len == optional(3));
```

等价的 lambda 版本，如你所见，C++ 的 lambda 表达式非常非常啰嗦：

```cpp
string *maybe_name = maybe_user | map([](User &user) -> string& { return user.name; })
int *maybe_age = maybe_user | and_then([](User &user) { return as_unowned(user.age); })
optional<size_t> maybe_len = maybe_name | map([](string &s) { return s.length(); });
```

在 `and_then` 和 `map` 中使用成员对象指针时，请注意临时对象的生存期，以避免产生悬空指针。

以下代码会造成未定义行为，`maybe_name` 成了悬空指针，因为 `make_user()` 的返回值是一个临时对象，而临时对象会在语句结束时被析构。

```cpp
optional<User> make_user() {
    return User{.name = "Bob", .age = 20};
}

string *maybe_name = make_user() | map(&User::name); // 错误！
```

在末尾加一个 `cloned()` 就能修复这个问题。请注意 `cloned()` 产出了一个 `optional<string>`（可空的值）而不是 `string *`（可空引用）。这意味着变量 `maybe_name` 现在拥有这个 string 的所有权。

```cpp
optional<string> maybe_name = make_user()
                              | map(&User::name)
                              | cloned(); // 正确
```

# 附带工具
## `>>` 运算符
`>>` 运算符和 `and_then` 是等价的。

```cpp
using maybe_ext::utils::operator>>;

optional<int> sq(int x) { return x * x; }
optional<int> nope(int) { return nullopt; }

assert_eq(optional(2) >> sq >> sq, optional(16));
assert_eq(optional(2) >> sq >> nope, nullopt);
assert_eq(optional(2) >> nope >> sq, nullopt);
assert_eq(optional<int>() >> sq >> sq, nullopt);
```

注：C++ 可以重载 `>>=` 运算符，但 `>>=` 是右结合的，这样一来代码就得写成 `(optional(2) >>= sq) >>= nope`。为了少写括号，这里重载的是 `>>` 运算符。

## Convertor
下述示例使用一个 `Convertor<int>` 把字符串长度转换为 `int` 类型。

```cpp
using maybe_ext::utils::Convertor;
optional<string> maybe_string = "whu";
optional<int> maybe_len = maybe_string
                          | map(&string::length)
                          | map(Convertor<int>()); // string::size_type -> int
assert(maybe_len == optional(3));
```

## 重载方法选择器
使用成员函数指针时，可能会因为存在多个重载函数而出现编译错误。

```cpp
struct Object {
    string method() { return "method()"; }
    string method() const { return "method() const"; }
};

optional<Object> maybe = Object();
optional<string> m = maybe | map(&Object::method);
// 编译错误，因为编译器不知道该选哪个 `method`
```

辅助函数 `mut_fn` 可以选中 `method` 的非 const 重载。

```cpp
using maybe_ext::utils::mut_fn;
optional<string> m = maybe | map(mut_fn(&Object::method));
assert(m == optional<string>("method()"));
```

使用 lambda 也不失为一种解决办法。

```cpp
optional<string> m = maybe | map([](Object &x) { return x.method(); });
assert(m == optional<string>("method()"));
```

# 参见
Rust 中的 `Option` 类型：[Module std::option](https://doc.rust-lang.org/std/option/index.html), [Option in std::option](https://doc.rust-lang.org/std/option/enum.Option.html)

C++23 为 [std::optional](https://en.cppreference.com/w/cpp/utility/optional) 增添了 [and_then](https://en.cppreference.com/w/cpp/utility/optional/and_then)、[transform](https://en.cppreference.com/w/cpp/utility/optional/transform) 和 [or_else](https://en.cppreference.com/w/cpp/utility/optional/or_else) 方法。相关提案：[p0798R6 Monadic operations for std::optional](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2021/p0798r6.html)。
