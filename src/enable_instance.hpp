#ifndef ENABLE_INSTANCE_HPP_
#define ENABLE_INSTANCE_HPP_

#pragma once

namespace game
{

template <typename T>
class enable_instance
{
public:
    static
    T* &instance() { static T * _ptr = nullptr; return _ptr; }

    enable_instance() { instance() = static_cast<T*>(this); }
    virtual ~enable_instance() { if (instance() == static_cast<T*>(this)) instance() = nullptr; }
};

} // namespace game
#endif // ENABLE_INSTANCE_HPP_
