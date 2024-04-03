#pragma once

#include <algorithm>

template<typename T>
class UniquePtr
{
private :
    T* _ptr = nullptr;

public :
    constexpr UniquePtr() noexcept = default;

    constexpr UniquePtr(T* ptr) noexcept
    {
        _ptr = ptr;
    }

    ~UniquePtr() noexcept
    {
        delete _ptr;
    }


    //Destroy Copy Constructor
    UniquePtr(const UniquePtr<T>&) = delete;

    //Copy
    constexpr UniquePtr<T>& operator=(const UniquePtr<T>&) noexcept = delete;

    //Move Constructor
    constexpr UniquePtr(UniquePtr<T>&& other) noexcept
    {
//        _ptr = other._ptr;
//        other._ptr = nullptr;
        //In one line
        std::swap(_ptr, other . _ptr);
    };

    //Move Assignment
    constexpr UniquePtr<T>& operator=(UniquePtr<T>&& other) noexcept
    {
        std::swap(_ptr, other . _ptr);
        return *this;
    };

    constexpr T& operator*() const noexcept
    {
        return *_ptr;
    }

    constexpr T* operator->() const noexcept
    {
        return _ptr;
    }
};

template<typename T>
UniquePtr<T> MakeUnique(T value) noexcept
{
    return UniquePtr<T>(new T(value));
}