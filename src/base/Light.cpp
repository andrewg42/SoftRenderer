#include "base/Light.h"

#include <iostream>

Light::Light() : pos({1.0f, 1.0f, 1.0f}), color({1.0f, 1.0f, 1.0f})
{
    //dir = -pos;
}

Light::~Light() = default;