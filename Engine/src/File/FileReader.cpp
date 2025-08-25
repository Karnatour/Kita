#include "../kitapch.h"
#include "../File/FileReader.h"
#include <filesystem>

namespace Kita {
    std::string FileReader::readFile(const std::filesystem::path& path) {
        std::ifstream file(path, std::ios::binary);
        if (!file) {
            KITA_ENGINE_ERROR("Unable to read file {} returning empty string", path.string());
            return "";
        }

        const std::size_t fileSize = std::filesystem::file_size(path);

        std::vector<char> buffer(fileSize);
        file.read(buffer.data(), static_cast<std::streamsize>(fileSize));

        std::string content(buffer.begin(), buffer.end());
        return content;
    }
} // Kita
