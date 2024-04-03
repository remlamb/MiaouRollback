#pragma once

#include <cstddef>

template<typename T>
class SharedPtr
{
private:
    T* _ptr = nullptr;
    std::size_t* _count = nullptr;

public:
    constexpr SharedPtr() noexcept = default;

    constexpr SharedPtr(T* ptr) noexcept
    {
        _ptr = ptr;
        _count = new std::size_t(1);
    }

    ~SharedPtr() noexcept
    {
        (*_count)--;
        if (*_count == 0)
        {
            delete _ptr;
            delete _count;
        }
    }

    //Copy Constructor
    constexpr SharedPtr(const SharedPtr& ptr) noexcept
    {
        _ptr = ptr . _ptr;
        _count = ptr . _count;
        (*_count)++;
    }

    //Copy
    constexpr SharedPtr<T>& operator=(const SharedPtr<T>& ptr) noexcept
    {
        _ptr = ptr . _ptr;
        _count = ptr . _count;
        (*_count)++;
    }

    //Move Constructor
    constexpr SharedPtr(const SharedPtr&& ptr) noexcept
    {
        _ptr = ptr . _ptr;
        _count = ptr . _count;

        ptr . _ptr = nullptr;
        ptr . _count = nullptr;
    }

    //Move Assignment
    constexpr SharedPtr<T>& operator=(SharedPtr<T>&& ptr) noexcept
    {
        _ptr = ptr . _ptr;
        _count = ptr . _count;

        ptr . _ptr = nullptr;
        ptr . _count = nullptr;
    }

    constexpr T* Get() const noexcept
    {
        return _ptr;
    }

    constexpr T& operator*() const noexcept
    {
        return *_ptr;
    }

    constexpr T* operator->() const noexcept
    {
        return Get();
    }

    [[nodiscard]] constexpr std::size_t Count() const noexcept
    {
        return *_count;
    }
};