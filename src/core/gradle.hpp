#pragma once
#include <string>
#include <functional> // Add this include for std::function

// Executes a Gradle task in the given project path and streams the output
// to the provided callback.
void RunGradleTask(const std::string& project_path, const std::string& task, std::function<void(const std::string&)> on_output);
