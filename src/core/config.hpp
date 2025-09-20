#pragma once
#include <string>

// Reads mod name and id from fabric.mod.json
bool ReadModJson(const std::string& path, std::string& mod_name, std::string& mod_id, std::string& version, std::string& description, std::string& authors);

// Writes mod name and id to fabric.mod.json
bool WriteModJson(const std::string& path, const std::string& mod_name, const std::string& mod_id, const std::string& version, const std::string& description, const std::string& authors);
