#include <gtest/gtest.h>
#include "BTree.h"
#include <algorithm>

using BTreeIntString = BTree<int, std::string>;
using BTreeIntStringPtr = std::shared_ptr<BTreeIntString>;

class BTreeTest: public ::testing::Test
{ 
public: 
    void SetUp()
    {
        _tree = std::make_shared<BTreeIntString>();
    }

    void TearDown()
    {
        _tree.reset();
    }

    BTreeIntString& ttt()
    {
        return *_tree;
    }

    const BTreeIntString& cttt() const
    {
        return *_tree;
    }

    BTreeIntStringPtr _tree;
};


TEST_F(BTreeTest, empty_tree)
{
    EXPECT_EQ(0, ttt().size());
    EXPECT_EQ(false, ttt().keyExists(0));
    EXPECT_EQ(nullptr, ttt().find(0));
    EXPECT_EQ(nullptr, cttt().find(0));
}


TEST_F(BTreeTest, insert_1_element)
{
    ttt().insert(0, "value 0");
    EXPECT_EQ(1, ttt().size());
    EXPECT_EQ(true, ttt().keyExists(0));

    auto value = ttt().find(0);
    EXPECT_TRUE(value != nullptr);
    EXPECT_EQ("value 0", *value);

    const auto cvalue = cttt().find(0);
    EXPECT_TRUE(cvalue != nullptr);
    EXPECT_EQ("value 0", *cvalue);

    EXPECT_EQ("value 0", ttt().at(0));
    EXPECT_EQ("value 0", cttt().at(0));
}


TEST_F(BTreeTest, insert_the_same_element_for_many_times)
{
    ttt().insert(0, "value 0");
    EXPECT_EQ(1, ttt().size());
    EXPECT_EQ(true, ttt().keyExists(0));
    EXPECT_EQ("value 0", cttt().at(0));

    ttt().insert(0, "value 00");
    EXPECT_EQ(1, ttt().size());
    EXPECT_EQ(true, ttt().keyExists(0));
    EXPECT_EQ("value 00", cttt().at(0));

    ttt().insert(0, "value 000");
    EXPECT_EQ(1, ttt().size());
    EXPECT_EQ(true, ttt().keyExists(0));
    EXPECT_EQ("value 000", cttt().at(0));
}


TEST_F(BTreeTest, insert_many_elements)
{
    std::vector<int> keys {-100, -78, -49, -37, -29, -26, -21, -17, -10, -6, -2, -1, 0, 4, 8, 9, 12, 17, 19, 22, 27, 30, 33, 47, 50, 79, 83, 99};
    std::random_shuffle(keys.begin(), keys.end());
    for(int key : keys)
    {
        //for(auto key: keys) std::cout << "key:" << key << std::endl;
        ttt().insert(key, "value " + std::to_string(key));
    }
    EXPECT_EQ(keys.size(), ttt().size());

    for(int key : keys)
    {
        auto value = ttt().find(key);
        EXPECT_TRUE(value != nullptr);
        auto expected = "value " + std::to_string(key);
        EXPECT_EQ(expected, *value);
        EXPECT_EQ(expected, ttt().at(key));
    }
}


TEST_F(BTreeTest, remove_not_existing_element)
{
    ttt().remove(0);
    EXPECT_EQ(0, ttt().size());
    EXPECT_EQ(false, ttt().keyExists(0));
}

TEST_F(BTreeTest, insert_1_element_and_remove)
{
    ttt().insert(0, "value 0");
    EXPECT_EQ(1, ttt().size());
    EXPECT_EQ(true, ttt().keyExists(0));

    ttt().remove(0);
    EXPECT_EQ(0, ttt().size());
    EXPECT_EQ(false, ttt().keyExists(0));
}


TEST_F(BTreeTest, insert_3_elements_and_remove_from_bottom_to_top)
{
    ttt().insert(0, "value 0");
    EXPECT_EQ(1, ttt().size());
    EXPECT_EQ(true, ttt().keyExists(0));

    ttt().insert(1, "value 1");
    EXPECT_EQ(2, ttt().size());
    EXPECT_EQ(true, ttt().keyExists(1));

    ttt().insert(2, "value 2");
    EXPECT_EQ(3, ttt().size());
    EXPECT_EQ(true, ttt().keyExists(1));

    ttt().remove(2);
    EXPECT_EQ(2, ttt().size());
    EXPECT_EQ(false, ttt().keyExists(2));

    ttt().remove(1);
    EXPECT_EQ(1, ttt().size());
    EXPECT_EQ(false, ttt().keyExists(1));

    ttt().remove(0);
    EXPECT_EQ(0, ttt().size());
    EXPECT_EQ(false, ttt().keyExists(0));
}


TEST_F(BTreeTest, insert_3_elements_and_remove_from_top_to_bottom)
{
    ttt().insert(0, "value 0");
    EXPECT_EQ(1, ttt().size());
    EXPECT_EQ(true, ttt().keyExists(0));

    ttt().insert(1, "value 1");
    EXPECT_EQ(2, ttt().size());
    EXPECT_EQ(true, ttt().keyExists(1));

    ttt().insert(2, "value 2");
    EXPECT_EQ(3, ttt().size());
    EXPECT_EQ(true, ttt().keyExists(2));

    ttt().remove(0);
    EXPECT_EQ(2, ttt().size());
    EXPECT_EQ(false, ttt().keyExists(0));

    ttt().remove(1);
    EXPECT_EQ(1, ttt().size());
    EXPECT_EQ(false, ttt().keyExists(1));

    ttt().remove(2);
    EXPECT_EQ(0, ttt().size());
    EXPECT_EQ(false, ttt().keyExists(2));
}


TEST_F(BTreeTest, insert_3_elements_and_remove_middle_top_bottom)
{
    ttt().insert(0, "value 0");
    EXPECT_EQ(1, ttt().size());
    EXPECT_EQ(true, ttt().keyExists(0));

    ttt().insert(1, "value 1");
    EXPECT_EQ(2, ttt().size());
    EXPECT_EQ(true, ttt().keyExists(1));

    ttt().insert(2, "value 2");
    EXPECT_EQ(3, ttt().size());
    EXPECT_EQ(true, ttt().keyExists(1));

    ttt().remove(1);
    EXPECT_EQ(2, ttt().size());
    EXPECT_EQ(false, ttt().keyExists(1));

    ttt().remove(0);
    EXPECT_EQ(1, ttt().size());
    EXPECT_EQ(false, ttt().keyExists(0));

    ttt().remove(2);
    EXPECT_EQ(0, ttt().size());
    EXPECT_EQ(false, ttt().keyExists(2));
}


TEST_F(BTreeTest, insert_3_elements_and_remove_middle_bottom_top)
{
    ttt().insert(0, "value 0");
    EXPECT_EQ(1, ttt().size());
    EXPECT_EQ(true, ttt().keyExists(0));

    ttt().insert(1, "value 1");
    EXPECT_EQ(2, ttt().size());
    EXPECT_EQ(true, ttt().keyExists(1));

    ttt().insert(2, "value 2");
    EXPECT_EQ(3, ttt().size());
    EXPECT_EQ(true, ttt().keyExists(1));

    ttt().remove(1);
    EXPECT_EQ(2, ttt().size());
    EXPECT_EQ(false, ttt().keyExists(1));

    ttt().remove(2);
    EXPECT_EQ(1, ttt().size());
    EXPECT_EQ(false, ttt().keyExists(2));

    ttt().remove(0);
    EXPECT_EQ(0, ttt().size());
    EXPECT_EQ(false, ttt().keyExists(1));
}


TEST_F(BTreeTest, remove_not_existing_key)
{
    ttt().insert(100, "value 100");
    EXPECT_EQ(1, ttt().size());
    EXPECT_EQ(true, ttt().keyExists(100));

    ttt().remove(101);
    EXPECT_EQ(1, ttt().size());
    EXPECT_EQ(true, ttt().keyExists(100));
    EXPECT_EQ(false, ttt().keyExists(101));
}


TEST_F(BTreeTest, insert_100_remove_100)
{
    ttt().insert(100, "value 100");
    EXPECT_EQ(1, ttt().size());
    EXPECT_EQ(true, ttt().keyExists(100));

    ttt().remove(100);
    EXPECT_EQ(0, ttt().size());
    EXPECT_EQ(false, ttt().keyExists(100));
}


TEST_F(BTreeTest, insert_100_101_remove_100)
{
    ttt().insert(100, "value 100");
    EXPECT_EQ(1, ttt().size());
    EXPECT_EQ(true, ttt().keyExists(100));

    ttt().insert(101, "value 101");
    EXPECT_EQ(2, ttt().size());
    EXPECT_EQ(true, ttt().keyExists(101));

    ttt().remove(100);
    EXPECT_EQ(1, ttt().size());
    EXPECT_EQ(false, ttt().keyExists(100));
    EXPECT_EQ(true, ttt().keyExists(101));
}


TEST_F(BTreeTest, insert_100_101_remove_101)
{
    ttt().insert(100, "value 100");
    EXPECT_EQ(1, ttt().size());
    EXPECT_EQ(true, ttt().keyExists(100));

    ttt().insert(101, "value 101");
    EXPECT_EQ(2, ttt().size());
    EXPECT_EQ(true, ttt().keyExists(101));

    ttt().remove(101);
    EXPECT_EQ(1, ttt().size());
    EXPECT_EQ(true, ttt().keyExists(100));
    EXPECT_EQ(false, ttt().keyExists(101));
}


TEST_F(BTreeTest, insert_99_100_101_remove_99)
{
    ttt().insert(99, "value 99");
    EXPECT_EQ(1, ttt().size());
    EXPECT_EQ(true, ttt().keyExists(99));

    ttt().insert(100, "value 100");
    EXPECT_EQ(2, ttt().size());
    EXPECT_EQ(true, ttt().keyExists(100));

    ttt().insert(101, "value 101");
    EXPECT_EQ(3, ttt().size());
    EXPECT_EQ(true, ttt().keyExists(101));

    ttt().remove(99);

    EXPECT_EQ(2, ttt().size());
    EXPECT_EQ(false, ttt().keyExists(99));
    EXPECT_EQ(true, ttt().keyExists(100));
    EXPECT_EQ(true, ttt().keyExists(101));
}


TEST_F(BTreeTest, insert_99_100_101_remove_100)
{
    ttt().insert(99, "value 99");
    EXPECT_EQ(1, ttt().size());
    EXPECT_EQ(true, ttt().keyExists(99));

    ttt().insert(100, "value 100");
    EXPECT_EQ(2, ttt().size());
    EXPECT_EQ(true, ttt().keyExists(100));

    ttt().insert(101, "value 101");
    EXPECT_EQ(3, ttt().size());
    EXPECT_EQ(true, ttt().keyExists(101));

    ttt().remove(100);

    EXPECT_EQ(2, ttt().size());
    EXPECT_EQ(true, ttt().keyExists(99));
    EXPECT_EQ(false, ttt().keyExists(100));
    EXPECT_EQ(true, ttt().keyExists(101));
}


TEST_F(BTreeTest, insert_99_100_101_remove_101)
{
    ttt().insert(99, "value 99");
    EXPECT_EQ(1, ttt().size());
    EXPECT_EQ(true, ttt().keyExists(99));

    ttt().insert(100, "value 100");
    EXPECT_EQ(2, ttt().size());
    EXPECT_EQ(true, ttt().keyExists(100));

    ttt().insert(101, "value 101");
    EXPECT_EQ(3, ttt().size());
    EXPECT_EQ(true, ttt().keyExists(101));

    ttt().remove(101);

    EXPECT_EQ(2, ttt().size());
    EXPECT_EQ(true, ttt().keyExists(99));
    EXPECT_EQ(true, ttt().keyExists(100));
    EXPECT_EQ(false, ttt().keyExists(101));
}


TEST_F(BTreeTest, insert_10_elements_ascending_order_remove_ascending_order)
{
    std::vector<int> keys {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    for(int key : keys)
    {
        ttt().insert(key, "value " + std::to_string(key));
    }
    EXPECT_EQ(keys.size(), ttt().size());
    EXPECT_EQ(10, ttt().size());

    for(int i=0; i<keys.size(); ++i)
    {
        auto key = keys[i];
        EXPECT_EQ(true, ttt().keyExists(key));
        ttt().remove(key);
        EXPECT_EQ(false, ttt().keyExists(key));
        EXPECT_EQ(keys.size()-(i+1), ttt().size());
    }

    EXPECT_EQ(ttt().size(), 0);
}


TEST_F(BTreeTest, insert_10_elements_ascending_order_remove_descending_order)
{
    std::vector<int> keys {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    for(int key : keys)
    {
        ttt().insert(key, "value " + std::to_string(key));
    }
    EXPECT_EQ(keys.size(), ttt().size());
    EXPECT_EQ(10, ttt().size());

    for(int i=0; i<keys.size(); ++i)
    {
        auto key = keys[keys.size() - i - 1];
        EXPECT_EQ(true, ttt().keyExists(key));
        ttt().remove(key);
        EXPECT_EQ(false, ttt().keyExists(key));
        EXPECT_EQ(keys.size()-(i+1), ttt().size());
    }

    EXPECT_EQ(ttt().size(), 0);
}


TEST_F(BTreeTest, insert_10_elements_descending_order_remove_ascending_order)
{
    std::vector<int> keys {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    for(int key : keys)
    {
        ttt().insert(key, "value " + std::to_string(key));
    }
    EXPECT_EQ(keys.size(), ttt().size());
    EXPECT_EQ(10, ttt().size());

    for(int i=0; i<keys.size(); ++i)
    {
        auto key = keys[keys.size() - i - 1];
        EXPECT_EQ(true, ttt().keyExists(key));
        ttt().remove(key);
        EXPECT_EQ(false, ttt().keyExists(key));
        EXPECT_EQ(keys.size()-(i+1), ttt().size());
    }

    EXPECT_EQ(ttt().size(), 0);
}


TEST_F(BTreeTest, insert_10_elements_descending_order_remove_descending_order)
{
    std::vector<int> keys {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    for(int key : keys)
    {
        ttt().insert(key, "value " + std::to_string(key));
    }
    EXPECT_EQ(keys.size(), ttt().size());
    EXPECT_EQ(10, ttt().size());

    for(int i=0; i<keys.size(); ++i)
    {
        auto key = keys[i];
        EXPECT_EQ(true, ttt().keyExists(key));
        ttt().remove(key);
        EXPECT_EQ(false, ttt().keyExists(key));
        EXPECT_EQ(keys.size()-(i+1), ttt().size());
    }

    EXPECT_EQ(ttt().size(), 0);
}


TEST_F(BTreeTest, insert_many_elements_and_remove_one_by_one)
{
    std::vector<int> keys {-100, -78, -49, -37, -29, -26, -21, -17, -10, -6, -2, -1, 0, 4, 8, 9, 12, 17, 19, 22, 27, 30, 33, 47, 50, 79, 83, 99};

    {
        std::vector<int> tmpKeys(keys);
        std::random_shuffle(tmpKeys.begin(), tmpKeys.end());
        for(auto key : tmpKeys)
        {
            ttt().insert(key, "value " + std::to_string(key));
        }
    }
    EXPECT_EQ(keys.size(), ttt().size());
    EXPECT_EQ(28, ttt().size());

//  std::cout << "Inorder traversal of the tree" << std::endl;
//  ttt().inorder(&ttt());
//  std::cout << std::endl;

    for(int i=0; i<keys.size(); ++i)
    {
        auto key = keys[i];
        EXPECT_EQ(true, ttt().keyExists(key));
//      std::cout << "removing key:" << key << "..." << std::endl;
        ttt().remove(key);

//      std::cout << "Inorder traversal of the tree" << std::endl;
//      ttt().inorder(&ttt());
//      std::cout << std::endl;

        EXPECT_EQ(false, ttt().keyExists(key));
        EXPECT_EQ(keys.size()-(i+1), ttt().size());
    }

    EXPECT_EQ(ttt().size(), 0);
}

