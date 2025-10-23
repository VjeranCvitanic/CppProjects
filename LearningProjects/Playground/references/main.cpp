#include <iostream>

class A
{
public:
    A() = default;
    A(int val_)
        : val(val_)
    {}
    ~A()
    {
        std::cout << "destructor";
    }
    int getVal() const
    {
        return val;
    }
private:
    int val;
};

template <typename T>
void fun(const T*& obj)
{
    std::cout << obj->getVal() << std::endl;
}

int main()
{
    const A a(6);
    const A* b = &a;
    fun(b);
    return 0;
}