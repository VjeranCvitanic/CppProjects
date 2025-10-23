#include <cstddef>
#include <cstdlib>
#include <functional>
#include <iostream>

#define DEBUG 0

template <typename T>
void print(T data)
{
#if DEBUG
    std::cerr << data << std::endl;
#endif
}

template<typename Key, typename Value>
class MyHashMapNode
{
public:
    MyHashMapNode<Key, Value>()
        : key{}, value{}, hash_value(0), next(nullptr)
    {
        print("Node: constructor");
    }
    MyHashMapNode<Key, Value>(Key key_, Value val_, int hash_val_)
        : key(key_), value(val_), hash_value(hash_val_), next(nullptr)
    {
        print("Node: constructor 2");
    }
    MyHashMapNode<Key, Value>(Key key_, Value val_, int hash_val_, MyHashMapNode<Key, Value>* next_)
        : key(key_), value(val_), hash_value(hash_val_), next(next_)
    {
        print("Node: constructor 3");
    }
    MyHashMapNode<Key, Value>(const MyHashMapNode<Key, Value>& other)
        : key(other.key), value(other.value), hash_value(other.hash_value)
    {
        print("Node: copy constructor");
        next = other.next ? new MyHashMapNode<Key, Value>(*other.next) : nullptr; // TODO: recursion problem -> init by value?
    }
    MyHashMapNode<Key, Value>(MyHashMapNode<Key, Value>&& other)
        : key(std::move(other.key)), value(std::move(other.value)), hash_value(other.hash_value), next(other.next)
    {
        print("Node: move constructor");
        other.next = nullptr;
        other.key = {};
        other.value = {};
        other.hash_value = 0;
    }
    MyHashMapNode& operator=(const MyHashMapNode<Key, Value>& other)
    {
        print("Node: copy assignment operator");
        if(this == &other)
            return this;
        key = other.key;
        value = other.value;
        hash_value = other.hash_value;
        next = other.next ? new MyHashMapNode<Key, Value>(*other.next) : nullptr; // TODO: recursion problem -> init by value?

        return *this;
    }
    MyHashMapNode& operator=(MyHashMapNode<Key, Value>&& other)
    {
        print("Node: move assignment operator");
        if(this == &other)
            return this;
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
    
    ~MyHashMapNode<Key, Value>()
    {
        print("Node: destructor");
    }

    MyHashMapNode<Key, Value>* getNext() const
    {
        return next;
    }

    void insert(const Key& key, const Value& val, size_t hash_val)
    {
        MyHashMapNode<Key, Value>* prev = find_prev_or_last(key, hash_val);

        prev->next = new MyHashMapNode<Key, Value>(key, val, hash_val, prev->next);
    }

    Value* get(const Key& key, size_t hash_val)
    {
        MyHashMapNode<Key, Value>* target = find(key, hash_val);
        if(target && target->hash_value == hash_val)
        {
            if(target->key == key)
                {std::cout << target->value << std::endl;
                return &target->value;}
            return nullptr;
        }
        else 
            return nullptr;
    }

    void remove(const Key& key, size_t hash_val)
    {
        MyHashMapNode<Key, Value>* prev = find_prev(key, hash_val);
        if(!prev)
            return;
        MyHashMapNode<Key, Value>* del = prev->next;
        prev->next = del->next;
        delete del;
    }

    void Print()
    {
        MyHashMapNode<Key, Value>* curr = this->next;
        while(curr)
        {
            std::cout << curr->value << "(" << curr->key << ")";
            curr = curr->next;
            if(curr)
                std::cout << " -> ";
        }
    }
private:
    Key key;
    Value value;
    int hash_value; // for comparison
    MyHashMapNode* next;

    MyHashMapNode<Key, Value>* find_prev_or_last(const Key& key, size_t hash_val)
    {
        MyHashMapNode<Key, Value>* curr = this;
        while(curr->next && curr->next->hash_value < hash_val)
        {
            curr = curr->next;
        }

        return curr;
    }
    MyHashMapNode<Key, Value>* find_prev(const Key& key, size_t hash_val)
    {
        MyHashMapNode<Key, Value>* prev = find_prev_or_last(key, hash_val);

        return prev->next ? prev : nullptr;
    }
    MyHashMapNode<Key, Value>* find(const Key& key, size_t hash_val)
    {
        MyHashMapNode<Key, Value>* prev = find_prev(key, hash_val);
        return prev ? prev->next : nullptr;
    }
};

template<typename Key, typename Value>
class MyHashMap
{
public:
    MyHashMap<Key, Value>(int num_buckets_)
        : num_buckets(num_buckets_)
    {
        print("Map: constructor");
        buckets = new MyHashMapNode<Key, Value>*[num_buckets];
        for(int i = 0; i < num_buckets; i++)
        {
            buckets[i] = new MyHashMapNode<Key, Value>();
        }
    }
    MyHashMap<Key, Value>(const MyHashMap<Key, Value>& other)
        : num_buckets(other.num_buckets)
    {
        print("Map: copy constructor");
        buckets = new MyHashMapNode<Key, Value>*[num_buckets];
        for(int i = 0; i < num_buckets; i++)
        {
            buckets[i] = new MyHashMapNode<Key, Value>(*other.buckets[i]);
        }
    }
    MyHashMap<Key, Value>(MyHashMap<Key, Value>&& other)
        : num_buckets(other.num_buckets), buckets(other.buckets)
    {
        print("Map: move constructor");
        other.buckets = nullptr;
        other.num_buckets = 0;
    }
    MyHashMap<Key, Value>& operator=(const MyHashMap<Key, Value>& other)
    {
        print("Map: copy operator");
        if(*this == other)
            return this;
        num_buckets = other.num_buckets;
        delete_all_buckets();
        copy_all_buckets(other.buckets);

        return *this;
    }
    MyHashMap<Key, Value>& operator=(MyHashMap<Key, Value>&& other)
    {
        print("Map: move operator");
        if(*this == other)
            return this;
        num_buckets = other.num_buckets;
        delete_all_buckets();
        buckets = other.buckets;

        other.buckets = nullptr;
        other.num_buckets = 0;

        return *this;
    }
    ~MyHashMap<Key, Value>()
    {
        print("Map: destructor");
        delete_all_buckets();
        delete[] buckets;
        num_buckets = 0;
    }

    void Add(const Key& key, const Value& value)
    {
        size_t hash_val = hash_fun(key);
        size_t mod = hash_val % num_buckets;
        print("mod");
        print(mod);
        add_to_bucket(key, value, hash_val, mod);
    }
    Value* Get(const Key& key)
    {
        size_t hash_val = hash_fun(key);
        size_t mod = hash_val % num_buckets;
        Value* ret_val = get_from_bucket(key, hash_val, mod);
        return ret_val;
    }
    void Delete(const Key& key)
    {
        size_t hash_val = hash_fun(key);
        size_t mod = hash_val % num_buckets;
        delete_from_bucket(key, hash_val, mod);
    }

    void Print()
    {
        std::cout << "MyHashMap" << std::endl;
        std::cout << std::endl << "---------------------" << std::endl;
        for(int i = 0; i < num_buckets; i++)
        {
            std::cout << i << ":    ";
            buckets[i]->Print();
            std::cout << std::endl << "---------------------" << std::endl;
        }
    }
private:
    int num_buckets;
    MyHashMapNode<Key, Value>** buckets;

    size_t hash_fun(const Key& key)
    {
        return std::hash<Key>()(key);
    }

    void add_to_bucket(const Key& key, const Value& val, const size_t hash_val, const size_t mod)
    {
        MyHashMapNode<Key, Value>* curr = buckets[mod];
        print("HERE");
        if(!curr)
        {
            print("Error!");
            return;
        }
        curr->insert(key, val, hash_val);
    }

    Value* get_from_bucket(const Key& key, const size_t hash_val, const size_t mod)
    {
        MyHashMapNode<Key, Value>* curr = buckets[mod];
        
        return curr->get(key, hash_val);
    }

    void delete_from_bucket(const Key& key, const size_t hash_val, const size_t mod)
    {
        MyHashMapNode<Key, Value>* curr = buckets[mod];
        curr->remove(key, hash_val);
    }

    void delete_all_buckets()
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

    void copy_all_buckets(MyHashMapNode<Key, Value>** buckets_)
    {
        for(int i = 0; i < num_buckets; i++)
        {
            buckets[i] = new MyHashMapNode<Key, Value>(*buckets_[i]);
        }
    }
};

int main()
{
    MyHashMap<int, int> map(5);
    srand((unsigned)time(0)); 
    for(int i = 0; i < 100; i++)
    {
        map.Add(i, rand() % 100);
    }

    map.Print();

    for(int i = 0; i < 10; i++)
    {
        int* val = map.Get(i);
        if(val)
            std::cout << i << ": " << *val << std::endl;
    }
    return 0;
}

// TODO:
// copy & move constructors, copy & move assignment operators - DONE
// Logger
// print tree - DONE
// issue w Get