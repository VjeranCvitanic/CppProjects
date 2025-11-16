#ifndef _MyHashMapNode_
#define _MyHashMapNode_

#include <iostream>
#include <cstdlib>
#include "Logger.h"

template<typename Key, typename Value>
class MyHashMap;

template<typename Key, typename Value>
class MyHashMapNode
{
public:
    MyHashMapNode<Key, Value>();
    MyHashMapNode<Key, Value>(const Key& key_, const Value& val_, const size_t& hash_val_);
    MyHashMapNode<Key, Value>(const Key& key_, const Value& val_, const size_t& hash_val_, MyHashMapNode<Key, Value>* next_);
    MyHashMapNode<Key, Value>(const MyHashMapNode<Key, Value>& other);
    MyHashMapNode<Key, Value>(MyHashMapNode<Key, Value>&& other);
    MyHashMapNode& operator=(const MyHashMapNode<Key, Value>& other);
    MyHashMapNode& operator=(MyHashMapNode<Key, Value>&& other);
    ~MyHashMapNode<Key, Value>();

    MyHashMapNode<Key, Value>* getNext() const;
    void setValue(Value val);
    Key getKey();

    int8_t insert(const Key& key, const Value& val, const size_t& hash_val);
    int8_t insert(MyHashMapNode<Key, Value>* new_node);
    Value* get(const Key& key, const size_t& hash_val);
    int8_t remove(const Key& key, const size_t& hash_val);

    void Print();
private:
    Key key;
    Value value;
    size_t hash_value;
    MyHashMapNode* next;

    MyHashMapNode<Key, Value>* find_prev_or_last(const Key& key, const size_t& hash_val);
    MyHashMapNode<Key, Value>* find_prev(const Key& key, const size_t& hash_val);
    MyHashMapNode<Key, Value>* find(const Key& key, const size_t& hash_val);

    void delete_all_next();
    void copy_all_next(MyHashMapNode<Key, Value>* head);

    template<typename, typename>
    friend class MyHashMap;
};


#endif