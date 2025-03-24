#include "../kitapch.h"
#include "FileReader.h"
#include <filesystem>
#include <fstream>

namespace Kita {
    std::string FileReader::readFile(const std::string& filePath) {
        std::ifstream file(filePath,std::ios::binary);
        if (!file) {
            KITA_ENGINE_ERROR("Unable to read file {} returning empty string",filePath.c_str());
            return "";
        }

        const std::size_t fileSize = std::filesystem::file_size(filePath);

        std::vector<char> buffer(fileSize);
        file.read(buffer.data(), static_cast<std::streamsize>(fileSize));

        std::string content(buffer.begin(),buffer.end());
        return content;

    }
} // Kita