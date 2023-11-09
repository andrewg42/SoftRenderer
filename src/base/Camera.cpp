#include "base/Camera.h"

#include <iostream>
#include <glm/fwd.hpp>

#include "math/Mat_Op.h"

Camera::Camera():
    changed(true),
    eye({0.0f, 0.0f, 5.0f}),
    look_at({0.0f, 0.0f, 0.0f}),
    up_vec({0.0f, 1.0f, 0.0f}), 
    keep_up_axis({0.0f, 1.0f, 0.0f})
{}

Camera::~Camera() = default;

glm::mat4 Camera::get_proj_mat_persp(float fovy_, float aspect_, float zNear_,
                                     float zFar_)
{
    return perspective(fovy_, aspect_, zNear_, zFar_);
}

glm::mat4 Camera::get_lookAt_mat()
{
    lookAt_mat = lookAt(eye, look_at, up_vec);
    return lookAt_mat;
}