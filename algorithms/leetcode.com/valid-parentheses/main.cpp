
#include <gtest/gtest.h>

using namespace std;

// assumptions
// 1. между скобками одного уровня должно быть число элементов кратное 2-м: ([]), но не {[]{}
// 2. всё что между скобками и кратное двум должно само быть валидно - рекурсия?

// Что если просто инкрементировать на единичу при открывании и декрементировать при закрывании -> есл резульаь 0 -> всё ок
// i.e. [] => 0->1->0;
// i.e. [{}({})] => 0->1->0->1->2->1->0

#include <stack>

struct Impl
{
    struct Closure
    {
        char begin;
        char end;
    };

    Closure c0{'(', ')'};
    Closure c1{'{', '}'};
    Closure c2{'[', ']'};

    std::tuple<bool, char, char> isOpening(char c)
    {
        if(c == c0.begin)
            return make_tuple(true, c0.begin, c0.end); 
        if(c == c1.begin)
            return make_tuple(true, c1.begin, c1.end); 
        if(c == c2.begin)
            return make_tuple(true, c2.begin, c2.end); 

        return make_tuple(false, 'a', 'a');
    }

    std::tuple<bool, char, char> isClosure(char c)
    {
        if(c == c0.end)
            return make_tuple(true, c0.begin, c0.end); 
        if(c == c1.end)
            return make_tuple(true, c1.begin, c1.end); 
        if(c == c2.end)
            return make_tuple(true, c2.begin, c2.end); 

        return make_tuple(false, 'a', 'a');
    }

    bool isValid(const string& s)
    {
        if(s.size()%2 != 0)
            return false; // Simple check since correct string shall contain 2*n elements

        std::stack<Closure> stack;
        
        for(size_t i=0; i<s.size(); ++i)
        {
            auto c = s[i];
            auto [opening, begin1, end1] = isOpening(c);
            auto [closure, begin2, end2] = isClosure(c);
//          cout << "--------------" << c << endl;
//          cout << "step[" << i << "], c:" << c << endl;
            if(opening)
            {
//              cout << "opening:" << opening << ", begin1:" << begin1 << ", end1:" << end1 << endl;
//              cout << "step[" << i << "] opening - pushing:" << begin1 << endl;
                stack.push( Closure{begin1, end1} );

            }
            else if(closure)
            {
//              cout << "closure:" << closure << ", begin2:" << begin2 << ", end2:" << end2 << endl;
                if(stack.top().end == end2)
                {
                    stack.pop();
//                  cout << "step[" << i << "] closure - popping:" << end2 << endl;
                }
            }
            else
            {
                assert(false);
            }
        }

        return stack.size() == 0; 
    }
};




template<typename ToBeTested>
class Test : public ::testing::Test
{
public:
    using Component = ToBeTested;
    using ComponentPtr = std::shared_ptr<Component>;

    void SetUp()
    {
        _tbt = std::make_shared<Component>();
    }

    void TearDown()
    {
        _tbt.reset();
    }

    Component& tbt() { return *_tbt; }
    const Component& ctbt() const { return *_tbt; }
    ComponentPtr _tbt; // tbt - "to be tested"
};

using ToBeTested = Test<Impl>;




TEST_F(ToBeTested, case_0)
{
    string s("()");
    auto result = tbt().isValid(s);
    EXPECT_EQ(true, result);
}

TEST_F(ToBeTested, case_1)
{
    string s("({})");
    auto result = tbt().isValid(s);
    EXPECT_EQ(true, result);
}

TEST_F(ToBeTested, case_2)
{
    string s("({})[]");
    auto result = tbt().isValid(s);
    EXPECT_EQ(true, result);
}

TEST_F(ToBeTested, case_4)
{
    string s("({})[()]");
    auto result = tbt().isValid(s);
    EXPECT_EQ(true, result);
}

TEST_F(ToBeTested, case_5)
{
    string s("({})[(){}]");
    auto result = tbt().isValid(s);
    EXPECT_EQ(true, result);
}

TEST_F(ToBeTested, case_6)
{
    string s("({})[(){}](()(()))");
    auto result = tbt().isValid(s);
    EXPECT_EQ(true, result);
}

TEST_F(ToBeTested, failure_0)
{
    string s("(");
    auto result = tbt().isValid(s);
    EXPECT_EQ(false, result);
}

TEST_F(ToBeTested, failure_1)
{
    string s("(]");
    auto result = tbt().isValid(s);
    EXPECT_EQ(false, result);
}

TEST_F(ToBeTested, failure_2)
{
    string s("({])");
    auto result = tbt().isValid(s);
    EXPECT_EQ(false, result);
}

TEST_F(ToBeTested, failure_3)
{
    string s("({})[(){}](()(())");
    auto result = tbt().isValid(s);
    EXPECT_EQ(false, result);
}

