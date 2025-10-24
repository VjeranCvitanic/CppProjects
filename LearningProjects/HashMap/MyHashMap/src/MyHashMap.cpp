#include "../include/MyHashMap.h"

// Class functions
template<typename Key, typename Value>
MyHashMap<Key, Value>::MyHashMap(int num_buckets_)
    : num_buckets(num_buckets_)
{
    Logger::Log(__FILE__, __func__, TRACE, "Basic constructor");
    buckets = new MyHashMapNode<Key, Value>*[num_buckets];
    for(int i = 0; i < num_buckets; i++)
    {
        buckets[i] = new MyHashMapNode<Key, Value>();
    }
}
template<typename Key, typename Value>
MyHashMap<Key, Value>::MyHashMap(const MyHashMap<Key, Value>& other)
    : num_buckets(other.num_buckets)
{
    Logger::Log(__FILE__, __func__,  TRACE, "Copy constructor");
    buckets = new MyHashMapNode<Key, Value>*[num_buckets];
    for(int i = 0; i < num_buckets; i++)
    {
        buckets[i] = new MyHashMapNode<Key, Value>(*other.buckets[i]);
    }
}
template<typename Key, typename Value>
MyHashMap<Key, Value>::MyHashMap(MyHashMap<Key, Value>&& other)
    : num_buckets(other.num_buckets), buckets(other.buckets)
{
    Logger::Log(__FILE__, __func__,  TRACE, "Move constructor");
    other.buckets = nullptr;
    other.num_buckets = 0;
}
template<typename Key, typename Value>
MyHashMap<Key, Value>& MyHashMap<Key, Value>::operator=(const MyHashMap<Key, Value>& other)
{
    Logger::Log(__FILE__, __func__,  TRACE, "Copy assignment operator");
    if(this == &other)
        return *this;
    num_buckets = other.num_buckets;
    delete_all_buckets();
    copy_all_buckets(other.buckets);

    return *this;
}
template<typename Key, typename Value>
MyHashMap<Key, Value>& MyHashMap<Key, Value>::operator=(MyHashMap<Key, Value>&& other)
{
    Logger::Log(__FILE__, __func__,  TRACE, "Move assignment operator");
    if(this == &other)
        return *this;
    num_buckets = other.num_buckets;
    delete_all_buckets();
    buckets = other.buckets;

    other.buckets = nullptr;
    other.num_buckets = 0;

    return *this;
}
template<typename Key, typename Value>
MyHashMap<Key, Value>::~MyHashMap()
{
    Logger::Log(__FILE__, __func__,  TRACE, "Destructor");
    delete_all_buckets();
    delete[] buckets;
    num_buckets = 0;
}

template<typename Key, typename Value>
void MyHashMap<Key, Value>::Add(const Key& key, const Value& value)
{
    size_t hash_val = hash_fun(key);
    size_t mod = hash_val % num_buckets;
    Logger::Log(__FILE__, __func__,  DEBUG, mod);
    add_to_bucket(key, value, hash_val, mod);
}
template<typename Key, typename Value>
Value* MyHashMap<Key, Value>::Get(const Key& key)
{
    size_t hash_val = hash_fun(key);
    size_t mod = hash_val % num_buckets;
    Logger::Log(__FILE__, __func__,  DEBUG, mod);
    Value* ret_val = get_from_bucket(key, hash_val, mod);
    return ret_val;
}
template<typename Key, typename Value>
void MyHashMap<Key, Value>::Delete(const Key& key)
{
    size_t hash_val = hash_fun(key);
    size_t mod = hash_val % num_buckets;
    delete_from_bucket(key, hash_val, mod);
}

template<typename Key, typename Value>
void MyHashMap<Key, Value>::PrintHashMap()
{
    Logger::Draw("MyHashMap\n");
    Logger::Draw("\n-----------------------------------------------------------------------------------------\n");
    for(int i = 0; i < num_buckets; i++)
    {
        Logger::Draw(i, ":    ");
        buckets[i]->Print();
        Logger::Draw("\n-----------------------------------------------------------------------------------------\n");
    }
}


template<typename Key, typename Value>
size_t MyHashMap<Key, Value>::hash_fun(const Key& key)
{
    return std::hash<Key>()(key);
}

template<typename Key, typename Value>
void MyHashMap<Key, Value>::add_to_bucket(const Key& key, const Value& val, const size_t hash_val, const size_t mod)
{
    MyHashMapNode<Key, Value>* curr = buckets[mod];
    curr->insert(key, val, hash_val);
}

template<typename Key, typename Value>
Value* MyHashMap<Key, Value>::get_from_bucket(const Key& key, const size_t hash_val, const size_t mod)
{
    MyHashMapNode<Key, Value>* curr = buckets[mod];
    
    return curr->get(key, hash_val);
}

template<typename Key, typename Value>
void MyHashMap<Key, Value>::delete_from_bucket(const Key& key, const size_t hash_val, const size_t mod)
{
    MyHashMapNode<Key, Value>* curr = buckets[mod];
    curr->remove(key, hash_val);
}

template<typename Key, typename Value>
void MyHashMap<Key, Value>::delete_all_buckets()
{
    for(int i = 0; i < num_buckets; i++)
    {
        MyHashMapNode<Key, Value>* curr = buckets[i];
        while(curr)
        {
            MyHashMapNode<Key, Value>* temp = curr->getNext();
            delete curr;
            curr = temp;
        }
    }
}

template<typename Key, typename Value>
void MyHashMap<Key, Value>::copy_all_buckets(MyHashMapNode<Key, Value>** buckets_)
{
    for(int i = 0; i < num_buckets; i++)
    {
        buckets[i] = new MyHashMapNode<Key, Value>(*buckets_[i]);
    }
}


template class MyHashMap<int, int>;
