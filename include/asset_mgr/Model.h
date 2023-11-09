#pragma once

#include <string>
#include <vector>
#include "glm/glm.hpp"

#include "asset_mgr/Mesh.h"

struct Model {
	std::string m_name;
	std::vector<Mesh> vec_meshes;

	void load_obj(std::string const &path);
};