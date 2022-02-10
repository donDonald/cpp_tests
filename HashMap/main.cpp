#include <gtest/gtest.h>
#include "HashMap.h"
#include <algorithm>

using HashMapTypeA = HashMap<uint8_t, std::string>;

class TestStuff : public ::testing::Test
{ 
public: 
    using Component = HashMapTypeA;
    using ComponentPtr = std::shared_ptr<Component>;

    void SetUp()
    {
        _tbt = std::make_shared<Component>(10);
    }

    void TearDown()
    {
        _tbt.reset();
    }

    Component& tbt() { return *_tbt; }
    const Component& ctbt() const { return *_tbt; }
    ComponentPtr _tbt; // tbt - "to be tested"
};

TEST_F(TestStuff, empty_container)
{
    EXPECT_EQ(0, tbt().size());
    EXPECT_EQ(10, tbt().maxSize());
    EXPECT_EQ(false, tbt().keyExists(0));
    EXPECT_EQ(nullptr, tbt().find(0));
    EXPECT_EQ(nullptr, ctbt().find(0));
}

//  TEST_F(TestStuff, insert_1_element_than_check)
//  {
//      tbt().insert(0, "value 0");
//      EXPECT_EQ(1, tbt().size());
//      EXPECT_EQ(true, tbt().keyExists(0));

//  //  auto value = ttt().find(0);
//  //  EXPECT_TRUE(value != nullptr);
//  //  EXPECT_EQ("value 0", *value);

//  //  const auto cvalue = cttt().find(0);
//  //  EXPECT_TRUE(cvalue != nullptr);
//  //  EXPECT_EQ("value 0", *cvalue);

//  //  EXPECT_EQ("value 0", ttt().at(0));
//  //  EXPECT_EQ("value 0", cttt().at(0));
//  }


















///     TEST_F(BTreeTest, insert_the_same_element_for_many_times)
///     {
///         ttt().insert(0, "value 0");
///         EXPECT_EQ(1, ttt().size());
///         EXPECT_EQ(true, ttt().keyExists(0));
///         EXPECT_EQ("value 0", cttt().at(0));

///         ttt().insert(0, "value 00");
///         EXPECT_EQ(1, ttt().size());
///         EXPECT_EQ(true, ttt().keyExists(0));
///         EXPECT_EQ("value 00", cttt().at(0));

///         ttt().insert(0, "value 000");
///         EXPECT_EQ(1, ttt().size());
///         EXPECT_EQ(true, ttt().keyExists(0));
///         EXPECT_EQ("value 000", cttt().at(0));
///     }

///     TEST_F(BTreeTest, insert_many_elements)
///     {
///         std::vector<int> keys {-100, -78, -49, -37, -29, -26, -21, -17, -10, -6, -2, -1, 0, 4, 8, 9, 12, 17, 19, 22, 27, 30, 33, 47, 50, 79, 83, 99};
///         std::random_shuffle(keys.begin(), keys.end());
///         for(int key : keys)
///         {
///             //for(auto key: keys) std::cout << "key:" << key << std::endl;
///             ttt().insert(key, "value " + std::to_string(key));
///         }
///         EXPECT_EQ(keys.size(), ttt().size());

///         for(int key : keys)
///         {
///             auto value = ttt().find(key);
///             EXPECT_TRUE(value != nullptr);
///             auto expected = "value " + std::to_string(key);
///             EXPECT_EQ(expected, *value);
///             EXPECT_EQ(expected, ttt().at(key));
///         }
///     }

///     TEST_F(BTreeTest, remove_not_existing_element)
///     {
///         ttt().remove(0);
///         EXPECT_EQ(0, ttt().size());
///         EXPECT_EQ(false, ttt().keyExists(0));
///     }

///     TEST_F(BTreeTest, insert_1_element_and_remove)
///     {
///         ttt().insert(0, "value 0");
///         EXPECT_EQ(1, ttt().size());
///         EXPECT_EQ(true, ttt().keyExists(0));

///         ttt().remove(0);
///         EXPECT_EQ(0, ttt().size());
///         EXPECT_EQ(false, ttt().keyExists(0));
///     }

///     TEST_F(BTreeTest, insert_3_elements_and_remove_from_bottom_to_top)
///     {
///         ttt().insert(0, "value 0");
///         EXPECT_EQ(1, ttt().size());
///         EXPECT_EQ(true, ttt().keyExists(0));

///         ttt().insert(1, "value 1");
///         EXPECT_EQ(2, ttt().size());
///         EXPECT_EQ(true, ttt().keyExists(1));

///         ttt().insert(2, "value 2");
///         EXPECT_EQ(3, ttt().size());
///         EXPECT_EQ(true, ttt().keyExists(1));

///         ttt().remove(2);
///         EXPECT_EQ(2, ttt().size());
///         EXPECT_EQ(false, ttt().keyExists(2));

///         ttt().remove(1);
///         EXPECT_EQ(1, ttt().size());
///         EXPECT_EQ(false, ttt().keyExists(1));

///         ttt().remove(0);
///         EXPECT_EQ(0, ttt().size());
///         EXPECT_EQ(false, ttt().keyExists(0));
///     }

///     TEST_F(BTreeTest, insert_3_elements_and_remove_from_top_to_bottom)
///     {
///         ttt().insert(0, "value 0");
///         EXPECT_EQ(1, ttt().size());
///         EXPECT_EQ(true, ttt().keyExists(0));

///         ttt().insert(1, "value 1");
///         EXPECT_EQ(2, ttt().size());
///         EXPECT_EQ(true, ttt().keyExists(1));

///         ttt().insert(2, "value 2");
///         EXPECT_EQ(3, ttt().size());
///         EXPECT_EQ(true, ttt().keyExists(1));

///         ttt().remove(0);
///         EXPECT_EQ(2, ttt().size());
///         EXPECT_EQ(false, ttt().keyExists(0));

///         ttt().remove(1);
///         EXPECT_EQ(1, ttt().size());
///         EXPECT_EQ(false, ttt().keyExists(1));

///         ttt().remove(2);
///         EXPECT_EQ(0, ttt().size());
///         EXPECT_EQ(false, ttt().keyExists(2));
///     }

///     TEST_F(BTreeTest, insert_3_elements_and_remove_middle_top_bottom)
///     {
///         ttt().insert(0, "value 0");
///         EXPECT_EQ(1, ttt().size());
///         EXPECT_EQ(true, ttt().keyExists(0));

///         ttt().insert(1, "value 1");
///         EXPECT_EQ(2, ttt().size());
///         EXPECT_EQ(true, ttt().keyExists(1));

///         ttt().insert(2, "value 2");
///         EXPECT_EQ(3, ttt().size());
///         EXPECT_EQ(true, ttt().keyExists(1));

///         ttt().remove(1);
///         EXPECT_EQ(2, ttt().size());
///         EXPECT_EQ(false, ttt().keyExists(1));

///         ttt().remove(0);
///         EXPECT_EQ(1, ttt().size());
///         EXPECT_EQ(false, ttt().keyExists(0));

///         ttt().remove(2);
///         EXPECT_EQ(0, ttt().size());
///         EXPECT_EQ(false, ttt().keyExists(2));
///     }

///     TEST_F(BTreeTest, insert_3_elements_and_remove_middle_bottom_top)
///     {
///         ttt().insert(0, "value 0");
///         EXPECT_EQ(1, ttt().size());
///         EXPECT_EQ(true, ttt().keyExists(0));

///         ttt().insert(1, "value 1");
///         EXPECT_EQ(2, ttt().size());
///         EXPECT_EQ(true, ttt().keyExists(1));

///         ttt().insert(2, "value 2");
///         EXPECT_EQ(3, ttt().size());
///         EXPECT_EQ(true, ttt().keyExists(1));

///         ttt().remove(1);
///         EXPECT_EQ(2, ttt().size());
///         EXPECT_EQ(false, ttt().keyExists(1));

///         ttt().remove(2);
///         EXPECT_EQ(1, ttt().size());
///         EXPECT_EQ(false, ttt().keyExists(2));

///         ttt().remove(0);
///         EXPECT_EQ(0, ttt().size());
///         EXPECT_EQ(false, ttt().keyExists(1));
///     }

///     //  TEST_F(BTreeTest, insert_many_elements_and_remove_one_by_one)
///     //  {
///     //      //std::vector<int> keys {-100, -78, -49, -37, -29, -26, -21, -17, -10, -6, -2, -1, 0, 4, 8, 9, 12, 17, 19, 22, 27, 30, 33, 47, 50, 79, 83, 99};
///     //      //std::vector<int> keys {-100, -78, -49, -37};
///     //      std::vector<int> keys {-100, -78, -49, -37, -29, -26};
///     //      std::random_shuffle(keys.begin(), keys.end());
///     //      for(int key : keys)
///     //      {
///     //          //for(auto key: keys) std::cout << "key:" << key << std::endl;
///     //          ttt().insert(key, "value " + std::to_string(key));
///     //      }
///     //      EXPECT_EQ(keys.size(), ttt().size());

///     //      for(int i=0; i<keys.size(); i)
///     //      {
///     //          auto key = keys[i];
///     //  std::cout << "aaa, key:" << key << std::endl;
///     //          EXPECT_EQ(true, ttt().keyExists(key));
///     //          ttt().remove(key);
///     //          EXPECT_EQ(false, ttt().keyExists(key));
///     //  ++i;
///     //          //EXPECT_EQ(keys.size()-i), ttt().size());
///     //      }
///     //  }

