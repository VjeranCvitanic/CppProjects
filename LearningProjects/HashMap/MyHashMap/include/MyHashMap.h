#ifndef _MyHashMap_
#define _MyHashMap_

#include <iostream>
#include "MyHashMapNode.h"
#include "Logger.h"

#define DEFAULT_START_NUM_BUCKETS 10
#define DEFAULT_START_MAX_LOAD_FACTOR 0.75
#define DEFAULT_EXPECTED_NUM_ELEMENTS 1000

template<typename Key, typename Value>
class MyHashMap
{
public:
    MyHashMap<Key, Value>(int expected_num_elements_ = DEFAULT_EXPECTED_NUM_ELEMENTS, float load_factor_ = DEFAULT_START_MAX_LOAD_FACTOR);
    MyHashMap<Key, Value>(const MyHashMap<Key, Value>& other);
    MyHashMap<Key, Value>(MyHashMap<Key, Value>&& other);
    MyHashMap<Key, Value>& operator=(const MyHashMap<Key, Value>& other);
    MyHashMap<Key, Value>& operator=(MyHashMap<Key, Value>&& other);
    ~MyHashMap<Key, Value>();

    void Add(const Key& key, const Value& value);
    void Add(MyHashMapNode<Key, Value>*&& other);
    Value* Get(const Key& key);
    void Delete(const Key& key);

    void Reserve(int exp_num_elems);

    int SetLoadFactor(float lf);

    void PrintHashMap();

private:
    int num_buckets;
    int curr_num_elements;
    int expected_num_elements;
    float max_load_factor;
    MyHashMapNode<Key, Value>** buckets;

    size_t hash_fun(const Key& key);

    void add_to_bucket(const Key& key, const Value& val, const size_t& hash_val, const size_t& mod);
    void add_to_bucket(MyHashMapNode<Key, Value>*&& new_node, int mod);
    Value* get_from_bucket(const Key& key, const size_t& hash_val, const size_t& mod);
    void delete_from_bucket(const Key& key, const size_t& hash_val, const size_t& mod);
    void delete_all_buckets();
    void copy_all_buckets(MyHashMapNode<Key, Value>** buckets_);
    void copy_buckets(MyHashMapNode<Key, Value>**& buckets_new_buckets, MyHashMapNode<Key, Value>** buckets_other_buckets);
    void init_buckets();
    void init_buckets(MyHashMapNode<Key, Value>** other_buckets);
    MyHashMapNode<Key, Value>** create_buckets(int num_buckets_);
    MyHashMapNode<Key, Value>** create_buckets(MyHashMapNode<Key, Value>** buckets_);

    float calc_load_factor(int num_elements);

    void maybe_rehash();
    void rehash_1();
    void rehash_2();
    void rehash(int new_num_buckets);

    void log_curr_state(); // Level?
};

#endif