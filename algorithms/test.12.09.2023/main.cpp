#include <gtest/gtest.h>
#include <iterator>
#include <vector>

using namespace std;

// Implemetation assumptions:
// 1. To make the algorithm container(type) agnostic lets' receive just iterators to begin and end of incomig sequence
// 2. The algorithm shall be iteratable to reduce memory usage algorithm will return a couple of iterators to sub-sequence upon every iteration
// 2. To reduce memory usage the algorithm shall be iterable and return a couple of iterators to sub-sequence upon every iteration

struct Impl
{
    template<typename Iterator, typename Delimeter>
    static tuple<Iterator, Iterator> split(Iterator begin, Iterator end, Delimeter d)
    {
        // skip leading delimeters
        while(*begin == d && begin != end)
            ++begin;

        for(auto e = begin; e != end; ++e)
        {
            if(*e == d)
            {
                return make_tuple(begin, e);
            }
        }

        // splitting is complete
        return make_tuple(begin, end);
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




TEST_F(ToBeTested, case_0_empty_string)
{
    vector<string> results;

    string str("");
    for(auto it = str.begin(); it != str.end(); it)
    {
        auto [subBegin, subEnd] =  ToBeTested::Component::split(it, str.end(), ' ');
        if(subBegin != subEnd)
        {
            string subStr(subBegin, subEnd);
            results.push_back(subStr);
        }
        it = subEnd;
    }

    EXPECT_EQ(0, results.size());
}




TEST_F(ToBeTested, case_1_delimetes_only)
{
    vector<string> results;

    string str("   ");
    for(auto it = str.begin(); it != str.end(); it)
    {
        auto [subBegin, subEnd] =  ToBeTested::Component::split(it, str.end(), ' ');
        if(subBegin != subEnd)
        {
            string subStr(subBegin, subEnd);
            results.push_back(string(subBegin, subEnd));
        }
        it = subEnd;
    }

    EXPECT_EQ(0, results.size());
}




TEST_F(ToBeTested, case_2_single_element)
{
    vector<string> results;

    string str("some");
    for(auto it = str.begin(); it != str.end(); it)
    {
        auto [subBegin, subEnd] =  ToBeTested::Component::split(it, str.end(), ' ');
        if(subBegin != subEnd)
        {
            string subStr(subBegin, subEnd);
            results.push_back(subStr);
        }
        it = subEnd;
    }

    EXPECT_EQ(1, results.size());
    EXPECT_EQ("some", results[0]);
}




TEST_F(ToBeTested, case_3_single_element_leading_delimeter)
{
    vector<string> results;

    string str("  some");
    for(auto it = str.begin(); it != str.end(); it)
    {
        auto [subBegin, subEnd] =  ToBeTested::Component::split(it, str.end(), ' ');
        if(subBegin != subEnd)
        {
            string subStr(subBegin, subEnd);
            results.push_back(subStr);
        }
        it = subEnd;
    }

    EXPECT_EQ(1, results.size());
    EXPECT_EQ("some", results[0]);
}




TEST_F(ToBeTested, case_4_single_element_trailing_delimeter)
{
    vector<string> results;

    string str("some  ");
    for(auto it = str.begin(); it != str.end(); it)
    {
        auto [subBegin, subEnd] =  ToBeTested::Component::split(it, str.end(), ' ');
        if(subBegin != subEnd)
        {
            string subStr(subBegin, subEnd);
            results.push_back(subStr);
        }
        it = subEnd;
    }

    EXPECT_EQ(1, results.size());
    EXPECT_EQ("some", results[0]);
}




TEST_F(ToBeTested, case_5_single_element_leading_and__trailing_delimeter)
{
    vector<string> results;

    string str("   some  ");
    for(auto it = str.begin(); it != str.end(); it)
    {
        auto [subBegin, subEnd] =  ToBeTested::Component::split(it, str.end(), ' ');
        if(subBegin != subEnd)
        {
            string subStr(subBegin, subEnd);
            results.push_back(subStr);
        }
        it = subEnd;
    }

    EXPECT_EQ(1, results.size());
    EXPECT_EQ("some", results[0]);
}




TEST_F(ToBeTested, case_6)
{
    vector<string> results;

    string str("some text with delimeters");
    for(auto it = str.begin(); it != str.end(); it)
    {
        auto [subBegin, subEnd] =  ToBeTested::Component::split(it, str.end(), ' ');
        if(subBegin != subEnd)
        {
            string subStr(subBegin, subEnd);
            results.push_back(subStr);
        }
        it = subEnd;
    }

    EXPECT_EQ(4, results.size());
    EXPECT_EQ("some", results[0]);
    EXPECT_EQ("text", results[1]);
    EXPECT_EQ("with", results[2]);
    EXPECT_EQ("delimeters", results[3]);
}

