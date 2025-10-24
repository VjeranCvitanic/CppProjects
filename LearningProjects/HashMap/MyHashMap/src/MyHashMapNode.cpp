#include "../include/MyHashMapNode.h"

// Class functions
template <typename Key, typename Value>
MyHashMapNode<Key, Value>::MyHashMapNode()
    : key{}, value{}, hash_value(0), next(nullptr)
{
    Logger::Log(__FILE__, __func__, TRACE, "Value Constructor");
}
template <typename Key, typename Value>
MyHashMapNode<Key, Value>::MyHashMapNode(Key key_, Value val_, size_t hash_val_)
    : key(key_), value(val_), hash_value(hash_val_), next(nullptr)
{
    Logger::Log(__FILE__, __func__, TRACE, "Value constructor 2");
}
template <typename Key, typename Value>
MyHashMapNode<Key, Value>::MyHashMapNode(Key key_, Value val_, size_t hash_val_, MyHashMapNode<Key, Value>* next_)
    : key(key_), value(val_), hash_value(hash_val_), next(next_)
{
    Logger::Log(__FILE__, __func__, TRACE, "Value constructor 3");
}
template <typename Key, typename Value>
MyHashMapNode<Key, Value>::MyHashMapNode(const MyHashMapNode<Key, Value>& other)
    : key(other.key), value(other.value), hash_value(other.hash_value)
{
    Logger::Log(__FILE__, __func__, TRACE, "Copy constructor");
    next = other.next ? new MyHashMapNode<Key, Value>(*other.next) : nullptr; // TODO: recursion problem -> init by value?
}
template <typename Key, typename Value>
MyHashMapNode<Key, Value>::MyHashMapNode(MyHashMapNode<Key, Value>&& other)
    : key(std::move(other.key)), value(std::move(other.value)), hash_value(other.hash_value), next(other.next)
{
    Logger::Log(__FILE__, __func__, TRACE, "Move constructor");
    other.next = nullptr;
    other.key = {};
    other.value = {};
    other.hash_value = 0;
}
template <typename Key, typename Value>
MyHashMapNode<Key, Value>& MyHashMapNode<Key, Value>::operator=(const MyHashMapNode<Key, Value>& other)
{
    Logger::Log(__FILE__, __func__, TRACE, "Copy assignment operator");
    if(this == &other)
        return *this;
    key = other.key;
    value = other.value;
    hash_value = other.hash_value;
    next = other.next ? new MyHashMapNode<Key, Value>(*other.next) : nullptr; // TODO: recursion problem -> init by value?

    return *this;
}
template <typename Key, typename Value>
MyHashMapNode<Key, Value>& MyHashMapNode<Key, Value>::operator=(MyHashMapNode<Key, Value>&& other)
{
    Logger::Log(__FILE__, __func__, TRACE, "Move assignment operator");
    if(this == &other)
        return *this;
    key = std::move(other.key);
    value = std::move(other.value);
    hash_value = other.hash_value;
    delete next;
    next = other.next;

    other.next = nullptr;
    other.key = {};
    other.value = {};
    other.hash_value = 0;

    return *this;
}

template <typename Key, typename Value>
MyHashMapNode<Key, Value>::~MyHashMapNode()
{
    Logger::Log(__FILE__, __func__, TRACE, "Destructor");
}

template <typename Key, typename Value>
MyHashMapNode<Key, Value>* MyHashMapNode<Key, Value>::getNext() const
{
    return next;
}

template <typename Key, typename Value>
void MyHashMapNode<Key, Value>::insert(const Key& key, const Value& val, size_t hash_val)
{
    MyHashMapNode<Key, Value>* prev = find_prev_or_last(key, hash_val);

    prev->next = new MyHashMapNode<Key, Value>(key, val, hash_val, prev->next);
}

template <typename Key, typename Value>
Value* MyHashMapNode<Key, Value>::get(const Key& key, size_t hash_val)
{
    MyHashMapNode<Key, Value>* target = find(key, hash_val);

    if(target && target->hash_value == hash_val)
    {
        if(target->key == key)
            return &target->value;
        return nullptr;
    }
    else 
        return nullptr;
}

template <typename Key, typename Value>
void MyHashMapNode<Key, Value>::remove(const Key& key, size_t hash_val)
{
    MyHashMapNode<Key, Value>* prev = find_prev(key, hash_val);
    if(!prev)
    {
        Logger::Log(__FILE__, __func__, DEBUG, "Element not existent in the map");
        return;
    }
    MyHashMapNode<Key, Value>* del = prev->next;
    prev->next = del->next;
    delete del;
}

template <typename Key, typename Value>
void MyHashMapNode<Key, Value>::Print()
{
    MyHashMapNode<Key, Value>* curr = this->next;
    while(curr)
    {
        Logger::Draw(curr->value, "(", curr->key, ")");
        curr = curr->next;
        if(curr)
            Logger::Draw(" -> ");
    }
}
template <typename Key, typename Value>
MyHashMapNode<Key, Value>* MyHashMapNode<Key, Value>::find_prev_or_last(const Key& key, size_t hash_val)
{
    MyHashMapNode<Key, Value>* curr = this;
    while(curr->next && curr->next->hash_value < hash_val)
    {
        curr = curr->next;
    }

    return curr;
}
template <typename Key, typename Value>
MyHashMapNode<Key, Value>* MyHashMapNode<Key, Value>::find_prev(const Key& key, size_t hash_val)
{
    MyHashMapNode<Key, Value>* prev = find_prev_or_last(key, hash_val);

    return prev->next ? prev : nullptr;
}
template <typename Key, typename Value>
MyHashMapNode<Key, Value>* MyHashMapNode<Key, Value>::find(const Key& key, size_t hash_val)
{
    MyHashMapNode<Key, Value>* prev = find_prev(key, hash_val);
    return prev ? prev->next : nullptr;
}


template class MyHashMapNode<int, int>;