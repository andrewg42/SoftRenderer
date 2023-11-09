#include "asset_mgr/Model.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "OBJ_Loader.h"

void Model::load_obj(std::string const &path) {
    objl::Loader loader;

    if (!loader.LoadFile(path)) {
        std::cerr << "Failed to Load File. May have failed to find it or it was not anssssssssssssss .obj file.\n";
        return;
    }
    for(std::size_t i{}; i<loader.LoadedMeshes.size(); i++) {
        Mesh my_mesh;
        objl::Mesh obj_mesh = loader.LoadedMeshes[i];

        // name
        my_mesh.mesh_name = obj_mesh.MeshName;

        // material
#define PER(x) my_mesh.mesh_material.x = obj_mesh.MeshMaterial.x
        PER(name);
        PER(Ns);
        PER(Ni);
        PER(d);
        PER(illum);
        PER(map_Ka);
        PER(map_Kd);
        PER(map_Ks);
        PER(map_Ns);
        PER(map_d);
        PER(map_bump);
#undef PER
#define PER(x)                                                                 \
    my_mesh.mesh_material.x =                                                  \
        glm::vec3(obj_mesh.MeshMaterial.x.X, obj_mesh.MeshMaterial.x.Y,        \
                  obj_mesh.MeshMaterial.x.Z)
        PER(Ka);
        PER(Kd);
        PER(Ks);
#undef PER

        // vertices and indices
        for(std::size_t j{}; j<obj_mesh.Vertices.size(); j++) {
            my_mesh.pos.push_back(glm::vec3(obj_mesh.Vertices[j].Position.X,
                                            obj_mesh.Vertices[j].Position.Y,
                                            obj_mesh.Vertices[j].Position.Z));
            my_mesh.norm.push_back(glm::vec3(obj_mesh.Vertices[j].Normal.X,
                                             obj_mesh.Vertices[j].Normal.Y,
                                             obj_mesh.Vertices[j].Normal.Z));
            my_mesh.uv.push_back(glm::vec2(obj_mesh.Vertices[j].TextureCoordinate.X,
                                           obj_mesh.Vertices[j].TextureCoordinate.Y));

        }
        for (std::size_t j{}; j < obj_mesh.Indices.size(); j += 3) {
            my_mesh.face.push_back(glm::uvec3(obj_mesh.Indices[j],
                                             obj_mesh.Indices[j+1],
                                             obj_mesh.Indices[j+2]));
        }

        vec_meshes.push_back(my_mesh);
    }
}

// [TOOD]: write my own loader for .obj file
//static std::string firt_token(std::string const &str)
//{
//    if (!str.size())
//        return "";
//
//    std::size_t beg = str.find_first_not_of(" \t");
//    if (beg == std::string::npos)
//        return "";
//
//    std::size_t end = str.find_first_of(" \t", beg);
//    if (end == std::string::npos)
//        return str.substr(beg);
//    return str.substr(beg, end - beg);
//}
//
//void Model::load_obj(std::string const &path)
//{
//    if (path.substr(path.size() - 4, 4) != ".obj")
//    {
//        std::cerr << "Error file type: " << path << '\n';
//        return;
//    }
//
//    std::ifstream file(path);
//    if (!file.is_open())
//    {
//        std::cerr << "Failed to open file: " << path << '\n';
//        return;
//    }
//
//    bool has_name = false;
//    std::string line;
//    while (std::getline(file, line)) {
//        std::string mark = firt_token(line);
//        switch(mark.size()) {
//        case 1:
//            if (mark == "v") {
//                std::istringstream s(line.substr(2));
//                glm::vec3 v;
//                s >> v.x >> v.y >> v.z;
//                pos.push_back(v);
//            }
//            else if (mark == "f") {
//                std::istringstream s(line.substr(2));
//                std::string splitted;
//                std::vector<glm::uvec3> indices;
//
//                while (std::getline(s, splitted, ' ')) {
//                    glm::uvec3 idx(1);
//                    std::istringstream ss(splitted);
//                    std::string slashsplitted;
//                    std::size_t idxsubs = 0;
//                    while (std::getline(ss, slashsplitted, '/') &&
//                           idxsubs < 3) {
//                        std::istringstream(slashsplitted) >> idx[idxsubs++];
//                    }
//                    indices.push_back(idx - 1u);
//                }
//                for (std::size_t i = 2; i < indices.size(); i++) {
//                    glm::umat3x3 face =
//                        glm::umat3x3(indices[0], indices[i - 1], indices[i]);
//                    faces.push_back(face);
//                }
//            }
//            else if (mark == "o") {
//                if (has_name)
//                    continue;
//                has_name = true;
//                std::istringstream s(line.substr(2));
//                s >> mesh_name;
//            }
//            break;
//        case 2:
//            if (mark == "vn") {
//                std::istringstream s(line.substr(3));
//                glm::vec3 v;
//                s >> v.x >> v.y >> v.z;
//                norm.push_back(v);
//            }
//            else if (mark == "vt") {
//                std::istringstream s(line.substr(3));
//                glm::vec2 v;
//                s >> v.x >> v.y;
//                uv.push_back(v);
//            }
//        }
//        
//    }
//
//    file.close();
//}
