#pragma once

#include <new>
#include <utility>

template <class T> struct Singleton {
    Singleton() = default;
    virtual ~Singleton() = default;

    Singleton(Singleton const &) = delete;
    Singleton &operator=(Singleton const &) = delete;

    template <class... Args> static T &instance(Args &&...args)
    {
        static T instance(std::forward<Args>(args)...);
        return instance;
    }
};
