#include "config.hpp"
#include "vendor/json.hpp"
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>

using json = nlohmann::json;

bool ReadModJson(const std::string& path, std::string& mod_name, std::string& mod_id, std::string& version, std::string& description, std::string& authors) {
    std::ifstream file(path);
    if (!file.is_open()) {
        return false;
    }

    try {
        json data = json::parse(file);
        mod_name = data.value("name", "");
        mod_id = data.value("id", "");
        version = data.value("version", "");
        description = data.value("description", "");

        if (data.contains("authors") && data["authors"].is_array()) {
            std::string authors_str;
            for (const auto& author : data["authors"]) {
                if (author.is_string()) {
                    if (!authors_str.empty()) authors_str += ", ";
                    authors_str += author.get<std::string>();
                } else if (author.is_object() && author.contains("name")) {
                    if (!authors_str.empty()) authors_str += ", ";
                    authors_str += author["name"].get<std::string>();
                }
            }
            authors = authors_str;
        } else {
            authors = "";
        }

    } catch (json::parse_error& e) {
        return false;
    }

    return true;
}

bool WriteModJson(const std::string& path, const std::string& mod_name, const std::string& mod_id, const std::string& version, const std::string& description, const std::string& authors) {
    json data;
    std::ifstream input_file(path);
    if (input_file.is_open()) {
        try {
            data = json::parse(input_file);
        } catch (json::parse_error& e) {
            data = json::object();
        }
    }
    input_file.close();

    data["name"] = mod_name;
    data["id"] = mod_id;
    data["version"] = version;
    data["description"] = description;

    // Parse the comma-separated authors string and convert to a json array of objects
    json authors_array = json::array();
    std::stringstream ss(authors);
    std::string author_name;
    while (std::getline(ss, author_name, ',')) {
        // Trim leading/trailing whitespace
        size_t first = author_name.find_first_not_of(' ');
        if (std::string::npos == first) {
            continue;
        }
        size_t last = author_name.find_last_not_of(' ');
        author_name = author_name.substr(first, (last - first + 1));
        
        if (!author_name.empty()) {
            json author_obj;
            author_obj["name"] = author_name;
            authors_array.push_back(author_obj);
        }
    }
    data["authors"] = authors_array;


    std::ofstream output_file(path);
    if (!output_file.is_open()) {
        return false;
    }

    output_file << std::setw(4) << data << std::endl;

    return true;
}
