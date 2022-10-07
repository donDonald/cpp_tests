#pragma once

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
    BTree* _left;
    BTree* _right;

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
        return result == nullptr ? nullptr : &result->_value;
    }

    Value* find(const Key& key)
    {
        auto result = _find(key);
        return result == nullptr ? nullptr : &result->_value;
    }

    const Value& at(const Key& key) const
    {
        auto result = _find(key);
        assert(result);
        return result->_value;
    }

    Value& at(const Key& key)
    {
        auto result = _find(key);
        assert(result);
        return result->_value;
    }

    void insert(const Key& key, const Value& value)
    {
        auto result = _find(key);
        if (result) {
            result->_key = key;
            result->_value = value;
            result->_isSet = true;
        } else {
            BTree node(key, value);
            _insert(node);
        }
    }

    void remove(const Key& key)
    {
        _remove(this, key);
    }

	void inorder(BTree* node)
	{
	    if (node!= nullptr) {
	        inorder(node->_left);
	        std::cout << node->_key <<" ";
	        inorder(node->_right);
	    }
	}
private:

    const BTree* _find(const Key& key) const
    {
        if (_isSet && _key == key) {
            return this;
        } else if (key < _key && _left) {
            return _left->_find(key);
        } else if (key > _key && _right){
            return _right->_find(key);
        }
        return nullptr;
    }

    BTree* _find(const Key& key)
    {
        static BTree* _parent = nullptr;
        if (_isSet && _key == key) {
            _parent = nullptr;
            return this;
        } else if (key < _key && _left) {
            _parent = this;
            return _left->_find(key);
        } else if (key > _key && _right){
            _parent = this;
            return _right->_find(key);
        }
        _parent = nullptr;
        return nullptr;
    }

    void _insert(BTree& node)
    {
        if (_isSet) {
            if(_key == node._key) {
                _key = node._key;
                _value = node._value;
            } else if (node._key < _key) {
                if (!_left) _left = new BTree;
                _left->_insert(node);
            } else {
                if (!_right) _right = new BTree;
                _right->_insert(node);
            }
        } else {
            _key = node._key;
            _value = node._value;
            _isSet = true;
        }
    }

    BTree* _remove(BTree* node, const Key& key)
    {
        if (node == nullptr)
            return node;

        if(key < node->_key)
        {
            BTree* tmp = _remove(node->_left, key);
            node->_left = tmp;
        }
        else if(key > node->_key)
        {
            BTree* tmp = _remove(node->_right, key);
            node->_right = tmp;
        }
        else
        {
            // It's a leaf node
            if(node->_left == nullptr && node->_right == nullptr)
            {
                node->_isSet = false;
                node->_key = Key();
                node->_value = Value();
                return nullptr;
            }
            else if(node->_left == nullptr)
            {
                BTree* tmp = node->_right;
                node->_isSet = tmp->_isSet;
                node->_left = tmp->_left;
                node->_right = tmp->_right;
                node->_key = tmp->_key;
                node->_value = tmp->_value;
                delete tmp;
                return node;
            }
            else if(node->_right == nullptr)
            {
                BTree* tmp = node->_left;
                node->_isSet = tmp->_isSet;
                node->_left = tmp->_left;
                node->_right = tmp->_right;
                node->_key = tmp->_key;
                node->_value = tmp->_value;
                delete tmp;
                return node;
            }

            // node with two children: Get the inorder successor
            // (smallest in the right subtree)
            BTree* tmp = minValueNode(node->_right);
            // Copy the inorder successor's content to this node
            node->_key = tmp->_key;

            // Delete the inorder successor
            node->_right = _remove(node->_right, tmp->_key);
        }
        return node;
    }

    BTree* minValueNode(BTree* node)
    {
        BTree* current = node;

        /* loop down to find the leftmost leaf */
        while (current && current->_left != nullptr)
            current = current->_left;

        return current;
    }

};

