#include "../kitapch.h"
#include "../File/FileReader.h"

namespace Kita {
    std::optional<std::vector<char>> FileReader::readFileRaw(const std::filesystem::path& path) {
        std::ifstream file(path, std::ios::binary);
        if (!file) {
            KITA_ENGINE_ERROR("Unable to read file {}", path.string());
            return std::nullopt;
        }

        const std::size_t fileSize = std::filesystem::file_size(path);

        std::vector<char> buffer(fileSize);
        file.read(buffer.data(), static_cast<std::streamsize>(fileSize));

        return buffer;
    }

    std::optional<std::string> FileReader::readFileString(const std::filesystem::path& path) {
        std::optional<std::vector<char>> buffer = readFileRaw(path);
        if (!buffer) {
            return std::nullopt;
        }

        std::string content(buffer.value().begin(), buffer.value().end());
        return content;
    }

    std::optional<uint64_t> FileReader::getFileHash(const std::filesystem::path& path) {
        const std::optional<std::vector<char>> buffer = readFileRaw(path);
        if (!buffer) {
            return std::nullopt;
        }

        return XXH64(buffer->data(), buffer->size(), 0);
    }
} // Kita
