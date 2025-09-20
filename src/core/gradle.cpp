#include "gradle.hpp"
#include "core/localization.hpp" // Added this include
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

// Custom deleter for FILE* from popen
struct PipeCloser {
    void operator()(FILE* pipe) const {
        if (pipe) {
            pclose(pipe);
        }
    }
};

void RunGradleTask(const std::string& project_path, const std::string& task, std::function<void(const std::string&)> on_output) {
    if (project_path.empty() || project_path == "No Project Opened") {
        on_output("Error: No project is open.");
        return;
    }

#ifdef _WIN32
    std::string gradlew_cmd = "gradlew.bat";
#else
    std::string gradlew_cmd = "./gradlew";
#endif

    // Navigate to project dir, execute task, and redirect stderr to stdout
    std::string command = "cd " + project_path + " && " + gradlew_cmd + " " + task + " 2>&1";

    std::array<char, 128> buffer;
    std::unique_ptr<FILE, PipeCloser> pipe(popen(command.c_str(), "r"));
    if (!pipe) {
        on_output(modtoolbox::core::Localization::Get("common_popen_failed"));
        return;
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        on_output(buffer.data()); // Stream output in real-time
    }
}
