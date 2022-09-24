#include <gtest/gtest.h>

#include <iostream>

//  If very briefly
//      - unique_ptr can be given a deliter
//          - is movable
//          - is not copyble
//      - shared_ptr can be given allocator and deleter
//          - is movable
//          - is copyble
//      - use std::make_shared since it allocates all (ptr itself and accounting stuff) memory at once
//      - use std::make_unique for unique ptrs (starting C++14)

//  std::unique_ptr
//      cool thing that this ptr can be given deleter, i.e. for managing Types with custom deletions

//      Unfortunally make_shared can NOT be given Allocator, only deleter therefore here is no optin to use Custom Allocator only Deleter
//      !!! Here is now way to use make_unique with custom Allocator, only Deleter can be specified



//  std::shared_ptr
//      Like unique_ptr shared_ptr can be used for custom Deletion;
//      Good thing that sgared_ptr can use custom allocator as well, see std::allocate_shared

//      https://habr.com/ru/post/304308/
//      http://www.cplusplus.com/reference/memory/allocate_shared/
//      https://github.com/pirog-spb/allocator_example/blob/master/main.cpp

//      Custom allocation and deletion ould be done like this:

//      std::allocator<int> allocator;
//      std::shared_ptr<int> foo = std::allocate_shared<int> (allocator, 100);

//      std::weak_ptr
//      std::make_shared

//      Блять, совсем недавно нафакапил с этим вопросом:
//      20.08.2021 TI feedback EMPLOYEE - ????? ???????; ?????? Rescue Mission CM_Senior C++ SW Developer_
//      20.08.2021 TI feedback EMPLOYEE - ????? ???????; ???????_SWH_Senior C++ Developer




struct Resource
{
    Resource(const std::string& name)
        : name_(name)
    {
//      std::cout << "Resource():" << name_ << std::endl;
    }

    ~Resource()
    {
//      std::cout << "~Resource():" << name_ << std::endl;
    }

    std::string name_;
};

TEST(SmartPointers, make_unique)
{
    // Create r1 using make_unique, no custom deleter
    std::unique_ptr<Resource> r2 = std::make_unique<Resource>("r1");
}





Resource* alloc_resource(const std::string name)
{
//  std::cout << "alloc_resource():" << name << std::endl;
    return new Resource(name);
}

void free_resource(Resource* resource)
{
    if(resource) {
//      std::cout << "free_resource():" << resource->name_ << std::endl;
        delete resource;
        resource = nullptr;
    }
}

struct ResourceDeleter
{
    void operator()(Resource* r)
    {
        if(r)free_resource(r);
    }
};

TEST(SmartPointers, unique_ptr_with_custom_deleter)
{
    std::unique_ptr<Resource, ResourceDeleter> r(alloc_resource("r2"));
    ASSERT_EQ("r2", r->name_);
}

TEST(SmartPointers, unique_ptr_with_custom_deleter_as_lambda)
{
    auto rclose = [](Resource* r){free_resource(r);};
    std::unique_ptr<Resource, decltype(rclose)> r{alloc_resource("r3"), rclose};
    ASSERT_EQ("r3", r->name_);
}




template <class T>
struct CustomAllocator {
    typedef T value_type;

    CustomAllocator() noexcept {}

    template <class U> CustomAllocator (const CustomAllocator<U>&) noexcept
    {
    }

    T* allocate (std::size_t n)
    {
        return reinterpret_cast<T*>( ::operator new(n*sizeof(T)));
    }
 
    void deallocate (T* p, std::size_t n)
    {
        ::operator delete(p);
    }
};




TEST(SmartPointers, allocate_shared_custom_allocator)
{
    CustomAllocator<Resource> allocator;
    auto r = std::allocate_shared<Resource>(allocator, "r4");
    ASSERT_EQ("r4", r->name_);
}

