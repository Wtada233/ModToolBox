#pragma once
#include <string>

/**
 * @brief Checks if the given path points to a valid project directory.
 * 
 * @param path The path to check.
 * @return true if the path is a valid project, false otherwise.
 */
bool IsValidProject(const std::string& path);
