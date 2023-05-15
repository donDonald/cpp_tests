#include <gtest/gtest.h>
#include "HashMap.h"
#include <algorithm>

using HashMapUint8String = HashMap<uint8_t, std::string>;

class HashMapBucket: public ::testing::Test
{ 
public: 
    using Component = HashMapUint8String::Bucket;
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

TEST_F(HashMapBucket, reserve)
{
    EXPECT_EQ(0, tbt().size());
    EXPECT_EQ(16, tbt()._maxSize);
    EXPECT_EQ(nullptr, tbt().find(0));
    EXPECT_EQ(false, tbt().exists(0));
    EXPECT_EQ(nullptr, ctbt().find(0));
    EXPECT_EQ(false, ctbt().exists(0));

    tbt().reserve(16+8);
    EXPECT_EQ(16+8, tbt()._maxSize);

    tbt().reserve(16+8+32);
    EXPECT_EQ(16+8+32, tbt()._maxSize);
}

TEST_F(HashMapBucket, insert)
{
    EXPECT_EQ(0, tbt().size());
    EXPECT_EQ(nullptr, tbt().find(0));
    EXPECT_EQ(false, tbt().exists(0));

    EXPECT_EQ("a", *tbt().insert(10, "a"));
    EXPECT_EQ(1, tbt().size());
    EXPECT_EQ(true, tbt().exists(10));
    EXPECT_EQ("a", tbt()[10]);

    EXPECT_EQ("b", *tbt().insert(20, "b"));
    EXPECT_EQ(2, tbt().size());
    EXPECT_EQ(true, tbt().exists(20));
    EXPECT_EQ("b", tbt()[20]);
}

TEST_F(HashMapBucket, remove)
{
    EXPECT_EQ(0, tbt().size());
    EXPECT_EQ(nullptr, tbt().find(0));
    EXPECT_EQ(false, tbt().exists(0));

    EXPECT_EQ(false, tbt().remove(10));

    tbt().insert(10, "a");
    EXPECT_EQ(1, tbt().size());
    EXPECT_EQ(true, tbt().exists(10));
    EXPECT_EQ("a", tbt()[10]);

    EXPECT_EQ(true, tbt().remove(10));
    EXPECT_EQ(0, tbt().size());
    EXPECT_EQ(false, tbt().exists(10));

    tbt().insert(20, "b");
    EXPECT_EQ(1, tbt().size());
    EXPECT_EQ(true, tbt().exists(20));
    EXPECT_EQ("b", tbt()[20]);

    tbt().insert(30, "c");
    EXPECT_EQ(2, tbt().size());
    EXPECT_EQ(true, tbt().exists(30));
    EXPECT_EQ("c", tbt()[30]);

    tbt().insert(40, "d");
    EXPECT_EQ(3, tbt().size());
    EXPECT_EQ(true, tbt().exists(40));
    EXPECT_EQ("d", tbt()[40]);
}

TEST_F(HashMapBucket, insert_remove_find_reserve)
{
    EXPECT_EQ(0, tbt().size());

    EXPECT_EQ("str 40", *tbt().insert(40, "str 40"));
    EXPECT_EQ(1, tbt().size());
    EXPECT_EQ(true, ctbt().exists(40));
    EXPECT_EQ("str 40", *ctbt().find(40));

    EXPECT_EQ("str 50", *tbt().insert(50, "str 50"));
    EXPECT_EQ(2, tbt().size());
    EXPECT_EQ(true, ctbt().exists(50));
    EXPECT_EQ("str 50", *ctbt().find(50));

    EXPECT_EQ("str 60", *tbt().insert(60, "str 60"));
    EXPECT_EQ(3, tbt().size());
    EXPECT_EQ(true, ctbt().exists(60));
    EXPECT_EQ("str 60", *ctbt().find(60));

    EXPECT_EQ(true, tbt().remove(50));
    EXPECT_EQ(2, tbt().size());

    EXPECT_EQ(16, tbt()._maxSize);
    tbt().reserve(16+16);
    EXPECT_EQ(32, tbt()._maxSize);

    EXPECT_EQ("str 70", *tbt().insert(70, "str 70"));
    EXPECT_EQ(3, tbt().size());
    EXPECT_EQ(true, ctbt().exists(70));
    EXPECT_EQ("str 70", *ctbt().find(70));

    EXPECT_EQ("str 80", *tbt().insert(80, "str 80"));
    EXPECT_EQ(4, tbt().size());
    EXPECT_EQ(true, ctbt().exists(80));
    EXPECT_EQ("str 80", *ctbt().find(80));

    EXPECT_EQ(16+16, tbt()._maxSize);
    tbt().reserve(16+16+16);
    EXPECT_EQ(48, tbt()._maxSize);

    EXPECT_EQ(true, tbt().remove(40));
    EXPECT_EQ(3, tbt().size());
}

TEST_F(HashMapBucket, auto_reserve)
{
    EXPECT_EQ(0, tbt().size());
    EXPECT_EQ(16, tbt()._maxSize);
    for(size_t i=0; i<16; ++i)
    {
        tbt().insert(i, std::to_string(i));
    }
    EXPECT_EQ(16, tbt().size());
    EXPECT_EQ(16, tbt()._maxSize);
    for(size_t i=16; i<16+100; ++i)
    {
        tbt().insert(i, std::to_string(i));
    }
    EXPECT_EQ(16+100, tbt().size());
    EXPECT_EQ(16 + 32 + 32 + 32 + 16, tbt()._maxSize);
}




class HashMapString: public ::testing::Test
{
public:
    using Component = HashMapUint8String;
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

TEST_F(HashMapString, reserve)
{
    EXPECT_EQ(0, tbt().size());
    EXPECT_EQ(16, tbt()._maxSize);
    EXPECT_EQ(nullptr, tbt().find(0));
    EXPECT_EQ(false, tbt().exists(0));
    EXPECT_EQ(nullptr, ctbt().find(0));
    EXPECT_EQ(false, ctbt().exists(0));

    tbt().reserve(16+8);
    EXPECT_EQ(16+8, tbt()._maxSize);

    tbt().reserve(16+8+32);
    EXPECT_EQ(16+8+32, tbt()._maxSize);
}

TEST_F(HashMapString, insert_1_element)
{
    tbt().insert(0, "value 0");
    EXPECT_EQ(1, tbt().size());
    EXPECT_EQ(true, tbt().exists(0));

    EXPECT_EQ("value 0", *tbt().find(0));
    EXPECT_EQ("value 0", *ctbt().find(0));

    EXPECT_EQ("value 0", tbt()[0]);
    EXPECT_EQ("value 0", ctbt()[0]);
}

TEST_F(HashMapString, insert_the_same_element_for_many_times)
{
    tbt().insert(0, "value 0");
    EXPECT_EQ(1, tbt().size());
    EXPECT_EQ(true, tbt().exists(0));

    EXPECT_EQ("value 0", *tbt().find(0));
    EXPECT_EQ("value 0", *ctbt().find(0));

    EXPECT_EQ("value 0", tbt()[0]);
    EXPECT_EQ("value 0", ctbt()[0]);

    tbt().insert(0, "value 0");
    EXPECT_EQ(1, tbt().size());
    EXPECT_EQ(true, tbt().exists(0));

    EXPECT_EQ("value 0", *tbt().find(0));
    EXPECT_EQ("value 0", *ctbt().find(0));

    EXPECT_EQ("value 0", tbt()[0]);
    EXPECT_EQ("value 0", ctbt()[0]);

    tbt().insert(0, "value 0");
    EXPECT_EQ(1, tbt().size());
    EXPECT_EQ(true, tbt().exists(0));

    EXPECT_EQ("value 0", *tbt().find(0));
    EXPECT_EQ("value 0", *ctbt().find(0));

    EXPECT_EQ("value 0", tbt()[0]);
    EXPECT_EQ("value 0", ctbt()[0]);
}

TEST_F(HashMapString, insert_many_elements)
{
    std::vector<int> keys {-100, -78, -49, -37, -29, -26, -21, -17, -10, -6, -2, -1, 0, 4, 8, 9, 12, 17, 19, 22, 27, 30, 33, 47, 50, 79, 83, 99};
    std::random_shuffle(keys.begin(), keys.end());
    for(int key : keys)
    {
        //for(auto key: keys) std::cout << "key:" << key << std::endl;
        tbt().insert(key, "value " + std::to_string(key));
    }
    EXPECT_EQ(keys.size(), tbt().size());

    for(int key : keys)
    {
        auto value = tbt().find(key);
        EXPECT_TRUE(value != nullptr);
        auto expected = "value " + std::to_string(key);
        EXPECT_EQ(expected, *value);
        EXPECT_EQ(expected, tbt()[key]);
    }
}

TEST_F(HashMapString, remove_not_existing_element)
{
    tbt().remove(0);
    EXPECT_EQ(0, tbt().size());
    EXPECT_EQ(false, tbt().exists(0));
}

TEST_F(HashMapString, insert_1_element_and_remove)
{
    tbt().insert(0, "value 0");
    EXPECT_EQ(1, tbt().size());
    EXPECT_EQ(true, tbt().exists(0));

    tbt().remove(0);
    EXPECT_EQ(0, tbt().size());
    EXPECT_EQ(false, tbt().exists(0));
}

TEST_F(HashMapString, remove)
{
    EXPECT_EQ(0, tbt().size());
    EXPECT_EQ(nullptr, tbt().find(0));
    EXPECT_EQ(false, tbt().exists(0));

    EXPECT_EQ(false, tbt().remove(10));

    tbt().insert(10, "a");
    EXPECT_EQ(1, tbt().size());
    EXPECT_EQ(true, tbt().exists(10));
    EXPECT_EQ("a", tbt()[10]);

    EXPECT_EQ(true, tbt().remove(10));
    EXPECT_EQ(0, tbt().size());
    EXPECT_EQ(false, tbt().exists(10));

    tbt().insert(20, "b");
    EXPECT_EQ(1, tbt().size());
    EXPECT_EQ(true, tbt().exists(20));
    EXPECT_EQ("b", tbt()[20]);

    tbt().insert(30, "c");
    EXPECT_EQ(2, tbt().size());
    EXPECT_EQ(true, tbt().exists(30));
    EXPECT_EQ("c", tbt()[30]);

    tbt().insert(40, "d");
    EXPECT_EQ(3, tbt().size());
    EXPECT_EQ(true, tbt().exists(40));
    EXPECT_EQ("d", tbt()[40]);
}

TEST_F(HashMapString, insert_many_elements_and_remove_one_by_one)
{
    std::vector<int> keys {-100, -78, -49, -37, -29, -26, -21, -17, -10, -6, -2, -1, 0, 4, 8, 9, 12, 17, 19, 22, 27, 30, 33, 47, 50, 79, 83, 99};

    {
        std::vector<int> tmpKeys(keys);
        std::random_shuffle(tmpKeys.begin(), tmpKeys.end());
        for(auto key : tmpKeys)
        {
            tbt().insert(key, "value " + std::to_string(key));
        }
    }
    EXPECT_EQ(keys.size(), tbt().size());
    EXPECT_EQ(28, tbt().size());

    for(int i=0; i<keys.size(); ++i)
    {
        auto key = keys[i];
        EXPECT_EQ(true, tbt().exists(key));
        tbt().remove(key);

        EXPECT_EQ(false, tbt().exists(key));
        EXPECT_EQ(keys.size()-(i+1), tbt().size());
    }

    EXPECT_EQ(tbt().size(), 0);
}

