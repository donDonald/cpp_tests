#pragma once

// Custom durty MFC-like hash map
#include <unordered_map>

template<class Key, class Value, class HashFoo=std::hash<Key>>
class HashMap
{
public:
    class Bucket
    {
    public:
        struct Record
        {
            Key _key;
            Value _value;
        };

        static constexpr int BUCKET_SIZE = 16;
        size_t _maxSize;
        size_t _size;
        Record* _records;

    public:
        Bucket(size_t maxSize = BUCKET_SIZE)
            : _maxSize(0)
            , _size(0)
            , _records(nullptr)
        {
            reserve(maxSize);
        }

        ~Bucket()
        {
            delete [] _records;
            _records = nullptr;
        }

        size_t size() const
        {
            return _size;
        }

        void reserve(size_t maxSize)
        {
            assert(maxSize > _maxSize);
            Record* records = new Record[maxSize];
            assert(records);
            for(size_t i=0; i<_size; i++)
            {
                records[i] = _records[i];
            }
            _maxSize = maxSize;
            delete [] _records;
            _records = records;
        }

        Value* find(const Key& key)
        {
            for(size_t i=0; i<_size; ++i)
            {
                if(_records[i]._key == key)
                {
                    return &_records[i]._value;
                }
            }
            return nullptr;
        }

        const Value* find(const Key& key) const
        {
            for(size_t i=0; i<_size; ++i)
            {
                if(_records[i]._key == key)
                {
                    return &_records[i]._value;
                }
            }
            return nullptr;
        }

        Value& operator [](const Key& key)
        {
            Value* value = find(key);
            assert(value);
            return *value;
        }

        const Value& operator [](const Key& key) const
        {
            const Value* value = find(key);
            assert(value);
            return *value;
        }

        bool exists(const Key& key) const
        {
            const Value* value = find(key);
            return value != nullptr;
        }

        Value* insert(const Key& key, const Value& value)
        {
            Value* newValue = nullptr;
            for(size_t i=0; i<_size; ++i)
            {
                if(_records[i]._key == key)
                {
                    _records[i]._value = value;
                    newValue = &_records[i]._value;
                }
            }

            if(!newValue)
            {
                if(_size == _maxSize)
                {
                    reserve(_maxSize + BUCKET_SIZE);
                }
                _records[_size]._key = key;
                _records[_size]._value = value;
                newValue = &_records[_size]._value;
                ++_size;
            }
            return newValue;
        }

        bool remove(const Key& key)
        {
            for(size_t i=0; i<_size; ++i)
            {
                if(_records[i]._key == key)
                {
                    Key k = _records[i]._key;
                    Value v = _records[i]._value;
                    k.~Key();
                    v.~Value();
                    if(i<(_size-1))
                    {
                        // move tail-most pair to just removed pair
                        _records[i]._key = _records[_size-1]._key;
                        _records[i]._value = _records[_size-1]._value;
                        _records[_size-1]._key.~Key();
                        _records[_size-1]._value.~Value();
                    }
                    --_size;
                    return true;
                }
            }
            return false;
        }

    };

    using HashType = std::size_t;
    size_t   _maxSize;
    size_t   _size;
    Bucket** _buckets;

public:
    HashMap(size_t maxSize = 16)
        : _maxSize(0)
        , _size(0)
        , _buckets(nullptr)
    {
        reserve(maxSize);
    }

    ~HashMap()
    {
        for(size_t i=0; i<_maxSize; i++)
        {
            Bucket* b = _buckets[i];
            if(b)
            {
                b->~Bucket();
                delete b;
                _buckets[i] = nullptr;
            }
        }
        free(_buckets);
        _buckets = nullptr;
    }

    size_t size() const
    {
        size_t s = 0;
        for(size_t i=0; i<_maxSize; i++)
        {
            Bucket* b = _buckets[i];
            if(b)
            {
                s += b->size();
            }
        }
        return s;
    }

    void reserve(size_t maxSize)
    {
        assert(maxSize > _maxSize);
        Bucket** buckets = (Bucket**)std::realloc(_buckets, maxSize*sizeof(Bucket*));
        assert(buckets);
        if(_size == 0)
        {
            memset(buckets, 0, maxSize*sizeof(Bucket*));
        }
        else
        {
            memset(_buckets + _size*sizeof(void*), 0, maxSize - _size);
        }
        _maxSize = maxSize;
        _buckets = buckets;
    }

    Value* find(const Key& key)
    {
        size_t index = indexFoo(key);
//        std::cout << "find(), key:" << (int)key << ", index:" << index << std::endl;
        Bucket* b = _buckets[index];
        Value* v = nullptr;
        if(b)
        {
            v = b->find(key);
        }
        return v;
    }

    const Value* find(const Key& key) const
    {
        size_t index = indexFoo(key);
//        std::cout << "find(), key:" << (int)key << ", index:" << index << std::endl;
        Bucket* b = _buckets[index];
        Value* v = nullptr;
        if(b)
        {
            v = b->find(key);
        }
        return v;
    }

    Value& operator [](const Key& key)
    {
        Value* value = find(key);
        assert(value);
        return *value;
    }

    const Value& operator [](const Key& key) const
    {
        const Value* value = find(key);
        assert(value);
        return *value;
    }

    bool exists(const Key& key) const
    {
        const Value* value = find(key);
        return value != nullptr;
    }

    Value* insert(const Key& key, const Value& value)
    {
        size_t index = indexFoo(key);
//        std::cout << "insert(), key:" << (int)key << ", index:" << index << std::endl;
        Bucket* b = _buckets[index];
        Value* v = nullptr;
        if(!b)
        {
            b = new Bucket();
            _buckets[index] = b;
        }
        v = b->insert(key, value);
        return v;
    }

    bool remove(const Key& key)
    {
        size_t index = indexFoo(key);
//        std::cout << "remove(), key:" << (int)key << ", index:" << index << std::endl;
        Bucket* b = _buckets[index];
        bool r = false;
        if(b)
        {
            r = b->remove(key);
        }
        return r;
    }

    std::size_t indexFoo(const Key& key) const
    {
        return HashFoo{}(key) % _maxSize;
    } 
};
