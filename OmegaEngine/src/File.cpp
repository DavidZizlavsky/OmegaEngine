#include "OmegaEngine/File.h"
#include "OmegaEngine/Renderer/Renderer.h"
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ios>
#include <cstdio>

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

        std::string line;
        unsigned long long vertexCount = 0;
        while (std::getline(file, line)) {
            if (line[0] == 'v' && line[1] == ' ') {
                vertexCount++;
            }
        }

        std::vector<Vertex> vertices = std::vector<Vertex>();
        std::vector<Index> indices = std::vector<Index>();

        vertices.reserve(vertexCount);
        indices.reserve(vertexCount * 6);

        std::vector<Index> face = std::vector<Index>();
        face.reserve(64);

        file.clear();
        file.seekg(0, std::ios_base::beg);
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#')
                continue;

            if (line[0] == 'v' && line[1] == ' ') {
                Vertex vertex{};

                if (std::sscanf(line.c_str() + 2, "%f %f %f", &vertex.position.x, &vertex.position.y, &vertex.position.z) != 3) {
                    throw std::runtime_error("Failed to parse .obj format!");
                }
                vertices.push_back(vertex);
            }
            else if (line[0] == 'f' && line[1] == ' ') {
                const char* stringPointer = line.c_str() + 2;
                while (*stringPointer) {
                    unsigned long index = std::strtoul(stringPointer, const_cast<char**>(&stringPointer), 10);
                    face.push_back(index - 1);

                    while (*stringPointer && *stringPointer != ' ') {
                        stringPointer++;
                    }
                    if (*stringPointer == ' ') {
                        stringPointer++;
                    }
                }

                for (int i = 1; i < face.size(); i++) {
                    indices.push_back(face[0]);
                    indices.push_back(face[i-1]);
                    indices.push_back(face[i]);
                }
                face.clear();
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