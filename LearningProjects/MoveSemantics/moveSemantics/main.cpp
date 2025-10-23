#include <cstddef>
#include <iostream>
#include <cstring>

class MyString
{
public:
    MyString() = default;
    MyString(const char* data_)
    {
        std::cout << "string create" << std::endl;
        size = strlen(data_);
        data = new char[size];
        memcpy(data, data_, size);
    }
    MyString(const MyString& other)
    {
        std::cout << "string copy" << std::endl;
        size = other.size;
        data = new char[size];
        memcpy(data, other.data, size);
    }
    MyString(MyString&& other) noexcept
    {
        std::cout << "string move" << std::endl;
        size = other.size;
        data = other.data;
        
        other.size = 0;
        other.data = nullptr;
    }
    ~MyString()
    {
        delete[] data;
    }
    MyString& operator=(MyString&& other)
    {
        std::cout << "string move operator" << std::endl;
        if (this == &other) 
            return *this;
        std::cout << "string move operator" << std::endl;
        std::cout << data << std::endl;
        delete[] data;
        std::cout << "string move operator" << std::endl;
        size = other.size;
        data = other.data;
        
        other.size = 0;
        other.data = nullptr;

        return *this;
    }
private:
    char* data;
    int size;
};

class Person
{
public:
    Person() = default;
    Person(const MyString& name_) : name(name_)
    {
        std::cout << "Person create" << std::endl;
    }
    Person(MyString&& name_) : name(std::move(name_))
    {
        std::cout << "Person create 2" << std::endl;
    }
    ~Person()
    {
        std::cout << "delete" << std::endl;
    }
private:
    MyString name;
};

int main()
{
    Person a("Vjeran");

    return 0;
}