#include "base/Input_Ctl.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "math/Mat_Op.h"
#include "viewer/ImGui_Context.h"

glm::mat4 Input_Ctl::get_proj() {
    return m_camera.get_proj_mat_persp(
        glm::radians(fovy),
        static_cast<float>(m_width) / m_height,
        zNear, zFar);
}

//void Input_Ctl::rotate(float x_cur, float y_cur, float x_last, float y_last)
//{
//// ref: https://github.com/nlguillemot/arcball_camera
//    float arcball_radius = static_cast<float>(std::max(scr_width, scr_height));
//
//    x_last -= static_cast<float>(scr_width) / 2;
//    x_cur -= static_cast<float>(scr_width) / 2;
//    y_last = static_cast<float>(scr_height) / 2 - y_last;
//    y_cur = static_cast<float>(scr_height) / 2 - y_cur;
//
//    float dist_cur = std::sqrtf(x_cur * x_cur + y_cur * y_cur);
//    float dist_last = std::sqrtf(x_last * x_last + y_last * y_last);
//
//    if(dist_last > arcball_radius) return;
//    float z_last =
//        std::sqrtf(arcball_radius * arcball_radius - x_last * x_last -
//                   y_last * y_last);
//    float z_cur = 0.0f;
//    if(dist_cur > arcball_radius) {
//        x_cur = (x_cur / dist_cur) * arcball_radius;
//        y_cur = (y_cur / dist_cur) * arcball_radius;
//        dist_cur = arcball_radius;
//    } else {
//        z_cur = std::sqrtf(arcball_radius * arcball_radius - x_cur * x_cur -
//                            y_cur * y_cur);
//    }
//
//    // rotate intersection points according to where the eye is
//    {
//        glm::vec3 const f(glm::normalize(m_camera.look_at - m_camera.eye));
//        glm::vec3 const s(glm::normalize(glm::cross(f, m_camera.up_vec)));
//        glm::vec3 const u(glm::cross(s, f));
//
//        glm::mat3 const eye_mat = {s, u, -f};
//
//        glm::vec3 p_last = eye_mat * glm::vec3(x_last, y_last, z_last);
//        glm::vec3 p_cur = eye_mat * glm::vec3(x_cur, y_cur, z_cur);
//        
//        x_last = p_last[0];
//        y_last = p_last[1];
//        z_last = p_last[2];
//
//        x_cur = p_cur[0];
//        y_cur = p_cur[1];
//        z_cur = p_cur[2];
//    }
//
//    // compute quaternion between the two vectors
//    glm::quat q;
//    {
//        float uv = std::sqrtf(x_cur * x_cur + y_cur * y_cur + z_cur * z_cur +
//            x_last * x_last + y_last * y_last + z_last * z_last);
//        q.w = uv + (x_cur * x_last + y_cur * y_last + z_cur * z_last);
//
//        if(q.w < 1.e-6f * uv) {
//            q.w = 0.0f;
//            if(std::fabs(x_last) > std::fabs(z_last)) {
//                q.x = -y_last;
//                q.y = x_last;
//                q.z = 0.0f;
//            } else {
//                q.x = 0.0f;
//                q.y = -z_last;
//                q.z = y_last;
//            }
//        } else {
//            q.x = y_last * z_cur - z_last * y_cur;
//            q.y = z_last * x_cur - x_last * z_cur;
//            q.z = x_last * y_cur - y_last * x_cur;
//        }
//
//        //float len = glm::length(q);
//        //q /= len;
//        q = glm::normalize(q);
//    }
//
//    float rotation_multiplier = 0.03f;
//    // amplify the quaternion's rotation by the multiplier
//    {
//        float c = q.w;
//        if(c > 0.9995f) {
//            q.x *= rotation_multiplier;
//            q.y = rotation_multiplier;
//            q.z = rotation_multiplier;
//            q.w = 1.0f + rotation_multiplier * (q.w - 1.0f);
//            q = glm::normalize(q);
//        } else {
//            // clamp to domain of acos
//            if (c < -1.0f) c = -1.0f;
//            else if (c > 1.0f) c = 1.0f;
//            // angle of the initial rotation
//            float theta_0 = acosf(c);
//            // apply multiplier to rotation
//            float theta = theta_0 * rotation_multiplier;
//
//            // compute the quaternion normalized difference
//            float qx = q.x;
//            float qy = q.y;
//            float qz = q.z;
//            float qw = q.w - c;
//            float len = sqrtf(qx * qx + qy * qy + qz * qz + qw * qw);
//            qx /= len;
//            qy /= len;
//            qz /= len;
//            qw /= len;
//
//            // do the slerp
//            q.x = qx * sinf(theta);
//            q.y = qy * sinf(theta);
//            q.z = qz * sinf(theta);
//            q.w = cosf(theta) + qw * sinf(theta);
//        }
//    }
//
//    glm::mat3 qmat = glm::transpose(glm::mat3_cast(q));
//    glm::mat3 qmat = glm::mat3_cast(q);
//    glm::vec3 to_eye_q = qmat * (m_camera.eye - m_camera.look_at);
//    glm::vec3 up_vec_q = qmat * m_camera.up_vec;
//
//    // update parameter of camera
//    m_camera.eye = m_camera.look_at + to_eye_q;
//    m_camera.up_vec = glm::normalize(up_vec_q);
//}

void Input_Ctl::rotate(float px_x1, float px_y1, float px_x0, float px_y0)
{
    float const rotation_multiplier = 3.0f;
    float x0 = (float)(px_x0 - scr_width / 2);
    float x1 = (float)(px_x1 - scr_width / 2);
    float y0 = (float)((scr_height - px_y0 - 1) - scr_height / 2);
    float y1 = (float)((scr_height - px_y1 - 1) - scr_height / 2);
    float arcball_radius =
        (float)(scr_width > scr_height ? scr_width : scr_height);

    glm::vec3 up = m_camera.up_vec;
    glm::vec3 eye = m_camera.eye;
    glm::vec3 target = m_camera.look_at;

    // distances to center of arcball
    float dist0 = sqrtf(x0 * x0 + y0 * y0);
    float dist1 = sqrtf(x1 * x1 + y1 * y1);

    float z0;
    if (dist0 > arcball_radius) {
        // initial click was not on the arcball, so just do nothing.
        return;
    }
    else {
        // compute depth of intersection using good old pythagoras
        z0 = sqrtf(arcball_radius * arcball_radius - x0 * x0 - y0 * y0);
    }

    float z1;
    if (dist1 > arcball_radius) {
        // started inside the ball but went outside, so clamp it.
        x1 = (x1 / dist1) * arcball_radius;
        y1 = (y1 / dist1) * arcball_radius;
        dist1 = arcball_radius;
        z1 = 0.0f;
    }
    else {
        // compute depth of intersection using good old pythagoras
        z1 = sqrtf(arcball_radius * arcball_radius - x1 * x1 - y1 * y1);
    }

    // rotate intersection points according to where the eye is
    {
        float to_eye_unorm[3] = {eye[0] - target[0], eye[1] - target[1],
                                 eye[2] - target[2]};
        float to_eye_len = sqrtf(to_eye_unorm[0] * to_eye_unorm[0] +
                                 to_eye_unorm[1] * to_eye_unorm[1] +
                                 to_eye_unorm[2] * to_eye_unorm[2]);
        float to_eye[3] = {to_eye_unorm[0] / to_eye_len,
                           to_eye_unorm[1] / to_eye_len,
                           to_eye_unorm[2] / to_eye_len};

        float across[3] = {-(to_eye[1] * up[2] - to_eye[2] * up[1]),
                           -(to_eye[2] * up[0] - to_eye[0] * up[2]),
                           -(to_eye[0] * up[1] - to_eye[1] * up[0])};

        // matrix that transforms standard coordinates to be relative to the
        // eye
        float eye_m[9] = {across[0], across[1], across[2], up[0],    up[1],
                          up[2],     to_eye[0], to_eye[1], to_eye[2]};

        float new_p0[3] = {
            eye_m[0] * x0 + eye_m[3] * y0 + eye_m[6] * z0,
            eye_m[1] * x0 + eye_m[4] * y0 + eye_m[7] * z0,
            eye_m[2] * x0 + eye_m[5] * y0 + eye_m[8] * z0,
        };

        float new_p1[3] = {
            eye_m[0] * x1 + eye_m[3] * y1 + eye_m[6] * z1,
            eye_m[1] * x1 + eye_m[4] * y1 + eye_m[7] * z1,
            eye_m[2] * x1 + eye_m[5] * y1 + eye_m[8] * z1,
        };

        x0 = new_p0[0];
        y0 = new_p0[1];
        z0 = new_p0[2];

        x1 = new_p1[0];
        y1 = new_p1[1];
        z1 = new_p1[2];
    }

    // compute quaternion between the two vectors
    // (http://lolengine.net/blog/2014/02/24/quaternion-from-two-vectors-final)
    float qw, qx, qy, qz;
    {
        float norm_u_norm_v = sqrtf((x0 * x0 + y0 * y0 + z0 * z0) *
                                    (x1 * x1 + y1 * y1 + z1 * z1));
        qw = norm_u_norm_v + (x0 * x1 + y0 * y1 + z0 * z1);

        if (qw < 1.e-6f * norm_u_norm_v) {
            /* If u and v are exactly opposite, rotate 180 degrees
             * around an arbitrary orthogonal axis. Axis normalisation
             * can happen later, when we normalise the quaternion. */
            qw = 0.0f;
            if (fabsf(x0) > fabsf(z0)) {
                qx = -y0;
                qy = x0;
                qz = 0.0f;
            }
            else {
                qx = 0.0f;
                qy = -z0;
                qz = y0;
            }
        }
        else {
            /* Otherwise, build quaternion the standard way. */
            qx = y0 * z1 - z0 * y1;
            qy = z0 * x1 - x0 * z1;
            qz = x0 * y1 - y0 * x1;
        }

        float q_len = sqrtf(qx * qx + qy * qy + qz * qz + qw * qw);
        qx /= q_len;
        qy /= q_len;
        qz /= q_len;
        qw /= q_len;
    }

    // amplify the quaternion's rotation by the multiplier
    // this is done by slerp(Quaternion.identity, q, multiplier)
    // math from
    // http://number-none.com/product/Understanding%20Slerp,%20Then%20Not%20Using%20It/
    {
        // cos(angle) of the quaternion
        float c = qw;
        if (c > 0.9995f) {
            // if the angle is small, linearly interpolate and normalize.
            qx = rotation_multiplier * qx;
            qy = rotation_multiplier * qy;
            qz = rotation_multiplier * qz;
            qw = 1.0f + rotation_multiplier * (qw - 1.0f);
            float q_len = sqrtf(qx * qx + qy * qy + qz * qz + qw * qw);
            qx /= q_len;
            qy /= q_len;
            qz /= q_len;
            qw /= q_len;
        }
        else {
            // clamp to domain of acos for robustness
            if (c < -1.0f)
                c = -1.0f;
            else if (c > 1.0f)
                c = 1.0f;
            // angle of the initial rotation
            float theta_0 = acosf(c);
            // apply multiplier to rotation
            float theta = theta_0 * rotation_multiplier;

            // compute the quaternion normalized difference
            float qx2 = qx;
            float qy2 = qy;
            float qz2 = qz;
            float qw2 = qw - c;
            float q2_len = sqrtf(qx2 * qx2 + qy2 * qy2 + qz2 * qz2 + qw2 * qw2);
            qx2 /= q2_len;
            qy2 /= q2_len;
            qz2 /= q2_len;
            qw2 /= q2_len;

            // do the slerp
            qx = qx2 * sinf(theta);
            qy = qy2 * sinf(theta);
            qz = qz2 * sinf(theta);
            qw = cosf(theta) + qw2 * sinf(theta);
        }
    }

    // vector from the target to the eye, which will be rotated according to
    // the arcball's arc.
    float to_eye[3] = {eye[0] - target[0], eye[1] - target[1],
                       eye[2] - target[2]};

    // convert quaternion to matrix (note: row major)
    float qmat[9] = {(1.0f - 2.0f * qy * qy - 2.0f * qz * qz),
                     2.0f * (qx * qy + qw * qz),
                     2.0f * (qx * qz - qw * qy),
                     2.0f * (qx * qy - qw * qz),
                     (1.0f - 2.0f * qx * qx - 2.0f * qz * qz),
                     2.0f * (qy * qz + qw * qx),
                     2.0f * (qx * qz + qw * qy),
                     2.0f * (qy * qz - qw * qx),
                     (1.0f - 2.0f * qx * qx - 2.0f * qy * qy)};

    // compute rotated vector
    float to_eye2[3] = {
        to_eye[0] * qmat[0] + to_eye[1] * qmat[1] + to_eye[2] * qmat[2],
        to_eye[0] * qmat[3] + to_eye[1] * qmat[4] + to_eye[2] * qmat[5],
        to_eye[0] * qmat[6] + to_eye[1] * qmat[7] + to_eye[2] * qmat[8]};

    // compute rotated up vector
    float up2[3] = {up[0] * qmat[0] + up[1] * qmat[1] + up[2] * qmat[2],
                    up[0] * qmat[3] + up[1] * qmat[4] + up[2] * qmat[5],
                    up[0] * qmat[6] + up[1] * qmat[7] + up[2] * qmat[8]};

    float up2_len = sqrtf(up2[0] * up2[0] + up2[1] * up2[1] + up2[2] * up2[2]);
    up2[0] /= up2_len;
    up2[1] /= up2_len;
    up2[2] /= up2_len;

    // update eye position
    m_camera.eye[0] = target[0] + to_eye2[0];
    m_camera.eye[1] = target[1] + to_eye2[1];
    m_camera.eye[2] = target[2] + to_eye2[2];

    // update up vector
    m_camera.up_vec[0] = up2[0];
    m_camera.up_vec[1] = up2[1];
    m_camera.up_vec[2] = up2[2];

}



void Input_Ctl::translate(float x_cur, float y_cur, float x_last, float y_last)
{
    float dx = (x_cur - x_last) / scr_width;
    float dy = -(y_cur - y_last) / scr_height;

    glm::vec3 const f(glm::normalize(m_camera.look_at - m_camera.eye));
    glm::vec3 const s(glm::normalize(glm::cross(f, m_camera.up_vec)));
    glm::vec3 const u(glm::cross(s, f));
    
    ImGuiIO &io = ImGui::GetIO();(void)io;
    float time_delta = 1.0f / io.Framerate;
    glm::vec3 dist_delta = time_delta * translate_speed * (dx * s + dy * u);

    m_camera.eye -= dist_delta;
    m_camera.look_at -= dist_delta;
}

void Input_Ctl::zoom(float y_offset) {
    fovy -= y_offset;
}