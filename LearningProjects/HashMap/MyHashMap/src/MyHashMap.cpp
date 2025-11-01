#include "../include/MyHashMap.h"
#include <cstddef>
#include <cstdlib>

// Class functions
template<typename Key, typename Value>
MyHashMap<Key, Value>::MyHashMap(int expected_num_elements_, float load_factor_, bool auto_rehash_)
    : num_buckets(ceil(expected_num_elements_/load_factor_)),
     curr_num_elements(0),
     expected_num_elements(expected_num_elements_),
     max_load_factor(load_factor_),
     auto_rehash(auto_rehash_)
{
    LOG_INFO("Basic constructor");

    init_buckets();
}
template<typename Key, typename Value>
MyHashMap<Key, Value>::MyHashMap(const MyHashMap<Key, Value>& other)
    : num_buckets(other.num_buckets),
     curr_num_elements(other.curr_num_elements),
     expected_num_elements(other.expected_num_elements),
     max_load_factor(other.max_load_factor),
     auto_rehash(other.auto_rehash)
{
    LOG_INFO("Copy constructor");
    init_buckets(other.buckets);
}
template<typename Key, typename Value>
MyHashMap<Key, Value>::MyHashMap(MyHashMap<Key, Value>&& other)
    : num_buckets(other.num_buckets),
     curr_num_elements(other.curr_num_elements),
     buckets(other.buckets),
     expected_num_elements(other.expected_num_elements),
     max_load_factor(other.max_load_factor),
     auto_rehash(other.auto_rehash)
{
    LOG_INFO("Move constructor");
    other.buckets = nullptr;
    other.num_buckets = 0;
    other.curr_num_elements = 0;
    other.expected_num_elements = 0;
    other.max_load_factor = 0.0;
}
template<typename Key, typename Value>
MyHashMap<Key, Value>& MyHashMap<Key, Value>::operator=(const MyHashMap<Key, Value>& other)
{
    LOG_INFO("Copy assignment operator");
    if(this == &other)
        return *this;
    delete_all_buckets();
    delete[] buckets;
    num_buckets = other.num_buckets;
    max_load_factor = other.max_load_factor;
    curr_num_elements = other.curr_num_elements;
    expected_num_elements = other.expected_num_elements;
    auto_rehash = other.auto_rehash;
    create_buckets(other.buckets);

    return *this;
}
template<typename Key, typename Value>
MyHashMap<Key, Value>& MyHashMap<Key, Value>::operator=(MyHashMap<Key, Value>&& other)
{
    LOG_INFO("Move assignment operator");
    if(this == &other)
        return *this;
    delete_all_buckets();
    num_buckets = other.num_buckets;
    max_load_factor = other.max_load_factor;
    curr_num_elements = other.curr_num_elements;
    expected_num_elements = other.expected_num_elements;
    auto_rehash = other.auto_rehash;
    buckets = other.buckets;
    

    other.buckets = nullptr;
    other.num_buckets = 0;
    other.curr_num_elements = 0;
    other.expected_num_elements = 0;
    other.max_load_factor = 0.0;
    other.auto_rehash = false;

    return *this;
}
template<typename Key, typename Value>
MyHashMap<Key, Value>::~MyHashMap()
{
    LOG_INFO("Destructor");
    delete_all_buckets();
    LOG_DEBUG("Delete[] buckets");
    delete[] buckets;
    buckets = nullptr;
    num_buckets = 0;
    curr_num_elements = 0;
    expected_num_elements = 0;
    max_load_factor = 0.0;
    auto_rehash = false;
    LOG_DEBUG("Destructor end");
}

template<typename Key, typename Value>
void MyHashMap<Key, Value>::do_Add(const Key& key, const Value& value)
{
    LOG_DEBUG("do_Add(key, val)");
    size_t hash_val = hash_fun(key);
    size_t mod = hash_val % num_buckets;
    LOG_DEBUG("Hash_val: ", hash_val, "Mod value: ", mod);
    add_to_bucket(key, value, hash_val, mod);
    curr_num_elements++;
}
template<typename Key, typename Value>
void MyHashMap<Key, Value>::do_Add(MyHashMapNode<Key, Value>* other)
{
    LOG_DEBUG("do_Add(*)");
    size_t mod = other->hash_value % num_buckets;
    LOG_DEBUG("Mod value: ", mod);
    add_to_bucket(other, mod);
    curr_num_elements++;
}

template<typename Key, typename Value>
void MyHashMap<Key, Value>::Add(const Key& key, const Value& value)
{
    LOG_INFO("key: ", key, "value: ", value);
    do_Add(key, value);
    maybe_rehash();
}
template<typename Key, typename Value>
void MyHashMap<Key, Value>::Add(MyHashMapNode<Key, Value>* other)
{
    LOG_INFO();
    do_Add(other);
    maybe_rehash();
}

template<typename Key, typename Value>
Value* MyHashMap<Key, Value>::Get(const Key& key)
{
    LOG_INFO();
    size_t hash_val = hash_fun(key);
    size_t mod = hash_val % num_buckets;
    LOG_DEBUG("Hash_val: ", hash_val, "Mod value: ", mod);
    Value* ret_val = get_from_bucket(key, hash_val, mod);
    return ret_val;
}
template<typename Key, typename Value>
void MyHashMap<Key, Value>::Delete(const Key& key)
{
    LOG_INFO();
    size_t hash_val = hash_fun(key);
    size_t mod = hash_val % num_buckets;
    LOG_DEBUG("Hash_val: ", hash_val, "Mod value: ", mod);
    delete_from_bucket(key, hash_val, mod);
    curr_num_elements--;
    //maybe_rehash(); // TODO - negative scenario
}

template<typename Key, typename Value>
void MyHashMap<Key, Value>::Reserve(int new_exp_num_elems)
{
    LOG_INFO("Reserve called:", new_exp_num_elems);
    expected_num_elements = new_exp_num_elems;
    if(calc_load_factor(expected_num_elements) > max_load_factor)
        rehash(ceil(expected_num_elements/max_load_factor));
    LOG_DEBUG("New num of buckets:", num_buckets);
}

template<typename Key, typename Value>
int MyHashMap<Key, Value>::SetLoadFactor(float lf)
{
    LOG_DEBUG();
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
    LOG_DEBUG();
    DRAW("\nMyHashMap\n");
    DRAW("\n-----------------------------------------------------------------------------------------\n");
    for(int i = 0; i < num_buckets; i++)
    {
        DRAW(i, ":    ");
        buckets[i]->Print(); // TODO buckets->print(i):
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
    LOG_DEBUG();
    MyHashMapNode<Key, Value>* curr = buckets[mod]; // TODO null check
    curr->insert(key, val, hash_val);
}

template<typename Key, typename Value>
void MyHashMap<Key, Value>::add_to_bucket(MyHashMapNode<Key, Value>* new_node, int mod)
{
    LOG_DEBUG();
    MyHashMapNode<Key, Value>* curr = buckets[mod];// TODO null check
    curr->insert(new_node);
}

template<typename Key, typename Value>
Value* MyHashMap<Key, Value>::get_from_bucket(const Key& key, const size_t& hash_val, const size_t& mod)
{
    LOG_DEBUG();
    MyHashMapNode<Key, Value>* curr = buckets[mod];// TODO null check
    
    return curr->get(key, hash_val);
}

template<typename Key, typename Value>
void MyHashMap<Key, Value>::delete_from_bucket(const Key& key, const size_t& hash_val, const size_t& mod)
{
    LOG_DEBUG();
    MyHashMapNode<Key, Value>* curr = buckets[mod];// TODO null check
    curr->remove(key, hash_val);
}

template<typename Key, typename Value>
void MyHashMap<Key, Value>::delete_all_buckets()
{
    LOG_INFO();
    for(int i = 0; i < num_buckets; i++)
    {
        LOG_DEBUG("i: ", i);
        MyHashMapNode<Key, Value>* curr = buckets[i];
        while(curr)
        {
            LOG_DEBUG("curr != nullptr");
            MyHashMapNode<Key, Value>* temp = curr->getNext();
            LOG_DEBUG("Delete bucket: ", curr);
            delete curr;
            LOG_DEBUG("After deleting bucket");
            curr = temp;
            LOG_DEBUG("curr = ", temp);
        }
        LOG_DEBUG("i: ", i, " - End");
    }

    LOG_DEBUG("End");
}

template<typename Key, typename Value>
void MyHashMap<Key, Value>::copy_all_buckets(MyHashMapNode<Key, Value>** buckets_)
{
    LOG_INFO();
    copy_buckets(buckets, buckets_);
}

template<typename Key, typename Value>
void MyHashMap<Key, Value>::copy_buckets(MyHashMapNode<Key, Value>** buckets_new_buckets, MyHashMapNode<Key, Value>** buckets_other_buckets)
{
    LOG_DEBUG();
    for(int i = 0; i < num_buckets; i++)
    {
        LOG_DEBUG("i: ", i);
        if(buckets_other_buckets[i])
        {
            buckets_new_buckets[i]->copy_all_next(buckets_other_buckets[i]->next);
        }
        else
        {
            buckets_new_buckets[i]->next = nullptr;
        }
        LOG_DEBUG("END i: ", i);
    }
}

template<typename Key, typename Value>
void MyHashMap<Key, Value>::init_buckets()
{
    LOG_DEBUG();
    create_buckets(num_buckets);
}

template<typename Key, typename Value>
void MyHashMap<Key, Value>::init_buckets(MyHashMapNode<Key, Value>** other_buckets)
{
    LOG_DEBUG();
    create_buckets(other_buckets);
}

template<typename Key, typename Value>
void MyHashMap<Key, Value>::create_buckets(int num_buckets_)
{
    LOG_DEBUG("num_buckets_: ", num_buckets_);
    buckets = new MyHashMapNode<Key, Value>*[num_buckets_]();
    for(int i = 0; i < num_buckets_; i++)
    {
        buckets[i] = new MyHashMapNode<Key, Value>(); //TODO nullptr
    }
}

template<typename Key, typename Value>
void MyHashMap<Key, Value>::create_buckets(MyHashMapNode<Key, Value>** other_buckets)
{
    LOG_INFO();
    create_buckets(num_buckets);
    copy_buckets(buckets, other_buckets);
}

template<typename Key, typename Value>
void MyHashMap<Key, Value>::maybe_rehash()
{
    LOG_DEBUG();
    if(auto_rehash == false)
        return;
    if(calc_load_factor(curr_num_elements) > max_load_factor)
        rehash(num_buckets * 1.5);
}

template<typename Key, typename Value>
float MyHashMap<Key, Value>::calc_load_factor(int num_elements)
{
    return num_elements * 1.0 / num_buckets;
}

template<typename Key, typename Value>
void MyHashMap<Key, Value>::rehash(int new_num_buckets)
{
    LOG_INFO("Rehashing...(new_num_buckets: ", new_num_buckets, ")");
    LOG_CURR_STATE;
    MyHashMap<Key, Value> newTempMap(floor(new_num_buckets * max_load_factor), max_load_factor);
    for(int i = 0; i < num_buckets; i++)
    {
        LOG_DEBUG("Copying bucket num: ", i);
        MyHashMapNode<Key, Value>* curr = buckets[i];

        while(curr && curr->next) // skip curr - empty head
        {
            newTempMap.do_Add(curr->next);
            curr = curr->next;
        }
    }

    //this->PrintHashMap();
    //newTempMap.PrintHashMap();

    *this = std::move(newTempMap);
}


template class MyHashMap<int, int>;
