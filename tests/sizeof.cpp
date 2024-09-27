#include <gtest/gtest.h>
#include <iostream>

// Define size of machine word here
#define MACHINE_WORD (sizeof(size_t))




TEST(sizeof, sizeof_void)
{
    auto size = sizeof(void);
    EXPECT_EQ(1, size);
}




TEST(sizeof, sizeof_void_ptr)
{
    void* ptr = nullptr;
    auto size = sizeof(ptr);
    EXPECT_EQ(MACHINE_WORD, size);
}




TEST(sizeof, sizeof_bool)
{
    bool val;
    auto size = sizeof(val);
    EXPECT_EQ(1, size);
}




TEST(sizeof, sizeof_bool_ptr)
{
    bool* ptr = nullptr;
    auto size = sizeof(ptr);
    EXPECT_EQ(MACHINE_WORD, size);
}




TEST(sizeof, sizeof_uint8_t)
{
    uint8_t val;
    auto size = sizeof(val);
    EXPECT_EQ(1, size);
}




TEST(sizeof, sizeof_int)
{
    int val;
    auto size = sizeof(val);
    EXPECT_EQ(4, size);
}




TEST(sizeof, sizeof_int64_6)
{
    int64_t val;
    auto size = sizeof(val);
    EXPECT_EQ(8, size);
}




struct EmptyStruct{};
TEST(sizeof, sizeof_EmptyStruct)
{
    EmptyStruct val;
    auto size = sizeof(val);
    EXPECT_EQ(1, size);
}




struct AlignedStruct {
    int a;
    char b;
    int c;
};
TEST(sizeof, sizeof_AlignedStruct)
{
    AlignedStruct val;
    auto size = sizeof(val);
    EXPECT_EQ(4+4+4, size);
    EXPECT_EQ(sizeof(AlignedStruct::a) + sizeof(AlignedStruct::b) + sizeof(AlignedStruct::c) + 3, size);
}




struct NotAlignedStruct {
    char b;
    int a;
    char c;
};
TEST(sizeof, sizeof_NotAlignedStruct)
{
    NotAlignedStruct val;
    auto size = sizeof(val);
    EXPECT_EQ(sizeof(NotAlignedStruct::a) + sizeof(NotAlignedStruct::b) + sizeof(NotAlignedStruct::c) + 3 + 3, size);
    EXPECT_EQ(4+4+4, size);
}




struct StructWithStaticMember {
    int a;
    static int b;
};
TEST(sizeof, sizeof_StructWithStaticMember)
{
    StructWithStaticMember val;
    auto size = sizeof(val);
    EXPECT_EQ(sizeof(StructWithStaticMember::a), size);
    EXPECT_EQ(4, size);
}




#pragma pack(push, 1)
struct AlignedAndPackedStruct {
    int a;
    char b;
    int c;
};
#pragma pack(pop)
TEST(sizeof, sizeof_AlignedAndPackedStruct)
{
    AlignedAndPackedStruct val;
    auto size = sizeof(val);
    EXPECT_EQ(sizeof(AlignedAndPackedStruct::a) + sizeof(AlignedAndPackedStruct::b) + sizeof(AlignedAndPackedStruct::c), size);
    EXPECT_EQ(4+1+4, size);
}




#pragma pack(push, 1)
struct NotAlignedAndPackedStruct {
    char b;
    int a;
    char c;
};
#pragma pack(pop)
TEST(sizeof, sizeof_NotAlignedAndPackedStruct)
{
    NotAlignedAndPackedStruct val;
    auto size = sizeof(val);
    EXPECT_EQ(sizeof(NotAlignedAndPackedStruct::a) + sizeof(NotAlignedAndPackedStruct::b) + sizeof(NotAlignedAndPackedStruct::c), size);
    EXPECT_EQ(1+4+1, size);
}




struct StructureWithDtor
{
    ~StructureWithDtor() {}
    int32_t a;
};
TEST(sizeof, sizeof_StructureWithDtor)
{
    StructureWithDtor val;
    auto size = sizeof(val);
    // Payload only
    EXPECT_EQ(sizeof(StructureWithDtor::a), size);
    EXPECT_EQ(4, size);
}




struct StructureWithVirtualDtor
{
    virtual ~StructureWithVirtualDtor() {}
    int32_t a;
};
TEST(sizeof, sizeof_StructureWithVirtualDtor)
{
    StructureWithVirtualDtor val;
    auto size = sizeof(val);
    // Vtable ptr + padding + payload
    EXPECT_EQ(sizeof(void*) + sizeof(StructureWithVirtualDtor::a) + 4, size);
    EXPECT_EQ(sizeof(void*) + 4 + 4, size);
}




#pragma pack(push, 1)
struct StructureWithVirtualDtorPacked
{
    virtual ~StructureWithVirtualDtorPacked() {}
    int32_t a;
};
TEST(sizeof, sizeof_StructureWithVirtualDtorPacked)
{
    StructureWithVirtualDtorPacked val;
    auto size = sizeof(val);
    // Vtable ptr + payload, no padding
    EXPECT_EQ(sizeof(void*) + sizeof(StructureWithVirtualDtorPacked::a), size);
    EXPECT_EQ(sizeof(void*) + 4, size);
}
#pragma pack(pop)




TEST(sizeof, sizeof_unique_ptr)
{
    // No control block, only pointer to payload
    std::unique_ptr<std::string> ptr;
    EXPECT_EQ(sizeof(void*), sizeof(ptr));
}




TEST(sizeof, sizeof_shared_ptr)
{
    // Control block + pointer to payload
    std::shared_ptr<std::string> ptr;
    EXPECT_EQ(2*sizeof(void*), sizeof(ptr));
}




TEST(sizeof, sizeof_weak_ptr)
{
    // Control block + pointer to payload
    std::weak_ptr<std::string> ptr;
    EXPECT_EQ(2*sizeof(void*), sizeof(ptr));
}
