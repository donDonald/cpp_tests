
//  https://proglib.io/p/algorithm-tasks

//  2. Результаты олимпиады
//  Во время проведения олимпиады каждый из участников получил свой идентификационный номер. Необходимо отсортировать список участников олимпиады по количеству набранных ими баллов.

//  Формат входных данных:
//  На первой строке дано число n (1 ≤ n ≤ 1000) – количество участников. На каждой следующей строке даны идентификационный номер и набранное число баллов соответствующего участника. Все числа во входном файле целые и не превышают 10^5.

//  Формат выходных данных:
//  Выведите исходный список в порядке убывания баллов. Если у некоторых участников одинаковые баллы, то их нужно отсортировать в порядке возрастания идентификационного номера.


//  Примеры:

//  Входные данные      Выходные данные
//  3
//  101 80              305 90
//  305 90              101 80
//  200 14              200 14


//  Входные данные	    Выходные данные
//  3
//  20 80               25 90
//  30 90               30 90
//  25 90               20 80


#include <gtest/gtest.h>
#include <unordered_set>

using namespace std;


struct Impl
{
    static void sort(vector<tuple<uint32_t, uint32_t>>& data)
    {
        // Chek incoming Ids for uniquness
        unordered_set<uint32_t> ids(data.size());
        for(auto d: data)
        {
            auto id = std::get<0>(d);
            if(ids.find(id) != ids.end())
            {
                string err("Id " + to_string(id) + " is duplicated");
                throw invalid_argument(err);
            }
            ids.insert(id);
        }

        auto cmp = [](const tuple<uint32_t, uint32_t>& a, const tuple<uint32_t, uint32_t>& b) -> bool
        {
            auto scoreA = get<1>(a);
            auto scoreB = get<1>(b);

            if(scoreA > scoreB)
            {
                return true;
            }
            else if(scoreA == scoreB)
            {
                auto idA = get<0>(a);
                auto idB = get<0>(b);
                return idA < idB;
            }

            return false;
        };

        std::sort(data.begin(), data.end(), cmp);
    }
};


template<typename ToBeTested>
class Test : public ::testing::Test
{
public:
    using Component = ToBeTested;
    using ComponentPtr = shared_ptr<Component>;

    void SetUp()
    {
        _tbt = make_shared<Component>();
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
    vector<tuple<uint32_t, uint32_t>> data {};
    Impl::sort(data);    
    for(auto d : data)
    {
       cout << std::get<0>(d) << " " << std::get<1>(d) << endl;
    }
    EXPECT_EQ(0, data.size());
}


TEST_F(ToBeTested, case_1)
{
    vector<tuple<uint32_t, uint32_t>> data {tuple{101, 80}, tuple{305, 90}, tuple{200, 14}};
    Impl::sort(data);    
    for(auto d : data)
    {
       cout << std::get<0>(d) << " " << std::get<1>(d) << endl;
    }
    EXPECT_EQ(3, data.size());

    EXPECT_EQ(305, get<0>(data[0]));
    EXPECT_EQ(90,  get<1>(data[0]));

    EXPECT_EQ(101, get<0>(data[1]));
    EXPECT_EQ(80,  get<1>(data[1]));

    EXPECT_EQ(200, get<0>(data[2]));
    EXPECT_EQ(14,  get<1>(data[2]));
}


TEST_F(ToBeTested, case_2)
{
    vector<tuple<uint32_t, uint32_t>> data {tuple{20, 80}, tuple{30, 90}, tuple{25, 90}};
    Impl::sort(data);    
    for(auto d : data)
    {
       cout << std::get<0>(d) << " " << std::get<1>(d) << endl;
    }
    EXPECT_EQ(3, data.size());

    EXPECT_EQ(25, get<0>(data[0]));
    EXPECT_EQ(90,  get<1>(data[0]));

    EXPECT_EQ(30, get<0>(data[1]));
    EXPECT_EQ(90,  get<1>(data[1]));

    EXPECT_EQ(20, get<0>(data[2]));
    EXPECT_EQ(80,  get<1>(data[2]));
}


TEST_F(ToBeTested, case_3_duplicated_element)
{
    try
    {
        vector<tuple<uint32_t, uint32_t>> data {tuple{20, 80}, tuple{30, 90}, tuple{25, 90}, tuple{25, 90}};
        Impl::sort(data);    
        FAIL();
    }
    catch(invalid_argument& err)
    {
        EXPECT_EQ(err.what(), string("Id 25 is duplicated"));
    }
    catch(...)
    {
        FAIL();
    }
}

