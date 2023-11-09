#include "base/Scene.h"

#include <iostream>

Scene::Scene() = default;

Scene::~Scene() = default;

void Scene::set_light(Camera &camera)
{
    m_lights.pos = camera.eye;
    //m_lights.dir = camera.look_at - camera.eye;
}

void add_model(std::string const &path) {}