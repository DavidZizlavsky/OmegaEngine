#include "OmegaEngine/File.h"
#include "OmegaEngine/Renderer/Renderer.h"
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace Omega {
    // Reads text file content and returns it as a string
	std::string File::LoadTextFile(const std::string& path)
	{
        std::ifstream file(path, std::ios::in | std::ios::binary);
        if (!file) {
            throw std::runtime_error("Failed to open file: " + path + "!");
        }

        std::ostringstream contents;
        contents << file.rdbuf();
        return contents.str();
	}

    // Parses .obj file into a renderer-ready struct
    MeshObject File::LoadMeshFromObj(const std::string& path)
    {
        std::ifstream file(path);
        if (!file) {
            throw std::runtime_error("Failed to open file: " + path + "!");
        }

        std::vector<Vertex> vertices = std::vector<Vertex>();
        std::vector<Index> indices = std::vector<Index>();

        std::string line;

        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#')
                continue;

            std::istringstream iss(line);
            std::string type;
            iss >> type;

            if (type == "v") {
                Vertex vertex;
                iss >> vertex.position.x >> vertex.position.y >> vertex.position.z;
                vertices.push_back(vertex);
            }
            else if (type == "f") {
                std::string vertexString;
                std::vector<Index> face = std::vector<Index>();
                
                while (iss >> vertexString) {
                    std::istringstream viss(vertexString);
                    std::string v, vt, vn;

                    std::getline(viss, v, '/');
                    std::getline(viss, vt, '/');
                    std::getline(viss, vn, '/');

                    face.push_back(std::stoi(v) - 1);
                }

                for (int i = 1; i < face.size(); i++) {
                    indices.push_back(face.at(0));
                    indices.push_back(face.at(i-1));
                    indices.push_back(face.at(i));
                }
            }
        } 

        MeshObject mo = {};
        mo.vertices = new Vertex[vertices.size()];
        std::memcpy(mo.vertices, vertices.data(), vertices.size() * sizeof(Vertex));
        mo.indices = new Index[indices.size()];
        std::memcpy(mo.indices, indices.data(), indices.size() * sizeof(Index));
        mo.verticesCount = vertices.size();
        mo.indicesCount = indices.size();
        return mo;
    }
}