#pragma once
#include <iostream>
#include <stdexcept>

template <class T>
class Vector
{
public:
	Vector() : size(0), capacity(initial_capacity), arr(new T[initial_capacity]) {};

	Vector(size_t _size) : size(_size), capacity(_size * 2), arr(new T[_size * 2] = { T() }) {};

	Vector(size_t _size, T _elem) : size(_size), capacity(_size * 2), arr(new T[_size * 2]) {
		for (int i = 0; i < size; ++i)
		{
			arr[i] = _elem;
		}
	};

	Vector(const Vector& another) {
		size = another.size;
		capacity = another.capacity;
		arr = new T[capacity];
		for (int i = 0; i < size; ++i)
		{
			arr[i] = another.arr[i];
		}
	};

	Vector(Vector&& other)
	{
		capacity = other.capacity;
		size = other.size;
		arr = std::exchange(other.arr, nullptr);
	}

	Vector& operator=(const Vector& another) {
		if (this != &another)
		{
			size = another.size;
			capacity = another.capacity;
			arr = new T[capacity];
			for (int i = 0; i < size; ++i)
			{
				arr[i] = another.arr[i];
			}
		}
		return *this;
	};

	Vector& operator=(Vector&& other)
	{
		if (this != &other)
		{
			capacity = other.capacity;
			size = other.size;
			arr = std::exchange(other.arr, nullptr);
		}
		return *this;
	}

	~Vector()
	{
		delete[] arr;
	};

	T& operator[](int index)
	{
		if (index < 0 || index >= size)
			throw std::bad_alloc();
		return arr[index];
	};

	size_t _size()
	{
		return size;
	};

	size_t _capacity()
	{
		return capacity;
	};

	void pop_back()
	{
		if (size < 1)
			throw std::bad_alloc();
		arr[size - 1] = T();
		--size;
	};

	void push_back(const T& new_elem)
	{
		if(size >= capacity)
		{
			capacity *= 2;
			T* temp = copy();
			delete[] arr;
			arr = temp;
		}
		arr[size] = new_elem;
		++size;
	};

	void reserve(size_t memory)
	{
		if (capacity > memory)
			return;
		capacity = memory;
	};

	void resize(size_t new_size, T _elem = T())
	{
		if (new_size <= size)
			size = new_size;
		else
		{
			if (new_size >= capacity)
			{
				capacity = new_size * 2;
				T* temp = copy();
				delete[] arr;
				arr = temp;
			}
			for (int i = size; i < new_size; i++)
			{
				arr[i] = _elem;
			}
			size = new_size;
		}
		return;
	};

private:
	size_t size;
	size_t capacity;
	T* arr;

	T* copy()
	{
		T* temp = new T[capacity];
		for (int i = 0; i < size; i++)
		{
			temp[i] = arr[i];
		}
		return temp;
	}

	static const size_t initial_capacity = 2;
};