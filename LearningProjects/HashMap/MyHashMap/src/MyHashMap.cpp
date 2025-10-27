#include "../include/MyHashMap.h"
#include "../include/utils.h"
#include <cstdlib>

// Class functions
template<typename Key, typename Value>
MyHashMap<Key, Value>::MyHashMap(int expected_num_elements_, float load_factor_)
    : num_buckets(expected_num_elements_/load_factor_), curr_num_elements(0), expected_num_elements(expected_num_elements_), max_load_factor(load_factor_)
{
    LOG_TRACE("Basic constructor");
    log_curr_state();

    init_buckets();
}
template<typename Key, typename Value>
MyHashMap<Key, Value>::MyHashMap(const MyHashMap<Key, Value>& other)
    : num_buckets(other.num_buckets)
{
    LOG_TRACE("Copy constructor");
    init_buckets(other.buckets);
}
template<typename Key, typename Value>
MyHashMap<Key, Value>::MyHashMap(MyHashMap<Key, Value>&& other)
    : num_buckets(other.num_buckets), buckets(other.buckets)
{
    LOG_TRACE("Move constructor");
    other.buckets = nullptr;
    other.num_buckets = 0;
}
template<typename Key, typename Value>
MyHashMap<Key, Value>& MyHashMap<Key, Value>::operator=(const MyHashMap<Key, Value>& other)
{
    LOG_TRACE("Copy assignment operator");
    if(this == &other)
        return *this;
    delete_all_buckets();
    num_buckets = other.num_buckets;
    copy_all_buckets(other.buckets);

    return *this;
}
template<typename Key, typename Value>
MyHashMap<Key, Value>& MyHashMap<Key, Value>::operator=(MyHashMap<Key, Value>&& other)
{
    LOG_TRACE("Move assignment operator");
    if(this == &other)
        return *this;
    delete_all_buckets();
    num_buckets = other.num_buckets;
    buckets = other.buckets;

    other.buckets = nullptr;
    other.num_buckets = 0;

    return *this;
}
template<typename Key, typename Value>
MyHashMap<Key, Value>::~MyHashMap()
{
    LOG_TRACE("Destructor");
    delete_all_buckets();
    delete[] buckets;
    num_buckets = 0;
}

template<typename Key, typename Value>
void MyHashMap<Key, Value>::Add(const Key& key, const Value& value)
{
    size_t hash_val = hash_fun(key);
    LOG_DEBUG("Hash_val: ", hash_val);
    size_t mod = hash_val % num_buckets;
    LOG_DEBUG("Mod value: ", mod);
    add_to_bucket(key, value, hash_val, mod);
    curr_num_elements++;
    maybe_rehash();
}
template<typename Key, typename Value>
void MyHashMap<Key, Value>::Add(MyHashMapNode<Key, Value>*&& other)
{
    size_t mod = other->hash_value % num_buckets;
    LOG_DEBUG("Mod value: ", mod);
    add_to_bucket(std::move(other), mod);
    curr_num_elements++;
    maybe_rehash();
}
template<typename Key, typename Value>
Value* MyHashMap<Key, Value>::Get(const Key& key)
{
    size_t hash_val = hash_fun(key);
    LOG_DEBUG("Hash_val: ", hash_val);
    size_t mod = hash_val % num_buckets;
    LOG_DEBUG("Mod value: ", mod);
    Value* ret_val = get_from_bucket(key, hash_val, mod);
    return ret_val;
}
template<typename Key, typename Value>
void MyHashMap<Key, Value>::Delete(const Key& key)
{
    size_t hash_val = hash_fun(key);
    LOG_DEBUG("Hash_val: ", hash_val);
    size_t mod = hash_val % num_buckets;
    LOG_DEBUG("Mod value: ", mod);
    delete_from_bucket(key, hash_val, mod);
    curr_num_elements--;
    maybe_rehash();
}

template<typename Key, typename Value>
void MyHashMap<Key, Value>::Reserve(int new_exp_num_elems)
{
    LOG_TRACE("Reserve called:", new_exp_num_elems);
    expected_num_elements = new_exp_num_elems;
    if(calc_load_factor(expected_num_elements) > max_load_factor)
        rehash_1();
    LOG_TRACE("New num of buckets:", num_buckets);
}

template<typename Key, typename Value>
int MyHashMap<Key, Value>::SetLoadFactor(float lf)
{
    if(lf <= 0)
    {
        LOG_WARNING("Load factor has to be in <0, +inf>, provided value: ", lf);
        return EXIT_FAILURE;
    }
    
    LOG_DEBUG("Max Load Factor set at: ", lf);
    max_load_factor = lf;

    return EXIT_SUCCESS;
}

template<typename Key, typename Value>
void MyHashMap<Key, Value>::PrintHashMap()
{
    DRAW("\nMyHashMap\n");
    DRAW("\n-----------------------------------------------------------------------------------------\n");
    for(int i = 0; i < num_buckets; i++)
    {
        DRAW(i, ":    ");
        buckets[i]->Print();
        DRAW("\n-----------------------------------------------------------------------------------------\n");
    }
}

template<typename Key, typename Value>
size_t MyHashMap<Key, Value>::hash_fun(const Key& key)
{
    return std::hash<Key>()(key);
}

template<typename Key, typename Value>
void MyHashMap<Key, Value>::add_to_bucket(const Key& key, const Value& val, const size_t& hash_val, const size_t& mod)
{
    MyHashMapNode<Key, Value>* curr = buckets[mod];
    curr->insert(key, val, hash_val);
}

template<typename Key, typename Value>
void MyHashMap<Key, Value>::add_to_bucket(MyHashMapNode<Key, Value>*&& new_node, int mod)
{
    MyHashMapNode<Key, Value>* curr = buckets[mod];
    curr->insert(std::move(new_node));
}

template<typename Key, typename Value>
Value* MyHashMap<Key, Value>::get_from_bucket(const Key& key, const size_t& hash_val, const size_t& mod)
{
    MyHashMapNode<Key, Value>* curr = buckets[mod];
    
    return curr->get(key, hash_val);
}

template<typename Key, typename Value>
void MyHashMap<Key, Value>::delete_from_bucket(const Key& key, const size_t& hash_val, const size_t& mod)
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
            LOG_DEBUG("Delete bucket");
            delete curr;
            curr = temp;
        }
    }
}

template<typename Key, typename Value>
void MyHashMap<Key, Value>::copy_all_buckets(MyHashMapNode<Key, Value>** buckets_)
{
    copy_buckets(buckets, buckets_);
}

template<typename Key, typename Value>
void MyHashMap<Key, Value>::copy_buckets(MyHashMapNode<Key, Value>**& buckets_new_buckets, MyHashMapNode<Key, Value>** buckets_other_buckets)
{
    for(int i = 0; i < num_buckets; i++)
    {
        if(buckets_other_buckets[i])
            buckets_new_buckets[i] = new MyHashMapNode<Key, Value>(*buckets_other_buckets[i]);
        else
        {
            LOG_WARNING("Buckets size mismatch at i: ", i);
            buckets_new_buckets[i] = new MyHashMapNode<Key, Value>();
        }
    }
}

template<typename Key, typename Value>
void MyHashMap<Key, Value>::init_buckets()
{
    buckets = create_buckets(num_buckets);
}

template<typename Key, typename Value>
void MyHashMap<Key, Value>::init_buckets(MyHashMapNode<Key, Value>** other_buckets)
{
    buckets = create_buckets(other_buckets);
}

template<typename Key, typename Value>
MyHashMapNode<Key, Value>** MyHashMap<Key, Value>::create_buckets(int num_buckets_)
{
    MyHashMapNode<Key, Value>** new_buckets = new MyHashMapNode<Key, Value>*[num_buckets_];
    for(int i = 0; i < num_buckets_; i++)
    {
        new_buckets[i] = new MyHashMapNode<Key, Value>();
    }

    return new_buckets;
}

template<typename Key, typename Value>
MyHashMapNode<Key, Value>** MyHashMap<Key, Value>::create_buckets(MyHashMapNode<Key, Value>** other_buckets)
{
    LOG_TRACE("Creating buckets");
    MyHashMapNode<Key, Value>** new_buckets = new MyHashMapNode<Key, Value>*[num_buckets];
    copy_buckets(new_buckets, other_buckets);

    return new_buckets;
}

template<typename Key, typename Value>
void MyHashMap<Key, Value>::maybe_rehash()
{
    LOG_DEBUG("maybe_rehash called");
    if(calc_load_factor(curr_num_elements) > max_load_factor)
        rehash_2();
}

template<typename Key, typename Value>
float MyHashMap<Key, Value>::calc_load_factor(int num_elements)
{
    return num_elements * 1.0 / num_buckets;
}

template<typename Key, typename Value>
void MyHashMap<Key, Value>::rehash_1()
{
    LOG_DEBUG("Rehash called");
    rehash(expected_num_elements / max_load_factor);
}

template<typename Key, typename Value>
void MyHashMap<Key, Value>::rehash_2()
{
    LOG_DEBUG("Rehash called");
    rehash(num_buckets * 1.5); // 50% increase
}

template<typename Key, typename Value>
void MyHashMap<Key, Value>::rehash(int new_num_buckets)
{
    LOG_INFO("Rehashing...");
    LOG_VAR(new_num_buckets);
    log_curr_state();
    MyHashMap<Key, Value> newTempMap(max(curr_num_elements, expected_num_elements), max_load_factor);

    for(int i = 0; i < num_buckets; i++)
    {
        LOG_DEBUG("Copying bucket num: ", i);
        MyHashMapNode<Key, Value>* curr = buckets[i];
        while(curr->next)
        {
            newTempMap.Add(std::move(curr->next));
            curr->next = curr->next->next;
        }
    }

    *this = std::move(newTempMap);
}

template<typename Key, typename Value>
void MyHashMap<Key, Value>::log_curr_state()
{
    LOG_DEBUG("Curr num of elems: ", curr_num_elements, ", Exp num: ", expected_num_elements, ", Num buckets: ", num_buckets, " Max Load Factor: ", max_load_factor, ", Curr load factor: ", calc_load_factor(curr_num_elements));
}

template class MyHashMap<int, int>;
