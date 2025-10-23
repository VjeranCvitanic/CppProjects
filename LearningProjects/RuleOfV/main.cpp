#include <iostream>

class string
{
public:
    string() = default;
    string(const char* data_)
    {
        std::cout << "Create Constructor" << std::endl;
        int size = strlen(data_) + 1;
        data = new char[size];
        memcpy(data, data_, size);
    }
    ~string()
    {
        std::cout << "Destructor" << std::endl;
        delete[] data;
    }
    char* getName() const
    {
        return data;
    }
private:
    char* data;
};

void myFun(string a)
{
    std::cout << "Inside myFun" << std::endl;
}

int main()
{
    string s("Vjeran");
    std::cout << "String: " << s.getName() << std::endl;
    myFun(s);
    std::cout << "After myFun" << std::endl;
    std::cout << "String: " << s.getName() << std::endl;
    return 0;
}