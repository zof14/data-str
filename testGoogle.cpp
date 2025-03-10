
#include <gtest/gtest.h>
#include "bi_ring.h"
#include "br_usage.h"
#include <iostream>

#include <stdexcept>
TEST(BiRingTest, PushFrontAddsElement) {
    bi_ring<int, std::string> ring;
    auto it = ring.push_front(1, "one");
    ASSERT_EQ(it.key(), 1);
    ASSERT_EQ(it.info(), "one");
    it = ring.push_front(2, "two");
    ASSERT_EQ(it.key(), 2);
    ASSERT_EQ(it.info(), "two");

    it = ring.push_front(3, "three");
    ASSERT_EQ(it.key(), 3);
    ASSERT_EQ(it.info(), "three");
    ++it;  
    ASSERT_EQ(it.key(), 2);
    ASSERT_EQ(it.info(), "two");
    ++it; 
    ASSERT_EQ(it.key(), 1);
    ASSERT_EQ(it.info(), "one");
    ++it;  
    ASSERT_EQ(it.key(), 3);
    ASSERT_EQ(it.info(), "three");
};

TEST(BiRingTest, PopFront) {
    bi_ring<int, std::string> bi_ring;
    auto it = bi_ring.push_front(1, "one");
    bi_ring.push_front(2, "two");
    bi_ring.push_front(3, "three");
    it = bi_ring.pop_front();
    ASSERT_EQ(it.key(), 2);
    ASSERT_EQ(it.info(), "two");


    it = bi_ring.pop_front();
    ASSERT_EQ(it.key(), 1);
    ASSERT_EQ(it.info(), "one");
    it = bi_ring.pop_front();
    ASSERT_TRUE(bi_ring.empty());

    EXPECT_THROW(bi_ring.pop_front(), std::runtime_error);
};
TEST(BiRingTest, PopFrontEmpty){
    bi_ring<int, std::string> bi_ring;
    EXPECT_THROW(bi_ring.pop_front(), std::runtime_error);
};
TEST(BiRingTest, Insert) {

    bi_ring<int, std::string> bi_ring;
    bi_ring.push_front(1, "one");
    bi_ring.push_front(2, "two");
    auto it = bi_ring.push_front(3, "three");
    auto after_ins = bi_ring.insert(it, 5, "five");
    --it;
    ++after_ins;
    ASSERT_EQ(it.key(),5);
    ASSERT_EQ(after_ins.key(),3);
    ASSERT_EQ(bi_ring.get_size(),4);
}
TEST(BiRingTest, FindKey) {
    bi_ring<int, std::string> ring;
    ring.push_back(1, "one");
    ring.push_back(2, "two");
    ring.push_back(3, "three");

    int key = 2;
    auto it = ring.find(key);
    ASSERT_NE(it, nullptr);
    ASSERT_EQ(it.key(), 2);
    ASSERT_EQ(it.info(), "two");
}

TEST(BiRingTest, FindKeyNE) {
    bi_ring<int, std::string> ring;
    ring.push_back(1, "one");
    ring.push_back(2, "two");

    int key = 3;
    auto it = ring.find(key);
    ASSERT_EQ(it, nullptr);
}

TEST(BiRingTest, Clear) {
    bi_ring<int, std::string> ring;
    ring.push_back(1, "one");
    ring.push_back(2, "two");
    ring.clear();
    ASSERT_TRUE(ring.empty());
    ASSERT_EQ(ring.get_size(), 0);
}
TEST(BiRingTest, Join){
    bi_ring<int, std::string> ring;
    ring.push_back(1, "one");
    ring.push_back(2, "two");
    bi_ring<int, std::string> ring2;
    ring.push_back(3, "three");
    ring.push_back(2, "two");
    bi_ring<int, std::string> res;
    res= join(ring,ring2);
    ASSERT_EQ(res.get_size(),3);
    res.print();


}


TEST(BiRingTest, PushBack1) {
    bi_ring<int, std::string> ring;
    ring.push_back(1, "one");
    ASSERT_EQ(ring.get_size(), 1);
    typename bi_ring<int, std::string>::iterator it = ring.begin();
    ASSERT_EQ(it.key(), 1);
    ASSERT_EQ(it.info(), "one");
    ++it;
    ASSERT_EQ(it.key(), 1); 
    ASSERT_EQ(it.info(), "one");
    --it;
    ASSERT_EQ(it.key(), 1);
    ASSERT_EQ(it.info(), "one");
}

TEST(BiRingTest, PushBack2) {
    bi_ring<int, std::string> ring;
    ring.push_back(1, "one");
    ring.push_back(2, "two");
    ASSERT_EQ(ring.get_size(), 2);
    typename bi_ring<int, std::string>::iterator it = ring.begin();
    ASSERT_EQ(it.key(), 1);
    ASSERT_EQ(it.info(), "one");
    ++it;
    ASSERT_EQ(it.key(), 2);
    ASSERT_EQ(it.info(), "two");
    ++it;
    ASSERT_EQ(it.key(), 1);
    --it;
    ASSERT_EQ(it.key(), 2);
    ASSERT_EQ(it.info(), "two");
    --it;
    ASSERT_EQ(it.key(), 1);
    ASSERT_EQ(it.info(), "one");
}
bool filter_predicate(const int& key) {
    return key % 2 == 0;
};


TEST(BiRingFuncTest, Filter) {
    bi_ring<int, std::string> ring;
    ring.push_back(1, "one");
    ring.push_back(2, "two");
    ring.push_back(3, "three");
    ring.push_back(4, "four");

    auto filtered = filter(ring, filter_predicate);
    typename bi_ring<int, std::string>::const_iterator it = filtered.cbegin();
    
    ASSERT_EQ(it.key(), 2);
    ASSERT_EQ(it.info(), "two");
    ++it;  
    ASSERT_EQ(it.key(), 4); 
    ASSERT_EQ(it.info(), "four");
};

TEST(BiRingFuncTest, FilterOne) {
    bi_ring<int, std::string> ring;
    ring.push_back(1, "one");
    auto filtered = filter(ring, filter_predicate);
    typename bi_ring<int, std::string>::const_iterator it = filtered.cbegin();
    ASSERT_EQ(filtered.get_size(), 0);
}

TEST(BiRingFuncTest, FilterEmpty){
    bi_ring<int, std::string> ring;
    auto filtered = filter(ring, filter_predicate);
    ASSERT_TRUE(filtered.empty());

}

TEST(BiRingFuncTest, Shuffle) {
    bi_ring<int, std::string> ring;
    ring.push_back(1, "one");
    ring.push_back(2, "two");
    ring.push_back(3, "three");
    ring.push_back(4, "four");

    bi_ring<int, std::string> second_ring;
    second_ring.push_back(3, "three");
    second_ring.push_back(4, "four");

    auto result = shuffle(ring, 2, second_ring, 2, 2);

    ASSERT_EQ(result.get_size(), 8);

    typename bi_ring<int, std::string>::const_iterator it = result.cbegin();
    ASSERT_EQ(it.key(), 1);  
}

TEST(BiRingFuncTest, ShuffleOneEmpty) {
    bi_ring<int, std::string> ring;
    ring.push_back(1, "one");
    ring.push_back(2, "two");
    ring.push_back(3, "three");
    ring.push_back(4, "four");

    bi_ring<int, std::string> second_ring;
    bi_ring<int, std::string> result = shuffle(ring, 2, second_ring, 2, 3);
    
    ASSERT_EQ(result.get_size(), 6); 
}

TEST(BiRingFuncTest,ShuffleTwoEmpty){
    bi_ring<int, std::string> ring;
    
    bi_ring<int, std::string> second_ring;
    bi_ring<int, std::string> result= shuffle(ring,2,second_ring,2,2);
    ASSERT_EQ(result.get_size(),0);
}



int aggregate(const int& key, const int& info1, const int& info2) {
    return info1 + info2;  
};

TEST(BiRingFuncTest, UniqueEmptyList) {
    bi_ring<int, int> ring;
    auto result = unique(ring, aggregate);
    ASSERT_EQ(result.get_size(), 0);
};
TEST(BiRingFuncTest, UniqueSingle) {
    bi_ring<int, int> ring;
    ring.push_front(1, 10);
    auto result = unique(ring, aggregate);
    ASSERT_EQ(result.get_size(), 1);

    typename bi_ring<int,int>::const_iterator it = result.cbegin();
    ASSERT_EQ(it.key(), 1);
    ASSERT_EQ(it.info(), 10);
};
