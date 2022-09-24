// https://www.fluentcpp.com/2018/06/19/3-simple-c17-features-that-will-make-your-code-simpler/

#include <gtest/gtest.h>

#include <thread>
#include <iostream>
#include <filesystem>

TEST(Cpp17, StructuredBindings)
{
//  https://www.fluentcpp.com/2018/06/19/3-simple-c17-features-that-will-make-your-code-simpler/
//  https://en.cppreference.com/w/cpp/language/structured_binding

//  We’ll begin with Structured Bindings. These were introduced as a means to allow a single definition to define multiple variables with different types. Structured bindings apply to many situations, and we’ll see several cases where they can make code more concise and simpler.

//  {
//      std::tuple<char, int, bool> mytuple()
//      {
//          char a = 'a';
//          int i = 123;
//          bool b = true;
//          return std::make_tuple(a, i, b);
//      }
//      char a;
//      int i;
//      bool b;
//      std::tie(a, i, b) = mytuple();
//  }
//  VS
//  {
//      std::tuple<char, int, bool> mytuple()
//      {
//          char a = 'a';
//          int i = 123;
//          bool b = true;
//          return std::make_tuple(a, i, b);
//      }
//      auto [a, i, b] = mytuple();
//  }

    // Old C++11/14 way
    {
        //std::tuple<char, int, bool> mytuple()
        auto mytuple = []()
        {
            char a = 'a';
            int i = 123;
            bool b = true;
            return std::make_tuple(a, i, b);
        };
        char a;
        int i;
        bool b;
        std::tie(a, i, b) = mytuple();
        EXPECT_EQ('a', a);
        EXPECT_EQ(123, i);
        EXPECT_EQ(true, b);
    }

    // C++17 way
    {
        auto mytuple = []()
        {
        //  char a = 'h';
        //  int i = 473;
        //  bool b = false;
        //  return std::make_tuple(a, i, b);
            return std::tuple('h', 473, false);
        };
        auto [a, i, b] = mytuple();
                EXPECT_EQ('h', a);
                EXPECT_EQ(473, i);
                EXPECT_EQ(false, b);
    }
}




TEST(Cpp17, CTAD_Class_template_argument_deduction)
{
//  Class template argument deduction (CTAD) / Template Argument Deduction
//  https://en.cppreference.com/w/cpp/language/class_template_argument_deduction

//  Then we’ll see Template Argument Deduction, which allows us to remove template arguments that we’re used to typing, but that we really shouldn’t need to.

//  {
//      std::pair p(2, 4.5);     // deduces to std::pair<int, double> p(2, 4.5);
//      std::tuple t(4, 3, 2.5); // same as auto t = std::make_tuple(4, 3, 2.5);
//      std::less l;             // same as std::less<void> l;



//      auto p = std::pair<int, double>(2, 4.5);
//         Vs
//      auto p = std::pair(2, 4.5);
//      std::pair p(2, 4.5);
//  }

//  {
//      template<class T> struct A
//      {
//          A(T, T);
//      };
//      auto y = new A{1, 2}; // allocated type is A<int>
//      }
//  }

//  {
//      auto mytuple()
//      {
//          char a = 'a';
//          int i = 123;
//          bool b = true;
//          return std::tuple(a, i, b);  // No types needed
//      }
//         Vs
//      auto mytuple()
//      {
//          return std::tuple(‘a’, 123, true);  // Auto type deduction from arguments
//      }
//  }

    std::pair<int, double> p1(2, 4.5); // old sty;e
    std::pair p2(2, 4.5); // C++17 style
    auto p3 = std::pair(2, 4.5); // C++17 style

    EXPECT_EQ(p1.first, p2.first);
    EXPECT_EQ(p1.second, p2.second);
        EXPECT_EQ(p3.first, p2.first);
        EXPECT_EQ(p3.second, p2.second);
    EXPECT_EQ(2, p1.first);
    EXPECT_EQ(4.5, p1.second);




    std::tuple<int, int, double> t1 = std::make_tuple(4, 3, 2.5); // old style
    std::tuple t2(4, 3, 2.5); // C++17 style
    auto t3 = std::tuple(4, 3, 2.5); // C++17 style

    EXPECT_EQ(std::get<0>(t1), std::get<0>(t2));
    EXPECT_EQ(std::get<1>(t1), std::get<1>(t2));
    EXPECT_EQ(std::get<2>(t1), std::get<2>(t2));
        EXPECT_EQ(std::get<0>(t3), std::get<0>(t2));
        EXPECT_EQ(std::get<1>(t3), std::get<1>(t2));
        EXPECT_EQ(std::get<2>(t3), std::get<2>(t2));
    EXPECT_EQ(4, std::get<0>(t1));
    EXPECT_EQ(3, std::get<1>(t1));
    EXPECT_EQ(2.5, std::get<2>(t1));
}




TEST(Cpp17, tuple_tie)
{
//  std::tuple //  It is a generalization of std::pair                                                       \
//  std::make_tuple(...)                                                                                     \
//                                                                                                           \
//  https://en.cppreference.com/w/cpp/utility/tuple/tie                                                      \
//  std::tie // Creates a tuple of lvalue references to its arguments or instances of std::ignore.           \
//                                                                                                           \
//  template< class... Types >                                                                               \
//  std::tuple<Types&...> tie( Types&... args ) noexcept;                                                    \
//                                                                                                           \
//  template< class... Types >                                                                               \
//  constexpr std::tuple<Types&...> tie( Types&... args ) noexcept;                                          \
//                                                                                                           \
//  https://lhcb.github.io/developkit-lessons/first-development-steps/05c-.html                         \

    {
        std::cout << "##################################(a):" << std::endl;
        auto a = std::make_tuple<std::string, std::string, int>("Aaa", "Aaaaaaaaaaaaa", 95);
        std::cout << "First Name:" << std::get<0>(a) << std::endl;
        std::cout << "Second Name Name:" << std::get<1>(a) << std::endl;
        std::cout << "Age:" << std::get<2>(a) << std::endl;
        EXPECT_EQ("Aaa", std::get<0>(a));
        EXPECT_EQ("Aaaaaaaaaaaaa", std::get<1>(a));
        EXPECT_EQ(95, std::get<2>(a));
    }




    {
        std::cout << "##################################(b):" << std::endl;
        std::tuple b("Bbb", "Bbbbbbbbbb", 17);
        std::cout << "First Name:" << std::get<0>(b) << std::endl;
        std::cout << "Second Name Name:" << std::get<1>(b) << std::endl;
        std::cout << "Age:" << std::get<2>(b) << std::endl;
        EXPECT_EQ("Bbb", std::get<0>(b));
        EXPECT_EQ("Bbbbbbbbbb", std::get<1>(b));
        EXPECT_EQ(17, std::get<2>(b));
    }




    {
        std::cout << "##################################(std::tie(sFirstName, sSecondName, age), old way):" << std::endl;
        std::string sFirstName;
        std::string sSecondName;
        int age;
        std::tie(sFirstName, sSecondName, age) = std::tuple("Ccc", "Cccccccccccc", 71);
        std::cout << "First Name:" << sFirstName << std::endl;
        std::cout << "Second Name Name:" << sSecondName << std::endl;
        std::cout << "Age:" << age << std::endl;
        EXPECT_EQ("Ccc", sFirstName);
        EXPECT_EQ("Cccccccccccc", sSecondName);
        EXPECT_EQ(71, age);
    }




    {
        std::cout << "##################################(std::tie(sFirstName, sSecondName, age), Cpp17 way):" << std::endl;
        auto [sFirstName, sSecondName, age] = std::tuple("Ccc.2", "Cccccccccccc.2", 72);
        std::cout << "First Name:" << sFirstName << std::endl;
        std::cout << "Second Name Name:" << sSecondName << std::endl;
        std::cout << "Age:" << age << std::endl;
        EXPECT_EQ("Ccc.2", sFirstName);
        EXPECT_EQ("Cccccccccccc.2", sSecondName);
        EXPECT_EQ(72, age);
    }




    {
        std::cout << "##################################(as ret value of a function)" << std::endl;
        auto f = [](int index){
            return std::tuple("Ddd" + std::to_string(index), "Ddddddd" + std::to_string(index), 2+index);
        };
        std::string sFirstName;
        std::string sSecondName;
        int age;

        std::tie(sFirstName, sSecondName, age) = f(0);
        std::cout << "First Name:" << sFirstName << std::endl;
        std::cout << "Second Name Name:" << sSecondName << std::endl;
        std::cout << "Age:" << age << std::endl;
        EXPECT_EQ("Ddd0", sFirstName);
        EXPECT_EQ("Ddddddd0", sSecondName);
        EXPECT_EQ(2+0, age);

        std::tie(sFirstName, sSecondName, age) = f(1);
        std::cout << "First Name:" << sFirstName << std::endl;
        std::cout << "Second Name Name:" << sSecondName << std::endl;
        std::cout << "Age:" << age << std::endl;
        EXPECT_EQ("Ddd1", sFirstName);
        EXPECT_EQ("Ddddddd1", sSecondName);
        EXPECT_EQ(2+1, age);
    }




    {
        std::cout << "##################################(modern C++17 way)" << std::endl;
        int ageParam = 10;
        auto f = [&ageParam](int index){
            return std::tuple("Eee" + std::to_string(index), "Eeeeeee" + std::to_string(index), ageParam);
        };
        {
            auto [sFirstName, sSecondName, age] = f(0);
            std::cout << "First Name:" << sFirstName << std::endl;
            std::cout << "Second Name Name:" << sSecondName << std::endl;
            std::cout << "Age:" << age << std::endl;
            EXPECT_EQ("Eee0", sFirstName);
            EXPECT_EQ("Eeeeeee0", sSecondName);
            EXPECT_EQ(10, ageParam);
            EXPECT_EQ(10, age);
        }

        ++ageParam;
        {
            auto [sFirstName, sSecondName, age] = f(1);
            std::cout << "First Name:" << sFirstName << std::endl;
            std::cout << "Second Name Name:" << sSecondName << std::endl;
            std::cout << "Age:" << age << std::endl;
            EXPECT_EQ("Eee1", sFirstName);
            EXPECT_EQ("Eeeeeee1", sSecondName);
            EXPECT_EQ(11, ageParam);
            EXPECT_EQ(11, age);
        }
    }
}




TEST(Cpp17, NestedNamesaces)
{
}




TEST(Cpp17, StdFilesystem)
{
    // std::filesytem
    EXPECT_EQ(true, std::filesystem::exists("/etc/hosts"));
    EXPECT_EQ(false, std::filesystem::exists("/etc/hostsAAAAAAA"));
}




TEST(Cpp17, StdOptional)
{
    // std::optional

//  https://en.cppreference.com/w/cpp/utility/optional
//  std::make_optional
//  constexpr explicit operator bool() const noexcept;
//  constexpr bool has_value() const noexcept;
//  EXPECT_TRUE(false);
    auto foo = [](int value){
        std::string result(std::to_string(value));
        bool isEven = (value%2==0);
        auto type = isEven ? " is even" : " is odd";
        result += type;
        if(isEven) return std::optional<std::string>(result);
        else return std::optional<std::string>();
    };

    for(int i=0; i<10; ++i)
    {
        std::string expected;
        auto value = foo(i);
        if(i%2==0)
        {
            expected = std::to_string(i) + " is even";
            EXPECT_EQ(expected, value);
        }
        else
        {
            EXPECT_TRUE(!value);
            expected = std::to_string(i) + " is odd";
        }
    }
}




TEST(Cpp17, StdStringView)
{
//  std::string_view
//  https://en.cppreference.com/w/cpp/string/basic_string_view
    std::string s("12345");

    std::string_view v(s);
    EXPECT_EQ("12345", v);

    std::reverse(s.begin(), s.end());
    EXPECT_EQ("54321", v);

    s[0] = 'A';
    EXPECT_EQ("A4321", v);

    std::reverse(s.begin(), s.end());
    EXPECT_EQ("1234A", v);
}




template<typename... Args>
bool And(Args... args) { return (... && args); }

template<typename... Args>
bool Or(Args... args) { return (... || args); }

template<typename ...Args>
int sum(Args&&... args)
{
    //return (args + ... + (1 * 2)); // OK
    return (args + ... ); // OK
}
 
TEST(Cpp17, FoldExpression)
{
//  fold expression
//  https://en.cppreference.com/w/cpp/language/fold

//  {
//      template<typename... Args>
//      bool And(Args... args) { return (... && args); }
//       
//      bool b = And(true, true, true, false);
//       // within And(), the unary left fold expands as
//       //  return ((true && true) && true) && false;
//       // b is false
//      }
//  }

    {
        bool result = And(true);
        EXPECT_EQ(true, result);

        result = And(false);
        EXPECT_EQ(false, result);

        result = And(true, true);
        EXPECT_EQ(true, result);

        result = And(true, false);
        EXPECT_EQ(false, result);

        result = And(false, false);
        EXPECT_EQ(false, result);
    }

    {
        bool result = Or(true);
        EXPECT_EQ(true, result);

        result = Or(false);
        EXPECT_EQ(false, result);

        result = Or(true, true);
        EXPECT_EQ(true, result);

        result = Or(true, false);
        EXPECT_EQ(true, result);

        result = Or(false, false);
        EXPECT_EQ(false, result);
    }

    {
        int result = sum(0);
        EXPECT_EQ(0, result);

        result = sum(0, 1);
        EXPECT_EQ(1, result);

        result = sum(0, 1, 2);
        EXPECT_EQ(3, result);

        result = sum(0, 1, 2, 3);
        EXPECT_EQ(6, result);
    }
}




TEST(Cpp17, SelectionInitialization)
{

    auto getval = [](){
        static auto counter = 0;
        ++counter;
//      std::cout << "counter:" << counter << std::endl;
//      std::cout << "counter.rem:" << (counter%2) << std::endl;

        int ret = (counter%2 == 0) ? 9 : 10;
//      std::cout << "counter.ret:" << ret << std::endl;
        return ret;
    };

    // Old way
    {
        auto a = getval();
        if (a < 10) {
            EXPECT_TRUE(false);
        } else {
            EXPECT_TRUE(true);
        }
    }
    // Vs C++17 way
    {
        if (auto a = getval(); a < 10) {
            EXPECT_TRUE(true);
        } else {
            EXPECT_TRUE(false);
        }
    }


    int caseA = 0, caseB = 0;
//  std::cout << "###############################" << std::endl;
    for(int i=0; i<11; ++i)
    {
//      std::cout << "############, i:" << i << std::endl;
        switch (auto a = getval(); a) {
            case 9:
                ++caseA;
                break;
            case 10:
                ++caseB;
                break;
            default:
            {
                EXPECT_TRUE(false);
            }
        }
    }
    EXPECT_EQ(5, caseA);
    EXPECT_EQ(6, caseB);
}

