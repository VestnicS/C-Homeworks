#include <iostream>
#include <cassert>

template <typename T>
class less_than_comparable
{
    friend bool operator>(const T& lhs, const T& rhs)
    {
        return rhs < lhs;
    }

    friend bool operator<=(const T& lhs, const T& rhs)
    {
        return !(lhs > rhs);
    }

    friend bool operator>=(const T& lhs, const T& rhs)
    {
        return !(lhs < rhs);
    }

    friend bool operator==(const T& lhs, const T& rhs)
    {
        return !(lhs < rhs && lhs > rhs);
    }

    friend bool operator!=(const T& lhs, const T& rhs)
    {
        return !(lhs == rhs);
    }
};

template <typename T>
class counter
{
public:

    counter() {++size_;}
    counter(const counter&) {++size_;}
    counter(counter&&) {++size_;}

    ~counter() {--size_;}

    static size_t size() {return size_;}

private:
    inline static size_t size_ = 0;
};

class Number : public less_than_comparable<Number>, public counter<Number>
{
public:
    Number(int value) : value_(value) {}

    int value() const { return value_; }

    bool operator<(const Number& other) const 
    {
        return value_ < other.value_;
    }

private:
    int value_;
};

int main() 
{
    Number one{1};
    Number two{2};
    Number three{3};
    Number four{4};

    assert(one >= one);
    assert(three <= four);
    assert(two == two);
    assert(three > two);
    assert(one < two);

    std::cout << "Count: " << counter<Number>::size() << std::endl;
    {
        Number temp(42);
        assert(counter<Number>::size() == 5);
    }
    assert(counter<Number>::size() == 4);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}