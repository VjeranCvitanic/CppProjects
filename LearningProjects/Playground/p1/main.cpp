#include <iostream>
#include <cstring>

class Person{
public:
    Person() = default;
    Person(char* name_, const int& age_)
        : name(name_), age(age_)
    {
        std::cout << "Constructor: " << &name << ", " << &name_ << std::endl;
    }
    ~Person() = default;
    char* getName()
    {
        return name;
    }
    void setPerson(char*&& name_, const int& age_)
    {
        name = name_;
        age = age_;
        std::cout << "setPerson: " << &name << ", " << &name_ << std::endl;
    }
//private:
    char* name;
    int age;
};

int main()
{
    Person a;
    a.setPerson("Vjeran", 23);
    {
        char ante[] = "Ante";
        a.setPerson(std::move(ante), 2);
        ante[0] = 'o';
    }
    std::cout << &a << ", " << &a.name << ", " << &a.age << ", " << a.getName() << std::endl;
    char v[1000] =  "12345";
    std::cout << &v << "., " << v << std::endl;
    std::cout << a.getName() << std::endl;
    /*while(strcmp(a.getName(), "onte") == 0)
    {
        static int i = 0;
        std::cout << i++ << std::endl;
    }*/
    //std::cin.get();
    return 0;
}