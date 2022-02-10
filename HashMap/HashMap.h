#ifndef c5fed941e2a9bbc7075a9f3e4ba9fc8b
#define c5fed941e2a9bbc7075a9f3e4ba9fc8b

// Custom durty MFC-like hash map
// g++ -lstdc++ -pthread HashMap.cpp
#include <iostream>
#include <future>
#include <cassert>
#include <chrono>

#include <vector>
#include <thread>
#include <future>
#include <numeric>
#include <iostream>
#include <chrono>
#include <functional>

//  {
//      Value* _values;

//  }

template<class Key, class Value>
class HashMap
{
    size_t _maxSize;
    Key* _keys;
    std::size_t _keysSize;
    Value* _values;
    std::size_t _valuesSize;

    using HashType = std::size_t;

public:
    HashMap(size_t maxSize)
        : _maxSize(maxSize)
        , _keys(nullptr)
        , _keysSize(0)
        , _values(nullptr)
        , _valuesSize(0)
    {
        void* rawKeys = calloc(_maxSize, sizeof(Key*));
        assert(rawKeys);
        _keys = (Key*)(rawKeys);

        void* rawValues = calloc(_maxSize, sizeof(Value*));
        assert(rawValues);
        _values = (Value*)(rawValues);
    }

    ~HashMap()
    {
        if (_keys)
        {
//          for(int i = 0; i < _maxSize; ++i)
//          {
//              // Free every value here explicitly with delete
//              Key* value = _keys[i];
//              Key* value = _keys[i];
//              if(value != nullptr)
//              {
//                  delete value;
//                  _keys[i] = nullptr;
//              }
//          }
            free(_keys);
            _keys = nullptr;
        }
        if (_values)
        {
            free(_values);
            _values = nullptr;
        }
    }

    void reserve(size_t maxSize)
    {
//      if (maxSize > _maxSize)
//      {
//          std::cout << "HashMap::reserve()" << std::endl;
//          std::cout << "        Allocatng, new maxSize" << maxSize << ", old maxSize:" << _maxSize << std::endl;
//          Key* keys = new Key[maxSize];
//          memcpy(keys, _keys, _maxSize*sizeof(Key));
//          delete _keys;
//          _keys = keys;;
//      }
//      else if (maxSize < _maxSize)
//      {
//          std::cout << "        dropping, new maxSize" << maxSize << ", old maxSize:" << _maxSize << std::endl;
//          Key* keys = new Key[maxSize];
//          memcpy(keys, _keys, _maxSize*sizeof(Key));
//          delete _keys;
//          _keys = keys;;
//      }
//      else
//      {
//          // Nothing to do here
//      }
     }

    size_t size() const
    {
        assert(_keysSize == _valuesSize);
        std::cout << "HashMap::size():" << _keysSize << std::endl;
        return _keysSize;
    }

    size_t maxSize() const
    {
        std::cout << "HashMap::maxSize():" <<  _maxSize << std::endl;
        return _maxSize;
    }

    const Value* find(const Key& key) const
    {
        std::size_t i = indexFoo(key);

    }

    bool keyExists(const Key& key) const
    {
        const Value* value = find(key);
        return value != nullptr;
    }

    void insert(const Key& key, const Value& value)
    {
        std::size_t i = indexFoo(key);
//      Key* pK = _keys;
//      Value* pV = _values;
//      assert(pK == nullptr);
//      assert(pV == nullptr);
        Key k = _keys[i];
        Value v = _values[i];
        k = key;
        v = value;
        ++_keysSize;
        ++_valuesSize;
        assert(_keysSize == _valuesSize);
    }

public:
    static HashType hashFoo(const Key& key)
    {
        auto hash = std::hash<Key>{}(key);
        std::cout << "HashMap::hashFoo(), hash:" << hash << std::endl;
        return hash;
    } 

    static std::size_t indexFoo(const Key& key)
    {
        HashType h = hashFoo(key);
        std::size_t index = h;
        std::cout << "HashMap::indexFoo(), index:" << index << std::endl;
        return index;
    } 

    //std::size_t normalize(std::size_t )

};

#if 0
class HumanBeing
{
    std::string _name;
public:
    HumanBeing()
        : _name("undefined")
    {
    }

    HumanBeing(const std::string& name)
        : _name(name)
    {
    }

    HumanBeing(const HumanBeing& other)
    {
        _name = other._name;
    }

    HumanBeing& operator=(const HumanBeing& other)
    {
        if (this != &other)
        {
            _name = other._name;
        }
        return *this;
    }

    const std::string name() const
    {
        return _name;
    }
};

int main()
{
    {
        std::cout << "################## Test 1, bool HashMap::keyExists(const Key& key) const" << std::endl;
        //HashMap<uint8_t, uint32_t> haaash(100);
        HashMap<uint8_t, HumanBeing> haaash(100);
        assert(haaash.maxSize() == 100);

        assert(haaash.size() == 0);
        bool exists = haaash.keyExists(0);
        assert(exists == false);

        haaash.insert(0, HumanBeing("0000000000"));
        assert(haaash.size() == 1);





//      haaash.reserve(1000);
//      assert(haaash.size() == 0);

//      bool exists = haaash.keyExists(0);
//      assert(exists == false);


//      haaash.insert(0, "0000000000");
//      exists = haaash.keyExists(0);
//      assert(exists == true);


//      haaash.insert(1, "1111111111");
//      exists = haaash.keyExists(1);
//      assert(exists == true);


//      haaash.insert(2, "2222222222");
//      exists = haaash.keyExists(2);
//      assert(exists == true);


//      haaash.insert(3, "3333333333");
//      exists = haaash.keyExists(3);
//      assert(exists == true);


//      haaash.insert(9, "9999999999");
//      exists = haaash.keyExists(9);
//      assert(exists == true);


//      haaash.insert(4, "4444444444");
//      exists = haaash.keyExists(4);
//      assert(exists == true);


//      haaash.insert(8, "8888888888");
//      exists = haaash.keyExists(8);
//      assert(exists == true);


//      haaash.insert(5, "5555555555");
//      exists = haaash.keyExists(5);
//      assert(exists == true);
    }

//  {
//      std::cout << "################## Value* find(const Key& key) const " << std::endl;
//      HashMap<uint32_t, std::string> haaash;

//      auto pStr = haaash.find(0);
//      assert(!pStr);

//      haaash.insert(0, "0");
//      pStr = haaash.find(0);
//      assert(pStr);
//      assert(*pStr == "0");

//      haaash.insert(1, "1");
//      pStr = haaash.find(1);
//      assert(pStr);
//      assert(*pStr == "1");

//      pStr = haaash.find(100);
//      assert(!pStr);

//      haaash.insert(100, "100");
//      pStr = haaash.find(100);
//      assert(pStr);
//      assert(*pStr == "100");

//      haaash.insert(20, "20");
//      pStr = haaash.find(20);
//      assert(pStr);
//      assert(*pStr == "20");

//      haaash.insert(17, "17");
//      pStr = haaash.find(17);
//      assert(pStr);
//      assert(*pStr == "17");

//      haaash.insert(48, "48");
//      pStr = haaash.find(48);
//      assert(pStr);
//      assert(*pStr == "48");

//      haaash.insert(56, "56");
//      pStr = haaash.find(56);
//      assert(pStr);
//      assert(*pStr == "56");

//      haaash.insert(45, "45");
//      pStr = haaash.find(45);
//      assert(pStr);
//      assert(*pStr == "45");

//      haaash.insert(38, "38");
//      pStr = haaash.find(38);
//      assert(pStr);
//      assert(*pStr == "38");
//  }

//  {
//      std::cout << "################## Coooooolll" << std::endl;
//      HashMap<uint32_t, std::string> haaash;

//      constexpr int values[] = {86,7, 2, 24, 4, 3, 28, 4, 89, 4, 6, 64, 44, 5, 16, 7, 44, 83, 33};
//      for(int key : values) {
//          haaash.insert(key, "yeee:" + key);
//      }

//      auto pStr = haaash.find(89);
//      assert(pStr);
//      assert(*pStr == "yeee:"+89);

//      pStr = haaash.find(98);
//      assert(!pStr);
//  }

    return 0;
}
#endif // 0

#endif // c5fed941e2a9bbc7075a9f3e4ba9fc8b
