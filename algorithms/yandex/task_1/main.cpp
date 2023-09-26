//  https://habr.com/ru/companies/yandex/articles/340784/

//  «Игра»

//  Условие

//  Петя и Вася играют в игру: они по очереди берут из колоды карточки, на которых написаны целые неповторяющиеся положительные числа (первый карточку всегда берет Петя). Карточки игроки берут по одной сверху колоды. После этого они сравнивают значения, записанные на карточках: игрок, у которого меньше, тянет еще одну карточку и оставляет ее у себя. Когда все карточки заканчиваются, Петя и Вася считают сумму значений, написанных на этих карточках. Проигрывает тот, у кого сумма получается меньше, чем у другого игрока.


//  Им надоело вручную тянуть карточки и сравнивать значения. Они попросили вас написать программу, которая по исходному набору карточек будет определять победителя.


//  Гарантируется, что для любого теста победителя можно будет определить однозначно.


//  Дополнительные параметры
//  Формат входных данных

//  В первой строке входных данных записано целое число $n$, кратное трем, — общее количество карточек $(3 \leq n \leq 999)$.

//  В следующей строке записаны $n$ различных целых положительных чисел $a_i$ — значения, написанные на карточках в том порядке, в котором Петя и Вася будут их раздавать $(1 \leq a_i \leq 1000)$.


//  Формат выходных данных

//  В единственной строке выведите Petya, если в игре побеждает Петя, или Vasya, если в игре побеждает Вася.


//  Примеры

//  Ввод	Вывод
//  3
//  1 2 3   Petya



//  Ввод	Вывод
//  3
//  1 4 2   Vasya

//  Ограничение по времени: 2 секунды

//  Ограничение по памяти: 64 МБ

#include <gtest/gtest.h>

using namespace std;

struct Impl
{
    struct Player
    {
        string _name;
        uint32_t _score;
    };

    tuple<const Player&, const Player&> run(Player& playerA, Player& playerB, const vector<uint16_t>& cards)
    {
        // Check that input sequnce size is event to 3
        if(!(cards.size() >= 3 && cards.size() <= 999))
        {
            string err("Input sequince size of " + to_string(cards.size()) + " shall be in range [3, 999]");
            throw invalid_argument(err);
        }

        // Check that input sequnce size is event to 3
        if(cards.size() % 3 != 0)
        {
            string err("Input sequince size of " + to_string(cards.size()) + " shall be even to 3");
            throw invalid_argument(err);
        }

        auto it = cards.begin();
        while(it != cards.end())
        {
            uint16_t a(0);
            uint16_t b(0);

            // player A picks a card
            a = *it;
            playerA._score += a;
//          cout << "player A:" << a << ", total:" << playerA._score << endl;

            // playerB picks a card
            ++it;
            b = *it;
            playerB._score += b;
//          cout << "player B:" << b << ", total:" << playerB._score << endl;

            // player with less card value picks a card
            ++it;
            if(a < b)
            {
                playerA._score += *it;
//              cout << "player A(extra card):" << *it << ", total:" << playerA._score << endl;
            }
            else
            {
                playerB._score += *it;
//              cout << "player B(extra card):" << *it << ", total:" << playerB._score << endl;
            }

            ++it;
        }

        if(playerA._score > playerB._score)
        {
            return make_tuple(std::cref(playerA), std::cref(playerB));
        }
        else
        {
            return make_tuple(std::cref(playerB), std::cref(playerA));
        }
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


TEST_F(ToBeTested, case_0_empty_input)
{
    try
    {
        vector<uint16_t> cards;
        Impl::Player Petya{"Petya", 0};
        Impl::Player Vasja{"Vasja", 0};
        auto result = tbt().run(Petya, Vasja, cards);
        FAIL();
    }
    catch(invalid_argument& err)
    {
        EXPECT_EQ(err.what(), string("Input sequince size of 0 shall be in range [3, 999]"));
    }
    catch(...)
    {
        FAIL();
    }
}

TEST_F(ToBeTested, case_1_oversized_input)
{
    try
    {
        vector<uint16_t> cards(1000);
        Impl::Player Petya{"Petya", 0};
        Impl::Player Vasja{"Vasja", 0};
        auto result = tbt().run(Petya, Vasja, cards);
        FAIL();
    }
    catch(invalid_argument& err)
    {
        EXPECT_EQ(err.what(), string("Input sequince size of 1000 shall be in range [3, 999]"));
    }
    catch(...)
    {
        FAIL();
    }
}

TEST_F(ToBeTested, case_2_incorrect_input_size)
{
    try
    {
        vector<uint16_t> cards{1, 2, 3, 4};
        Impl::Player Petya{"Petya", 0};
        Impl::Player Vasja{"Vasja", 0};
        auto result = tbt().run(Petya, Vasja, cards);
        FAIL();
    }
    catch(invalid_argument& err)
    {
        EXPECT_EQ(err.what(), string("Input sequince size of 4 shall be even to 3"));
    }
    catch(...)
    {
        FAIL();
    }
}


TEST_F(ToBeTested, case_3)
{
    vector<uint16_t> cards{1, 2, 3};
    Impl::Player Petya{"Petya", 0};
    Impl::Player Vasja{"Vasja", 0};
    auto result = tbt().run(Petya, Vasja, cards);

    EXPECT_EQ("Petya", get<0>(result)._name);
    EXPECT_EQ(1+3, get<0>(result)._score);

    EXPECT_EQ("Vasja", get<1>(result)._name);
    EXPECT_EQ(2, get<1>(result)._score);
}


TEST_F(ToBeTested, case_4)
{
    vector<uint16_t> cards{1, 4, 2};
    Impl::Player Petya{"Petya", 0};
    Impl::Player Vasja{"Vasja", 0};
    auto result = tbt().run(Petya, Vasja, cards);

    EXPECT_EQ("Vasja", get<0>(result)._name);
    EXPECT_EQ(4, get<0>(result)._score);

    EXPECT_EQ("Petya", get<1>(result)._name);
    EXPECT_EQ(1+2, get<1>(result)._score);
}



TEST_F(ToBeTested, case_5)
{
    vector<uint16_t> cards;
    cards.reserve(999);
    for(uint16_t i=1; i<=999; ++i)
    {
        cards.push_back(i);
    }
    Impl::Player Petya{"Petya", 0};
    Impl::Player Vasja{"Vasja", 0};
    auto result = tbt().run(Petya, Vasja, cards);

    EXPECT_EQ("Petya", get<0>(result)._name);
    EXPECT_EQ(333000, get<0>(result)._score);

    EXPECT_EQ("Vasja", get<1>(result)._name);
    EXPECT_EQ(166500, get<1>(result)._score);
}

