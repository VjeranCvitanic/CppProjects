#include <cstdint>
#include <iostream>

class A
{
public:
    A();
    A(uint16_t val_)
        : val(val_)
    {}
    ~A()
    {
        std::cout << "Destructor!" << std::endl;
    };
    uint16_t get_val() const
    {
        return val;
    }
private:
    uint16_t val;
};

class B
{
public:
    B();
    B(uint16_t val_)
        : val(val_)
    {}
    ~B()
    {
        std::cout << "Destructor!" << std::endl;
    };
    uint16_t get_val() const
    {
        return val;
    }
private:
    uint16_t val;
};

template<typename T>
uint16_t get_value(const T& obj)
{
    return obj.get_val();
}

template<typename T>
void print(T msg)
{
    std::cout << "Msg: " << msg << std::endl;
}

int main()
{
    A a(3);
    B b(7);
    print(get_value(a));
    print(get_value(b));
    
    return 0;
}