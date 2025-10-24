#ifndef _MyHashMapNode_
#define _MyHashMapNode_

#include <iostream>
#include "Logger.h"

template<typename Key, typename Value>
class MyHashMapNode
{
public:
    MyHashMapNode<Key, Value>();
    MyHashMapNode<Key, Value>(Key key_, Value val_, size_t hash_val_);
    MyHashMapNode<Key, Value>(Key key_, Value val_, size_t hash_val_, MyHashMapNode<Key, Value>* next_);
    MyHashMapNode<Key, Value>(const MyHashMapNode<Key, Value>& other);
    MyHashMapNode<Key, Value>(MyHashMapNode<Key, Value>&& other);
    MyHashMapNode& operator=(const MyHashMapNode<Key, Value>& other);
    MyHashMapNode& operator=(MyHashMapNode<Key, Value>&& other);
    ~MyHashMapNode<Key, Value>();

    MyHashMapNode<Key, Value>* getNext() const;

    void insert(const Key& key, const Value& val, size_t hash_val);
    Value* get(const Key& key, size_t hash_val);
    void remove(const Key& key, size_t hash_val);

    void Print();
private:
    Key key;
    Value value;
    size_t hash_value;
    MyHashMapNode* next;

    MyHashMapNode<Key, Value>* find_prev_or_last(const Key& key, size_t hash_val);
    MyHashMapNode<Key, Value>* find_prev(const Key& key, size_t hash_val);
    MyHashMapNode<Key, Value>* find(const Key& key, size_t hash_val);
};

#endif