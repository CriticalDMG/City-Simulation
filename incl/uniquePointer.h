#ifndef UNIQUE_POINTER_H
#define UNIQUE_POINTER_H
#include <iostream>

//custom unique pointer termplate class
template<typename T>
class uniquePointer
{
public:
    explicit uniquePointer(T* p);
    uniquePointer(uniquePointer&& oth) noexcept;
    uniquePointer& operator=(uniquePointer&& oth) noexcept;
    ~uniquePointer() noexcept;

    template <typename U>
    uniquePointer(uniquePointer<U>&& oth) noexcept 
    :ptr(oth.release()) {}

    template <typename U>
    uniquePointer& operator=(uniquePointer<U>&& oth) noexcept
    {
        reset(oth.release());
        return *this;
    }

    explicit operator bool() const noexcept
    {
        return ptr;
    }

    T* release() noexcept;
    void reset(T* pointer = nullptr) noexcept;
    
    T& operator*() const noexcept;
    T* operator->() const noexcept;

    T* get() const noexcept;
private:
    T* ptr;
};
  
template<typename T>
uniquePointer<T>::uniquePointer(T* p): ptr(p) {}

template<typename T>
uniquePointer<T>::uniquePointer(uniquePointer<T>&& oth) noexcept
: ptr(oth.ptr) { oth.ptr = nullptr; }

template<typename T>
uniquePointer<T>& uniquePointer<T>::operator=(uniquePointer&& oth) noexcept
{
    if(this != &oth)
    {
        delete ptr;
        
        ptr = oth.ptr;

        oth.ptr = nullptr;
    }

    return *this;
}

template<typename T>
uniquePointer<T>::~uniquePointer() noexcept
{
    delete ptr;
}

template<typename T>
T* uniquePointer<T>::release() noexcept
{   
    T* temp = ptr;
    ptr = nullptr;
    return temp;
}

template<typename T>
void uniquePointer<T>::reset(T* pointer) noexcept
{
    if(ptr != pointer)
    {
        delete ptr;
        ptr = pointer;
    }
}
template<typename T>
T& uniquePointer<T>::operator*() const noexcept
{
    return *ptr;
}

template<typename T>
T* uniquePointer<T>::operator->() const noexcept
{
    return ptr;
}

template<typename T>
T* uniquePointer<T>::get() const noexcept
{
    return ptr;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const uniquePointer<T>& ptr)
{
    os << *ptr;

    return os;
}
#endif //UNIQUE_POINTER_H