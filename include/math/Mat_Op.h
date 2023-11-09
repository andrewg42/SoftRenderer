#pragma once

#include "glm/gtc/matrix_transform.hpp"


glm::mat4 lookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up);

glm::mat4 perspective(float fovy, float aspect, float zNear, float zFar);

glm::mat4 proj_mat_ortho(float left, float right, float bottom, float top,
                         float zNear, float zFar);