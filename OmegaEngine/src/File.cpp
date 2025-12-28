#include "OmegaEngine/File.h"
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>

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
}