/*
 * @Author: Chengsen Dong 1034029664@qq.com
 * @Date: 2023-01-26 07:44:21
 * @LastEditors: Chengsen Dong 1034029664@qq.com
 * @LastEditTime: 2023-01-26 08:04:52
 * @FilePath: /SleepPanda/src/app/gtest_demo/gtest_demo.cpp
 * @Description: 
 * Copyright (c) 2023 by Chengsen Dong 1034029664@qq.com(www.github.com/xddcore), All Rights Reserved. 
 */
#include "gtest/gtest.h"
 
// The class that needs to be tested (the implementation of subtraction is wrong)
class Foo {
public:
    Foo(int num) {
        num_ = num;
    };
    void Add(int n) {
        num_ += n;
    };
    void Minus(int n) {
        num_ = n;
    };
    int GetNum() {
        return num_;
    };
private:
    int num_;
};
 
// Define a test fixture class FooTest
class FooTest: public testing::Test {
protected:
    Foo* foo;
    // Code here will be called immediately after the constructor (right before each test)
    void SetUp() {
        foo = new Foo(1);
    }
    // Code here will be called immediately after each test (right before the destructor)
    void TearDown() {
        delete foo;
    }
};
 
TEST_F(FooTest, test_add) {
    EXPECT_EQ(foo->GetNum(), 1);
    foo->Add(1);
    EXPECT_EQ(foo->GetNum(), 2);
}
 
TEST_F(FooTest, test_minus) {
    EXPECT_EQ(foo->GetNum(), 1);
    foo->Minus(1);
    EXPECT_EQ(foo->GetNum(), 0);
}
 
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}