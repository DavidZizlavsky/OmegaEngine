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
        unsigned long long vertexNormalCount = 0;
        unsigned long long faceCount = 0;
        while (std::getline(file, line)) {
            if (line[0] == 'v' && line[1] == ' ') {
                vertexCount++;
            }
            else if (line[0] == 'v' && line[1] == 'n' && line[2] == ' ') {
                vertexNormalCount++;
            } 
            else if (line[0] == 'f' && line[1] == ' ') {
                faceCount++;
            }
        }

        struct FaceData {
            size_t vertexIndex, vertexTextureIndex, vertexNormalIndex = 0;
        };

        struct Triangle {
            FaceData x, y, z;
        };

        std::vector<glm::vec3> extractedVertices = std::vector<glm::vec3>();
        extractedVertices.reserve(vertexCount);
        std::vector<glm::vec3> extractedVertexNormals = std::vector<glm::vec3>();
        extractedVertexNormals.reserve(vertexNormalCount);
        std::vector<Triangle> extractedTriangles = std::vector<Triangle>();

        std::vector<FaceData> faceBuffer = std::vector<FaceData>();
        faceBuffer.reserve(64);

        file.clear();
        file.seekg(0, std::ios_base::beg);
        while (std::getline(file, line)) {
            if (line[0] == 'v' && line[1] == ' ') {
                glm::vec3 vertexData = {};
                if (std::sscanf(line.c_str() + 2, "%f %f %f", &vertexData.x, &vertexData.y, &vertexData.z) != 3) {
                    throw std::runtime_error("Failed to parse vertex data!");
                }
                extractedVertices.push_back(vertexData);
            }
            else if (line[0] == 'v' && line[1] == 'n' && line[2] == ' ') {
                glm::vec3 vertexNormalData = {};
                if (std::sscanf(line.c_str() + 3, "%f %f %f", &vertexNormalData.x, &vertexNormalData.y, &vertexNormalData.z) != 3) {
                    throw std::runtime_error("Failed to parse vertex normal data!");
                }
                extractedVertexNormals.push_back(vertexNormalData);
            }
            else if (line[0] == 'v' && line[1] == 't' && line[2] == ' ') {
                glm::vec2 vertexTextureData = {};
                if (std::sscanf(line.c_str() + 3, "%f %f", &vertexTextureData.x, &vertexTextureData.y) != 2) {
                    throw std::runtime_error("Failed to parse vertex texture data!");
                }
                // TODO: add UV logic
            }
            else if (line[0] == 'f' && line[1] == ' ') {
                const char* charPointer = line.c_str() + 2;
                std::size_t positionOffset = 0;
                while (true) {
                    FaceData faceData = {};
                    int parsedVertexIndex = std::stoi(charPointer, &positionOffset, 10);
                    charPointer += positionOffset + 1;
                    int parsedVertexTextureIndex = std::stoi(charPointer, &positionOffset, 10);
                    charPointer += positionOffset + 1;
                    int parsedVertexNormalIndex = std::stoi(charPointer, &positionOffset, 10);
                    charPointer += positionOffset;
                    faceData.vertexIndex = static_cast<size_t>(parsedVertexIndex) - 1;
                    faceData.vertexTextureIndex = static_cast<size_t>(parsedVertexTextureIndex) - 1;
                    faceData.vertexNormalIndex = static_cast<size_t>(parsedVertexNormalIndex) - 1;
                    faceBuffer.push_back(faceData);
                    if (*charPointer != '\0') {
                        charPointer++;
                    }
                    else {
                        break;
                    }
                }
                for (int i = 2; i < faceBuffer.size(); i++) {
                    Triangle triangle = {};
                    triangle.x = faceBuffer[0];
                    triangle.y = faceBuffer[static_cast<size_t>(i) - 1];
                    triangle.z = faceBuffer[i];
                    extractedTriangles.push_back(triangle);
                }
                faceBuffer.clear();
            }
        }

        std::vector<Vertex> vertices = std::vector<Vertex>();
        std::vector<Index> indices = std::vector<Index>();

        vertices.reserve(extractedTriangles.size() * 3);
        indices.reserve(extractedTriangles.size() * 3);

        for (int i = 0; i < extractedTriangles.size(); i++) {
            Triangle triangle = extractedTriangles[i];
            glm::vec3 vertex = extractedVertices[triangle.x.vertexIndex];
            glm::vec3 vertexNormal = extractedVertexNormals[triangle.x.vertexNormalIndex];
            Vertex vert = {};
            vert.position = vertex;
            vert.normal = vertexNormal;
            Index index = vertices.size();
            vertices.push_back(vert);
            indices.push_back(index);

            triangle = extractedTriangles[i];
            vertex = extractedVertices[triangle.y.vertexIndex];
            vertexNormal = extractedVertexNormals[triangle.y.vertexNormalIndex];
            vert = {};
            vert.position = vertex;
            vert.normal = vertexNormal;
            index = vertices.size();
            vertices.push_back(vert);
            indices.push_back(index);

            triangle = extractedTriangles[i];
            vertex = extractedVertices[triangle.z.vertexIndex];
            vertexNormal = extractedVertexNormals[triangle.z.vertexNormalIndex];
            vert = {};
            vert.position = vertex;
            vert.normal = vertexNormal;
            index = vertices.size();
            vertices.push_back(vert);
            indices.push_back(index);
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