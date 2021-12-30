#include "prelude.hpp"
#include "maybe_ext/maybe_ext.hpp"

using namespace maybe_ext;

TEST(MaybeTest, student_opt) {
    struct Student {
        optional<string> name;
        optional<int> age;
    };
    optional<Student> student = Student{"12345", 16};
    optional<string::size_type> len = student
                                      | and_then(&Student::name)
                                      | map(&string::length);
    ASSERT_EQ(len, optional(5));

    int age = student
              | and_then(&Student::age)
              | value_or(233);
    ASSERT_EQ(age, 16);

    student = nullopt;
    len = student
          | and_then(&Student::name)
          | map(&string::length);
    ASSERT_EQ(len, nullopt);

    age = student
          | and_then(&Student::age)
          | value_or(233);
    ASSERT_EQ(age, 233);
}

TEST(MaybeTest, student_uptr) {
    struct Student {
        unique_ptr<string> name;
        unique_ptr<int> age;
    };
    unique_ptr<Student> student = make_unique<Student>(Student{
            make_unique<string>("12345"),
            make_unique<int>(16)
    });
    optional<string::size_type> len = student
                                      | and_then(&Student::name)
                                      | map(&string::length);
    ASSERT_EQ(len, optional(5));

    int age = student
              | and_then(&Student::age)
              | value_or(233);
    ASSERT_EQ(age, 16);

    student = nullptr;
    len = student
          | and_then(&Student::name)
          | map(&string::length);
    ASSERT_EQ(len, nullopt);

    age = student
          | and_then(&Student::age)
          | value_or(233);
    ASSERT_EQ(age, 233);
}
