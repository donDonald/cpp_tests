#include <memory>
#include <cassert>
#include <iostream>

// https://www.vertabelo.com/blog/all-about-indexes-part-2-mysql-index-structure-and-performance/

template<class Key, class Value>
class BTree
{
    bool _isSet; 
    Key _key;
    Value _value;
    std::unique_ptr<BTree> _left, _right;
public:

    BTree()
        : _isSet(false)
        , _key()
        , _value()
        , _left(nullptr)
        , _right(nullptr)
        {}

    BTree(const Key& key, const Value& value)
        : _isSet(true)
        , _key(key)
        , _value(value)
        , _left(nullptr)
        , _right(nullptr)
        {}

    /**
     * @brief count elements. Fucking slow, complexity is O(N)
     */
    size_t size() const
    {
        size_t result = 0;
        if(_isSet) ++result;
        if(_left) result += _left->size();
        if(_right) result += _right->size();
        return result;
    }

    bool keyExists(const Key& key) const
    {
        const Value* value = find(key);
        return value != nullptr;
    }

    const Value* find(const Key& key) const
    {
        auto result = _find(key);
        return result.first == nullptr ? nullptr : &result.first->_value;
    }

    Value* find(const Key& key)
    {
        auto result = _find(key);
        return result.first == nullptr ? nullptr : &result.first->_value;
    }

    const Value& at(const Key& key) const
    {
        auto result = _find(key);
        assert(result.first);
        return result.first->_value;
    }

    Value& at(const Key& key)
    {
        auto result = _find(key);
        assert(result.first);
        return result.first->_value;
    }

    void insert(const Key& key, const Value& value)
    {
        auto result = _find(key);
        if (result.first) {
            result.first->_key = key;
            result.first->_value = value;
            result.first->_isSet = true;
        } else {
            BTree node(key, value);
            _insert(node);
        }
    }

    void remove(const Key& key)
    {
//std::cout << "0_remove, key:" << key << std::endl;
        auto result = _find(key);
        auto node = result.first;
        auto parent = result.second;
//std::cout << "1_remove, key:" << key << std::endl;
        if (node) {
//std::cout << "2_remove, key:" << key << std::endl;
            if (parent) {
//std::cout << "3_remove, key:" << key << std::endl;
                auto left = std::move(node->_left);
                auto right = std::move(node->_right);

//std::cout << "4_remove, key:" << key << ", parent->key:" << parent->_key << std::endl;
                // Find myself, node to be removed
                if(key < parent->_key) {
                    // I'm on left side
//std::cout << "5_left_remove, key:" << key << std::endl;
                    parent->_left.reset();
                } else {
                    // I'm on right side
//std::cout << "5_right_remove, key:" << key << std::endl;
                    parent->_right.reset();
                }

                // Insert left and right of the TBR node into it's parent 
                if(left) parent->_insert(*left);
                if(right) parent->_insert(*right);
            } else {
                _isSet = false;
            }
        }
    }

private:

    mutable const BTree* _cparent;
    std::pair<const BTree*, const BTree*> _find(const Key& key) const
    {
//std::cout << "_find() const" << std::endl;
        if (_isSet && _key == key) {
            std::pair<const BTree*, const BTree*> result {this, _cparent};
            _cparent = nullptr;
            return result;
        } else if (key < _key && _left) {
            _cparent = this;
            return _left->_find(key);
        } else if (key > _key && _right){
            _cparent = this;
            return _right->_find(key);
        }
        _cparent = nullptr;
        return std::make_pair<const BTree*, const BTree*>(nullptr, nullptr);
    }

    std::pair<BTree*, BTree*> _find(const Key& key)
    {
        static BTree* _parent = nullptr;
//std::cout << "_0find(), parent:" << _parent << std::endl;
        if (_isSet && _key == key) {
//std::cout << "_1find(), parent:" << _parent << std::endl;
            std::pair<BTree*, BTree*> result {this, _parent};
            _parent = nullptr;
            return result;
        } else if (key < _key && _left) {
            _parent = this;
//std::cout << "_2find(), parent:" << _parent << std::endl;
            return _left->_find(key);
        } else if (key > _key && _right){
            _parent = this;
//std::cout << "_3find(), parent:" << _parent << std::endl;
            return _right->_find(key);
        }
        _parent = nullptr;
//std::cout << "_4find(), parent:" << _parent << std::endl;
        return std::make_pair<BTree*, BTree*>(nullptr, nullptr);
    }

    void _insert(BTree& node)
    {
//std::cout << "_insert()" << std::endl;
        if (_isSet) {
            if(_key == node._key) {
                _key = node._key;
                _value = node._value;
            } else if (node._key < _key) {
                if (!_left) _left = std::make_unique<BTree>();
                _left->_insert(node);
            } else {
                if (!_right) _right = std::make_unique<BTree>();
                _right->_insert(node);
            }
        } else {
            _key = node._key;
            _value = node._value;
            _isSet = true;
        }
    }

};

