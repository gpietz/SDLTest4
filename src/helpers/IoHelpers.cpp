#include <iostream>
#include <filesystem>

std::string getCurrentDirectory() {
    auto cwd = std::filesystem::current_path();
    return cwd.string();
}

std::string getFullResourcePath(const char* resource) {
    auto path = std::filesystem::path(resource);
    if (path.has_root_directory() || path.is_absolute())
        return path.string();

    auto current_dir = std::filesystem::path(getCurrentDirectory());
    auto full_path = current_dir / path;

    auto str_path = full_path.string();
    size_t position;
    while ((position = str_path.find("/")) != std::string::npos) {
        str_path.replace(position, 1, "\\");
    }

    return str_path;
}
