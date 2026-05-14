#include <iostream>
#include <vector>
#include <unordered_set>
#include <memory>
#include <algorithm>

class SetImpl {
public:
    virtual ~SetImpl() = default;
    virtual void add(int elem) = 0;
    virtual void remove(int elem) = 0;
    virtual bool contains(int elem) const = 0;
    virtual std::vector<int> getElements() const = 0;
    virtual size_t size() const = 0;
    virtual std::unique_ptr<SetImpl> clone() const = 0;
};

class SmallSetImpl : public SetImpl 
{
private:
    std::vector<int> data;
public:
    void add(int elem) override 
    {
        if (!contains(elem))
            data.push_back(elem);
    }

    void remove(int elem) override 
    {
        auto it = std::find(data.begin(), data.end(), elem);
        if (it != data.end())
            data.erase(it);
    }

    bool contains(int elem) const override 
    {
        return std::find(data.begin(), data.end(), elem) != data.end();
    }

    std::vector<int> getElements() const override 
    {
        return data;
    }

    size_t size() const override 
    {
        return data.size();
    }

    std::unique_ptr<SetImpl> clone() const override 
    {
        auto cloned = std::make_unique<SmallSetImpl>();
        cloned->data = data;
        return cloned;
    }
};

class LargeSetImpl : public SetImpl 
{
private:
    std::unordered_set<int> data;
public:
    void add(int elem) override 
    {
        data.insert(elem);
    }

    void remove(int elem) override 
    {
        data.erase(elem);
    }

    bool contains(int elem) const override 
    {
        return data.find(elem) != data.end();
    }

    std::vector<int> getElements() const override 
    {
        return std::vector<int>(data.begin(), data.end());
    }

    size_t size() const override 
    {
        return data.size();
    }

    std::unique_ptr<SetImpl> clone() const override 
    {
        auto cloned = std::make_unique<LargeSetImpl>();
        cloned->data = data;
        return cloned;
    }
};

class Set 
{
private:
    std::unique_ptr<SetImpl> impl;
    static constexpr size_t THRESHOLD = 10;

    void switchImplIfNeeded() 
    {
        size_t currentSize = impl->size();
        bool isSmall = (currentSize < THRESHOLD);
        if (isSmall && dynamic_cast<LargeSetImpl*>(impl.get())) 
        {
            auto newImpl = std::make_unique<SmallSetImpl>();
            for (int elem : impl->getElements())
            { 
                newImpl->add(elem);
            }
            impl = std::move(newImpl);
        }
        else if (!isSmall && dynamic_cast<SmallSetImpl*>(impl.get())) 
        {
            auto newImpl = std::make_unique<LargeSetImpl>();
            for (int elem : impl->getElements())
            {
                newImpl->add(elem);
            }
            impl = std::move(newImpl);
        }
    }

public:
    Set() : impl(std::make_unique<SmallSetImpl>()) {}

    Set(const Set& other) : impl(other.impl->clone()) {}

    Set& operator=(const Set& other) 
    {
        if (this != &other)
            impl = other.impl->clone();
        return *this;
    }

    void add(int elem) 
    {
        impl->add(elem);
        switchImplIfNeeded();
    }

    void remove(int elem) 
    {
        impl->remove(elem);
        switchImplIfNeeded();
    }

    bool contains(int elem) const 
    {
        return impl->contains(elem);
    }

    std::vector<int> getElements() const 
    {
        return impl->getElements();
    }

    Set unite(const Set& other) const 
    {
        Set result;
        for (int e : impl->getElements())
        {
            result.add(e);
        }
        for (int e : other.getElements())
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
        for (int e : smaller.getElements()) 
        {
            if (larger.contains(e))
                result.add(e);
        }
        return result;
    }

    void print() const 
    {
        std::cout << "{ ";
        for (int e : impl->getElements())
        {
            std::cout << e << " ";
        }
        std::cout << "}" << " (size=" << impl->size() << ") ";
        if (dynamic_cast<SmallSetImpl*>(impl.get()))
            std::cout << "[SmallSetImpl]" << std::endl;
        else
            std::cout << "[LargeSetImpl]" << std::endl;
    }
};

int main() 
{
    Set s1;
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

    Set s2;
    for (int i = 8; i <= 20; ++i)
    {
        s2.add(i);
    }

    std::cout << "\ns2: ";
    s2.print();

    Set unionSet = s1.unite(s2);
    std::cout << "Объединение s1 и s2: ";
    unionSet.print();

    Set intersectSet = s1.intersect(s2);
    std::cout << "Пересечение s1 и s2: ";
    intersectSet.print();
}