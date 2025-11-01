#include "../include/MyHashMapNode.h"

// Class functions
template <typename Key, typename Value>
MyHashMapNode<Key, Value>::MyHashMapNode()
    : key{}, value{}, hash_value(0), next(nullptr)
{
    LOG_DEBUG("Value Constructor");
}
template <typename Key, typename Value>
MyHashMapNode<Key, Value>::MyHashMapNode(const Key& key_, const Value& val_, const size_t& hash_val_)
    : key(key_), value(val_), hash_value(hash_val_), next(nullptr)
{
    LOG_VAR("Value Constructor 2", key_, val_, hash_val_);
}

template <typename Key, typename Value>
MyHashMapNode<Key, Value>::MyHashMapNode(const Key& key_, const Value& val_, const size_t& hash_val_, MyHashMapNode<Key, Value>* next_)
    : key(key_), value(val_), hash_value(hash_val_), next(next_)
{
    LOG_VAR("Value Constructor 3", key_, val_, hash_val_, next_);
}

template <typename Key, typename Value>
MyHashMapNode<Key, Value>::MyHashMapNode(const MyHashMapNode<Key, Value>& other)
    : key(other.key), value(other.value), hash_value(other.hash_value)
{
    LOG_DEBUG("Copy Constructor");
    copy_all_next(other.next); // skip (empty) head element
    LOG_DEBUG("Exit copy Constructor");
}
template <typename Key, typename Value>
MyHashMapNode<Key, Value>::MyHashMapNode(MyHashMapNode<Key, Value>&& other)
    : key(std::move(other.key)), value(std::move(other.value)), hash_value(other.hash_value), next(other.next)
{
    LOG_DEBUG("Move Constructor");
    other.next = nullptr;
    other.key = {};
    other.value = {};
    other.hash_value = 0;
}
template <typename Key, typename Value>
MyHashMapNode<Key, Value>& MyHashMapNode<Key, Value>::operator=(const MyHashMapNode<Key, Value>& other)
{
    LOG_DEBUG("Copy assignment operator");
    if(this == &other)
        return *this;
    key = other.key;
    value = other.value;
    hash_value = other.hash_value;
    delete_all_next();

    copy_all_next(other.next); // skip (empty) head element

    return *this;
}
template <typename Key, typename Value>
MyHashMapNode<Key, Value>& MyHashMapNode<Key, Value>::operator=(MyHashMapNode<Key, Value>&& other)
{
    LOG_DEBUG("Move assignment operator");
    if(this == &other)
        return *this;
    key = std::move(other.key);
    value = std::move(other.value);
    hash_value = other.hash_value;
    delete_all_next();
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
    LOG_VAR(key, value, hash_value, next);
}

template <typename Key, typename Value>
MyHashMapNode<Key, Value>* MyHashMapNode<Key, Value>::getNext() const
{
    LOG_DEBUG();
    return next;
}

template <typename Key, typename Value>
void MyHashMapNode<Key, Value>::insert(const Key& key, const Value& val, const size_t& hash_val)
{
    LOG_INFO();
    MyHashMapNode<Key, Value>* prev = find_prev_or_last(key, hash_val);
    prev->next = new MyHashMapNode<Key, Value>(key, val, hash_val, prev->next);
}

template <typename Key, typename Value>
void MyHashMapNode<Key, Value>::insert(MyHashMapNode<Key, Value>* new_node)
{
    LOG_INFO("ptr");
    MyHashMapNode<Key, Value>* prev = find_prev_or_last(key, new_node->hash_value);
    prev->next = new MyHashMapNode<Key, Value>(new_node->key, new_node->value, new_node->hash_value, prev->next);
}

template <typename Key, typename Value>
Value* MyHashMapNode<Key, Value>::get(const Key& key, const size_t& hash_val)
{
    LOG_INFO();
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
void MyHashMapNode<Key, Value>::remove(const Key& key, const size_t& hash_val)
{
    LOG_INFO();
    MyHashMapNode<Key, Value>* prev = find_prev(key, hash_val);
    if(!prev)
    {
        LOG_WARNING("Element not existent in the map");
        LOG_VAR(key, hash_val);
        return;
    }
    MyHashMapNode<Key, Value>* del = prev->next;
    prev->next = del->next;
    LOG_VAR(del->key, del->value, del->hash_value, del->next);
    delete del;
}

template <typename Key, typename Value>
void MyHashMapNode<Key, Value>::Print()
{
    MyHashMapNode<Key, Value>* curr = this->next;
    while(curr)
    {
        DRAW(curr->value, "(", curr->key, ")");
        curr = curr->next;
        if(curr)
            DRAW(" -> ");
    }
}

template <typename Key, typename Value>
MyHashMapNode<Key, Value>* MyHashMapNode<Key, Value>::find_prev_or_last(const Key& key, const size_t& hash_val)
{
    LOG_DEBUG();
    MyHashMapNode<Key, Value>* curr = this;
    while(curr->next && curr->next->hash_value < hash_val)
    {
        curr = curr->next;
    }

    return curr;
}
template <typename Key, typename Value>
MyHashMapNode<Key, Value>* MyHashMapNode<Key, Value>::find_prev(const Key& key, const size_t& hash_val)
{
    LOG_DEBUG();
    MyHashMapNode<Key, Value>* prev = find_prev_or_last(key, hash_val);

    return prev->next ? prev : nullptr;
}
template <typename Key, typename Value>
MyHashMapNode<Key, Value>* MyHashMapNode<Key, Value>::find(const Key& key, const size_t& hash_val)
{
    LOG_DEBUG();
    MyHashMapNode<Key, Value>* prev = find_prev(key, hash_val);
    return prev ? prev->next : nullptr;
}

template <typename Key, typename Value>
void MyHashMapNode<Key, Value>::delete_all_next()
{
    LOG_INFO();
    MyHashMapNode<Key, Value>* curr = this;
    MyHashMapNode<Key, Value>* temp = nullptr;
    while(curr->next)
    {
        temp = curr->next;
        curr->next = temp->next;
        delete temp;
    }
}

template <typename Key, typename Value>
void MyHashMapNode<Key, Value>::copy_all_next(MyHashMapNode<Key, Value>* other)
{
    LOG_DEBUG();
    MyHashMapNode<Key, Value>* curr_this = this;
    while(other)
    {
        curr_this->next = new MyHashMapNode<Key, Value>(other->key, other->value, other->hash_value, nullptr);
        
        other = other->next;
        curr_this = curr_this->next;
    }
}

template class MyHashMapNode<int, int>;