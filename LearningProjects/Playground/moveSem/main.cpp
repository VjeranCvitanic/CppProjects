#include <iostream>

template <typename T>
void print(T msg)
{
    std::cout << msg << std::endl;
}


class MyString
{
public:
    MyString()
        : data(nullptr), size(0)
    {}
    MyString(const char* data_)
    {
        print("MyString: Create constructor: ");
        std::cout << data_ << std::endl;
        size = strlen(data_) + 1;
        data = new char[size];
        memcpy(data, data_, size);
    }
    MyString(const MyString& other)
    {
        print("MyString: Copy constructor");
        size = other.size;
        data = new char[size];
        memcpy(data, other.data, size);
    }
    MyString(MyString&& other)
    {
        print("MyString: Move constructor");
        size = other.size;
        data = other.data;

        other.size = 0;
        other.data = nullptr;
    }
    MyString& operator=(const MyString& other)
    {
        print("MyString: Copy assignment operator");
        if(this == &other)
            return *this;
        delete[] data;
        size = other.size;
        data = new char[size];
        memcpy(data, other.data, size);
        return *this;
    }
    MyString& operator=(MyString&& other)
    {
        print("MyString: Move assignment operator");
        if(this == &other)
            return *this;
        size = other.size;
        delete[] data;
        data = other.data;

        other.size = 0;
        other.data = nullptr;
        return *this;
    }

    ~MyString()
    {
        print("MyString: Destructor");
        size = 0;
        delete[] data;
    }
private:
    char* data;
    int size;
};

class Person
{
public:
    Person()
        : name(""), age(0) 
    {
        print("Person: default constructor");
    }
    Person(const MyString& name_, int age_)
        : name(name_), age(age_)
    {
        print("Person: create constructor 1");
    }
    Person(MyString&& name_, int age_)
        : name(std::move(name_)), age(age_)
    {
        print("Person: create constructor 2");
    }
    Person(const Person& other)
        : name(other.name), age(other.age)
    {
        print("Person: copy constructor");
    }
    Person(Person&& other)
        : name(std::move(other.name)), age(other.age)
    {
        print("Person: move constructor");
    }
    Person operator=(const Person& other)
    {
        print("Person: copy assignment operator");
        if(this == &other)
            return *this;
        name = other.name;
        age = other.age;

        return *this;
    }
    Person operator=(Person&& other)
    {
        print("Person: move assignment operator");
        if(this == &other)
            return *this;
        name = std::move(other.name);
        age = other.age;

        return *this;
    }
    ~Person()
    {
        print("Person: destructor");
    }
private:
    MyString name;
    int age;
};

class Student : public Person
{
public:
    Student(const MyString& college_, float grade_) 
        : college(college_), grade(grade_)
    {
        print("Student: constructor 1");
    }
    Student(MyString&& college_, float grade_) 
        : college(std::move(college_)), grade(grade_)
    {
        print("Student: constructor 2");
    }
    ~Student()
    {
        print("Student: destructor");
    }
private:
    MyString college;
    float grade;
};

int main()
{
    //MyString name = "Anton";
    //Person a(MyString("Vjeran"), 23);
    Student s("FESB", 4.56);
    //Person b(name, 17);
    return 0;
}