#include <gtest/gtest.h>
#include <iostream>

namespace case1
{
    // Absence of virtual dtor leads to memory leak
    std::vector<std::string> callStack;

    // No virtual dtor for Base
    class Base
    {
    public:
        Base()
        {
//          std::cout << "case1::Base::Base()" << std::endl;
            callStack.push_back("case1::Base::Base()");
        }
        ~Base()
        {
//          std::cout << "case1::Base::~Base()" << std::endl;
            callStack.push_back("case1::Base::~Base()");
        }
    };

    class Derived : public Base
    {
    public:
        Derived()
        {
//          std::cout << "case1::Derived::Derived()" << std::endl;
            callStack.push_back("case1::Derived::Derived()");
        }
        ~Derived()
        {
//          std::cout << "case1::Derived::~Derived()" << std::endl;
            callStack.push_back("case1::Derived::~Derived()");
        }
    };
}


TEST(CtorsDtors, case1_mem_leak)
{
    using namespace case1;

    // no virtual dtor, owning as Base ptr, memory leak!
    callStack.clear();
    Base* p = new Derived;
    delete p;

    EXPECT_EQ(3, callStack.size());
    EXPECT_EQ("case1::Base::Base()", callStack[0]);
    EXPECT_EQ("case1::Derived::Derived()", callStack[1]);
    EXPECT_EQ("case1::Base::~Base()", callStack[2]);
}


TEST(CtorsDtors, case1_no_mem_leak)
{
    using namespace case1;

    // no virtual dtor, owning as Derived ptr, no memory leak
    callStack.clear();
    Derived* p = new Derived;
    delete p;

    EXPECT_EQ(4, callStack.size());
    EXPECT_EQ("case1::Base::Base()", callStack[0]);
    EXPECT_EQ("case1::Derived::Derived()", callStack[1]);
    EXPECT_EQ("case1::Derived::~Derived()", callStack[2]);
    EXPECT_EQ("case1::Base::~Base()", callStack[3]);
}


namespace case2
{
    // Having virtual dtor for types to be sub-classed is gooood idea
    std::vector<std::string> callStack;

    // virtual dtor is presenting
    class Base
    {
    public:
        Base()
        {
//          std::cout << "case2::Base::Base()" << std::endl;
            callStack.push_back("case2::Base::Base()");
        }
        virtual ~Base()
        {
//          std::cout << "case2::Base::~Base()" << std::endl;
            callStack.push_back("case2::Base::~Base()");
        }
    };

    class Derived : public Base
    {
    public:
        Derived()
        {
//          std::cout << "case2::Derived::Derived()" << std::endl;
            callStack.push_back("case2::Derived::Derived()");
        }
        ~Derived()
        {
//          std::cout << "case2::Derived::~Derived()" << std::endl;
            callStack.push_back("case2::Derived::~Derived()");
        }
    };
}

TEST(CtorsDtors, case2_no_mem_leak)
{
    using namespace case2;

    // virtual dtor is presenting, owning as Base ptr, no memory leak
    callStack.clear();
    Base* p = new Derived;
    delete p;

    EXPECT_EQ(4, callStack.size());
    EXPECT_EQ("case2::Base::Base()", callStack[0]);
    EXPECT_EQ("case2::Derived::Derived()", callStack[1]);
    EXPECT_EQ("case2::Derived::~Derived()", callStack[2]);
    EXPECT_EQ("case2::Base::~Base()", callStack[3]);
}


TEST(CtorsDtors, case2_no_mem_leak2)
{
    using namespace case2;

    // virtual dtor is presenting, owning as Base ptr, no memory leak
    callStack.clear();
    Derived* p = new Derived;
    delete p;

    EXPECT_EQ(4, callStack.size());
    EXPECT_EQ("case2::Base::Base()", callStack[0]);
    EXPECT_EQ("case2::Derived::Derived()", callStack[1]);
    EXPECT_EQ("case2::Derived::~Derived()", callStack[2]);
    EXPECT_EQ("case2::Base::~Base()", callStack[3]);
}




namespace case3
{
    // Having virtual dtor for types to be sub-classed is gooood idea
    std::vector<std::string> callStack;

    struct C
    {
        C()
        {
//          std::cout << "case3::C::C()" << std::endl;
            callStack.push_back("case3::C::C()");
        }
        ~C()
        {
//          std::cout << "case3::C::~C()" << std::endl;
            callStack.push_back("case3::C::~C()");
        }
    };

    struct D
    {
        D()
        {
//          std::cout << "case3::D::D()" << std::endl;
            callStack.push_back("case3::D::D()");
        }
        ~D()
        {
//          std::cout << "case3::D::~D()" << std::endl;
            callStack.push_back("case3::D::~D()");
        }
    };

    // virtual dtor is presenting
    struct A
    {
        A()
        {
//          std::cout << "case3::A::A()" << std::endl;
            callStack.push_back("case3::A::A()");
        }
        virtual ~A()
        {
//          std::cout << "case3::A::~A()" << std::endl;
            callStack.push_back("case3::A::~A()");
        }
    };

    struct B : public A
    {
        B()
        {
//          std::cout << "case3::B::B()" << std::endl;
            callStack.push_back("case3::B::B()");
        }
        ~B()
        {
//          std::cout << "case3::B::~B()" << std::endl;
            callStack.push_back("case3::B::~B()");
        }

        C _c;
        D _d;
    };

};

TEST(CtorsDtors, case3)
{
    using namespace case3;

    {
        auto ptr = std::make_shared<B>();
    }
    EXPECT_EQ(8, callStack.size());
    EXPECT_EQ("case3::A::A()", callStack[0]);
    EXPECT_EQ("case3::C::C()", callStack[1]);
    EXPECT_EQ("case3::D::D()", callStack[2]);
    EXPECT_EQ("case3::B::B()", callStack[3]);

    EXPECT_EQ("case3::B::~B()", callStack[4]);
    EXPECT_EQ("case3::D::~D()", callStack[5]);
    EXPECT_EQ("case3::C::~C()", callStack[6]);
    EXPECT_EQ("case3::A::~A()", callStack[7]);
}

