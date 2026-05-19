#include <iostream>
#include <vector>
#include <unordered_set>
#include <memory>
#include <algorithm>

template <typename T>
class SetImpl {
public:
    virtual ~SetImpl() = default;
    virtual void add(const T& elem) = 0;
    virtual void remove(const T& elem) = 0;
    virtual bool contains(const T& elem) const = 0;
    virtual std::vector<T> getElements() const = 0;
    virtual size_t size() const = 0;
    virtual std::unique_ptr<SetImpl<T>> clone() const = 0;
};

template <typename T>
class SmallSetImpl : public SetImpl<T> 
{
private:
    std::vector<T> data;
public:
    void add(const T& elem) override 
    {
        if (!contains(elem))
            data.push_back(elem);
    }

    void remove(const T& elem) override 
    {
        auto it = std::find(data.begin(), data.end(), elem);
        if (it != data.end())
            data.erase(it);
    }

    bool contains(const T& elem) const override 
    {
        return std::find(data.begin(), data.end(), elem) != data.end();
    }

    std::vector<T> getElements() const override 
    {
        return data;
    }

    size_t size() const override 
    {
        return data.size();
    }

    std::unique_ptr<SetImpl<T>> clone() const override 
    {
        auto cloned = std::make_unique<SmallSetImpl<T>>();
        cloned->data = data;
        return cloned;
    }
};

template <typename T>
class LargeSetImpl : public SetImpl<T> 
{
private:
    std::unordered_set<T> data;
public:
    void add(const T& elem) override 
    {
        data.insert(elem);
    }

    void remove(const T& elem) override 
    {
        data.erase(elem);
    }

    bool contains(const T& elem) const override 
    {
        return data.find(elem) != data.end();
    }

    std::vector<T> getElements() const override 
    {
        return std::vector<T>(data.begin(), data.end());
    }

    size_t size() const override 
    {
        return data.size();
    }

    std::unique_ptr<SetImpl<T>> clone() const override 
    {
        auto cloned = std::make_unique<LargeSetImpl<T>>();
        cloned->data = data;
        return cloned;
    }
};

template <typename T>
class Set 
{
private:
    std::unique_ptr<SetImpl<T>> impl;
    static constexpr size_t THRESHOLD = 10;

    void switchImplIfNeeded() 
    {
        size_t currentSize = impl->size();
        bool isSmall = (currentSize < THRESHOLD);
        if (isSmall && dynamic_cast<LargeSetImpl<T>*>(impl.get())) 
        {
            auto newImpl = std::make_unique<SmallSetImpl<T>>();
            for (const T& elem : impl->getElements())
            { 
                newImpl->add(elem);
            }
            impl = std::move(newImpl);
        }
        else if (!isSmall && dynamic_cast<SmallSetImpl<T>*>(impl.get())) 
        {
            auto newImpl = std::make_unique<LargeSetImpl<T>>();
            for (const T& elem : impl->getElements())
            {
                newImpl->add(elem);
            }
            impl = std::move(newImpl);
        }
    }

public:
    Set() : impl(std::make_unique<SmallSetImpl<T>>()) {}

    Set(const Set& other) : impl(other.impl->clone()) {}

    Set& operator=(const Set& other) 
    {
        if (this != &other)
            impl = other.impl->clone();
        return *this;
    }

    void add(const T& elem) 
    {
        impl->add(elem);
        switchImplIfNeeded();
    }

    void remove(const T& elem) 
    {
        impl->remove(elem);
        switchImplIfNeeded();
    }

    bool contains(const T& elem) const 
    {
        return impl->contains(elem);
    }

    std::vector<T> getElements() const 
    {
        return impl->getElements();
    }

    Set unite(const Set& other) const 
    {
        Set result;
        for (const T& e : impl->getElements())
        {
            result.add(e);
        }
        for (const T& e : other.getElements())
        {
            result.add(e);
        }
        return result;
    }

    Set intersect(const Set& other) const 
    {
        Set result;
        const Set& smaller = (impl->size() < other.impl->size()) ? *this : other;
        const Set& larger  = (impl->size() < other.impl->size()) ? other : *this;
        for (const T& e : smaller.getElements()) 
        {
            if (larger.contains(e))
                result.add(e);
        }
        return result;
    }

    void print() const 
    {
        std::cout << "{ ";
        for (const T& e : impl->getElements())
        {
            std::cout << e << " ";
        }
        std::cout << "}" << " (size=" << impl->size() << ") ";
        if (dynamic_cast<SmallSetImpl<T>*>(impl.get()))
            std::cout << "[SmallSetImpl]" << std::endl;
        else
            std::cout << "[LargeSetImpl]" << std::endl;
    }
};

int main() 
{
    Set<int> s1;
    std::cout << "Добавляем элементы от 1 до 15 в s1:" << std::endl;
    for (int i = 1; i <= 15; ++i) 
    {
        s1.add(i);
        s1.print();
    }

    std::cout << "\nУдаляем элемент 5, 10, 15:" << std::endl;
    s1.remove(5);
    s1.print();
    s1.remove(10);
    s1.print();
    s1.remove(15);
    s1.print();

    Set<int> s2;
    for (int i = 8; i <= 20; ++i)
    {
        s2.add(i);
    }

    std::cout << "\ns2: ";
    s2.print();

    Set<int> unionSet = s1.unite(s2);
    std::cout << "Объединение s1 и s2: ";
    unionSet.print();

    Set<int> intersectSet = s1.intersect(s2);
    std::cout << "Пересечение s1 и s2: ";
    intersectSet.print();
}