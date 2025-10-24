#ifndef _MyHashMap_
#define _MyHashMap_

#include <iostream>
#include "MyHashMapNode.h"
#include "Logger.h"

#define DEFAULT_NUM_BUCKETS 8

template<typename Key, typename Value>
class MyHashMap
{
public:
    MyHashMap<Key, Value>(int num_buckets_ = DEFAULT_NUM_BUCKETS);
    MyHashMap<Key, Value>(const MyHashMap<Key, Value>& other);
    MyHashMap<Key, Value>(MyHashMap<Key, Value>&& other);
    MyHashMap<Key, Value>& operator=(const MyHashMap<Key, Value>& other);
    MyHashMap<Key, Value>& operator=(MyHashMap<Key, Value>&& other);
    ~MyHashMap<Key, Value>();

    void Add(const Key& key, const Value& value);
    Value* Get(const Key& key);
    void Delete(const Key& key);

    void PrintHashMap();

private:
    int num_buckets;
    MyHashMapNode<Key, Value>** buckets;

    size_t hash_fun(const Key& key);

    void add_to_bucket(const Key& key, const Value& val, const size_t hash_val, const size_t mod);
    Value* get_from_bucket(const Key& key, const size_t hash_val, const size_t mod);
    void delete_from_bucket(const Key& key, const size_t hash_val, const size_t mod);
    void delete_all_buckets();
    void copy_all_buckets(MyHashMapNode<Key, Value>** buckets_);
};

#endif