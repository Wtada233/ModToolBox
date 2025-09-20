#include "project.hpp"
#include <filesystem>

bool IsValidProject(const std::string& path) {
    if (path.empty() || !std::filesystem::is_directory(path)) {
        return false;
    }

    // A simple heuristic: a directory is a valid project if it contains
    // either 'gradlew' (for Unix-like systems) or 'gradlew.bat' (for Windows).
    bool gradlew_exists = std::filesystem::exists(std::filesystem::path(path) / "gradlew");
    bool gradlew_bat_exists = std::filesystem::exists(std::filesystem::path(path) / "gradlew.bat");

    return gradlew_exists || gradlew_bat_exists;
}
