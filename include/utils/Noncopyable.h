#pragma once

struct Noncopyable {
    Noncopyable() = default;
    virtual ~Noncopyable() = default;

    Noncopyable(Noncopyable &) = delete;
    Noncopyable &operator=(Noncopyable &) = delete;
};