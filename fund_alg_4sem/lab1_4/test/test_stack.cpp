//#include <gtest/gtest.h>
//
//#include "../include/stack.h"
//
//namespace my_container {
//
//    class StackTest : public ::testing::Test {
//    protected:
//        Stack<int> empty_stack;
//        Stack<int> int_stack{3, 2, 1};
//        Stack<std::string> str_stack{"hello", "world"};
//
//        void SetUp() override {
//            while (!int_stack.empty()) int_stack.pop();
//            int_stack.push(1);
//            int_stack.push(2);
//            int_stack.push(3);
//        }
//    };
//
//    TEST_F(StackTest, DefaultConstructor) {
//        EXPECT_TRUE(empty_stack.empty());
//        EXPECT_EQ(empty_stack.size(), 0);
//    }
//
//    TEST_F(StackTest, InitializerListConstructor) {
//        Stack<int> s{5, 4, 3};
//        EXPECT_EQ(s.size(), 3);
//        EXPECT_EQ(s.top(), 5);
//    }
//
//    TEST_F(StackTest, CopyConstructor) {
//        Stack<int> copy(int_stack);
//        EXPECT_EQ(copy.size(), 3);
//        EXPECT_EQ(copy.top(), 3);
//    }
//
//    TEST_F(StackTest, MoveConstructor) {
//        Stack<int> moved(std::move(int_stack));
//        EXPECT_EQ(moved.size(), 3);
//        EXPECT_TRUE(int_stack.empty());
//    }
//
//    TEST_F(StackTest, CopyAssignment) {
//        Stack<int> copy;
//        copy = int_stack;
//        EXPECT_EQ(copy.top(), 3);
//    }
//
//    TEST_F(StackTest, MoveAssignment) {
//        Stack<int> moved;
//        moved = std::move(int_stack);
//        EXPECT_EQ(moved.size(), 3);
//        EXPECT_TRUE(int_stack.empty());
//    }
//
//    TEST_F(StackTest, TopMethods) {
//        EXPECT_EQ(int_stack.top(), 3);
//        int_stack.top() = 10;
//        EXPECT_EQ(int_stack.top(), 10);
//    }
//
//    TEST_F(StackTest, PushPopOperations) {
//        empty_stack.push(42);
//        EXPECT_EQ(empty_stack.top(), 42);
//        EXPECT_EQ(empty_stack.size(), 1);
//
//        empty_stack.pop();
//        EXPECT_TRUE(empty_stack.empty());
//        EXPECT_THROW(empty_stack.pop(), std::out_of_range);
//    }
//
//    TEST_F(StackTest, SwapMethod) {
//        Stack<int> s1{1}, s2{2, 3};
//        s1.swap(s2);
//
//        EXPECT_EQ(s1.size(), 2);
//        EXPECT_EQ(s2.size(), 1);
//        EXPECT_EQ(s1.top(), 2);
//        EXPECT_EQ(s2.top(), 1);
//    }
//
//    TEST_F(StackTest, Comparisons) {
//        Stack<int> s1{3, 2, 1}, s2{3, 2, 1}, s3{3, 2};
//
//        EXPECT_TRUE(s1 == s2);
//        EXPECT_FALSE(s1 != s2);
//        EXPECT_TRUE(s3 < s1);
//        EXPECT_TRUE(s1 >= s3);
//        EXPECT_TRUE((s1 <=> s2) == std::strong_ordering::equal);
//    }
//
//    TEST_F(StackTest, StringStack) {
//        str_stack.push("!");
//        EXPECT_EQ(str_stack.top(), "!");
//        EXPECT_EQ(str_stack.size(), 3);
//
//        Stack<std::string> copy(str_stack);
//        EXPECT_EQ(copy.top(), "!");
//        copy.pop();
//        EXPECT_EQ(copy.top(), "hello");
//    }
//
//}  // namespace my_container
#include <gtest/gtest.h>

#include "../include/stack.h"
#include <stdexcept>

namespace my_container {

    class StackTest : public ::testing::Test {
    protected:
        Stack<int> empty_stack;
        Stack<int> num_stack{1, 2, 3};
        Stack<std::string> text_stack{"a", "b"};

        void SetUp() override {
            num_stack = Stack<int>{1, 2, 3};
        }
    };

    TEST_F(StackTest, DefaultConstructor) {
        EXPECT_TRUE(empty_stack.empty());
        EXPECT_EQ(empty_stack.size(), 0);
    }

    TEST_F(StackTest, InitializerListConstructor) {
        Stack<int> s{10, 20, 30};
        EXPECT_EQ(s.size(), 3);
        EXPECT_EQ(s.top(), 10);
    }

    TEST_F(StackTest, CopyConstructor) {
        Stack<int> copy(num_stack);
        EXPECT_EQ(copy.size(), 3);
        EXPECT_EQ(copy.top(), 1);
    }

    TEST_F(StackTest, MoveConstructor) {
        Stack<int> moved(std::move(num_stack));
        EXPECT_EQ(moved.size(), 3);
        EXPECT_TRUE(num_stack.empty());
    }

    TEST_F(StackTest, CopyAssignment) {
        Stack<int> copy;
        copy = num_stack;
        EXPECT_EQ(copy.top(), 1);
    }

    TEST_F(StackTest, MoveAssignment) {
        Stack<int> moved;
        moved = std::move(num_stack);
        EXPECT_EQ(moved.size(), 3);
        EXPECT_TRUE(num_stack.empty());
    }

    TEST_F(StackTest, TopMethods) {
        EXPECT_EQ(num_stack.top(), 1);
        num_stack.top() = 100;
        EXPECT_EQ(num_stack.top(), 100);
        EXPECT_THROW(empty_stack.top(), std::out_of_range);
    }

    TEST_F(StackTest, PushPopOperations) {
        empty_stack.push(50);
        EXPECT_EQ(empty_stack.top(), 50);
        EXPECT_EQ(empty_stack.size(), 1);

        empty_stack.pop();
        EXPECT_TRUE(empty_stack.empty());
        EXPECT_THROW(empty_stack.pop(), std::out_of_range);
    }

    TEST_F(StackTest, SwapMethod) {
        Stack<int> s1{4}, s2{5, 6};
        s1.swap(s2);
        EXPECT_EQ(s1.size(), 2);
        EXPECT_EQ(s2.size(), 1);
        EXPECT_EQ(s1.top(), 5);
        EXPECT_EQ(s2.top(), 4);
    }

    TEST_F(StackTest, Comparisons) {
        Stack<int> s1{1, 2, 3}, s2{1, 2, 3}, s3{1, 2};
        EXPECT_TRUE(s1 == s2);
        EXPECT_FALSE(s1 != s2);
        EXPECT_TRUE(s3 < s1);
        EXPECT_TRUE(s1 >= s3);
        EXPECT_EQ(s1 <=> s2, std::strong_ordering::equal);
    }

    TEST_F(StackTest, StringStack) {
        text_stack.push("c");
        EXPECT_EQ(text_stack.top(), "c");
        EXPECT_EQ(text_stack.size(), 3);

        Stack<std::string> copy(text_stack);
        EXPECT_EQ(copy.top(), "c");
        copy.pop();
        EXPECT_EQ(copy.top(), "a");
    }

}