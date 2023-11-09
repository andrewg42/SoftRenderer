#pragma once

#include <memory>
#include <string>
#include <vector>

#include "base/Camera.h"
#include "base/Light.h"
#include "asset_mgr/Model.h"
#include "utils/Singleton.h"

enum class Mesh_File_Type : unsigned char {
    ERRNO = 0,
    OBJ,
};

struct Scene: Singleton<Scene> {
    std::vector<std::shared_ptr<Model>> vec_models;
    Light m_lights; // only support 1 light now

    Scene();
    ~Scene();

    void set_light(Camera &);
    //void add_model(std::string const &);
};
