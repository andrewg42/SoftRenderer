#include "math/Mat_Op.h"

glm::mat4 lookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up)
{
    // ref: glm::lookAtRH
    glm::vec3 const f(glm::normalize(center - eye));
    glm::vec3 const s(glm::normalize(glm::cross(f, up)));
    glm::vec3 const u(glm::cross(s, f));
    glm::mat4 Result(1);

    Result[0][0] = s.x;
    Result[1][0] = s.y;
    Result[2][0] = s.z;
    Result[0][1] = u.x;
    Result[1][1] = u.y;
    Result[2][1] = u.z;
    Result[0][2] = -f.x;
    Result[1][2] = -f.y;
    Result[2][2] = -f.z;
    Result[3][0] = -glm::dot(s, eye);
    Result[3][1] = -glm::dot(u, eye);
    Result[3][2] = dot(f, eye);

    return Result;
}


glm::mat4 perspective(float fovy, float aspect, float zNear, float zFar)
{
    // ref: glm::perspectiveRH_ZO
    float const tanHalfFovy = glm::tan(fovy / 2.0f);
    glm::mat4 Result(0.0f);

    Result[0][0] = 1.0f / (aspect * tanHalfFovy);
    Result[1][1] = 1.0f / tanHalfFovy;
    // reversed-z
    Result[2][2] = zNear / (zFar - zNear);
    Result[2][3] = -1.0f;
    Result[3][2] = (zFar * zNear) / (zFar - zNear);

    return Result;
}

glm::mat4 proj_mat_ortho(float left, float right, float bottom,
                                float top, float zNear, float zFar)
{
    // ref: glm::orthoRH_ZO
    glm::mat4 Result(1);

    Result[0][0] = 2.0f / (right - left);
    Result[1][1] = 2.0f / (top - bottom);
    Result[3][0] = -(right + left) / (right - left);
    Result[3][1] = -(top + bottom) / (top - bottom);
    // TODO: reversed-z
    Result[2][2] = -1.0f / (zFar - zNear);
    Result[3][2] = -zNear / (zFar - zNear);

    return Result;
}