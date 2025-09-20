#include "localization.hpp"
#include "vendor/json.hpp" // Assuming nlohmann/json is available
#include <fstream>
#include <iostream>
#include <locale> // For std::locale
#include <codecvt> // For std::codecvt_utf8
#include <string_view> // For std::string_view

namespace modtoolbox::core {

std::map<std::string, std::string> Localization::translations_;
std::string Localization::current_lang_code_ = "en"; // Default to English

void Localization::Init() {
    // Detect system locale
    std::string system_locale_str;
    try {
        // Attempt to get system locale from environment variables
        // This is a common way on Unix-like systems
        const char* lang_env = std::getenv("LANG");
        if (lang_env) {
            system_locale_str = lang_env;
        } else {
            // Fallback for other systems or if LANG is not set
            system_locale_str = std::locale("").name();
        }
    } catch (const std::runtime_error& e) {
        std::cerr << "Warning: Could not detect system locale: " << e.what() << std::endl;
        system_locale_str = "en_US.UTF-8"; // Default to a common English locale
    }

    // Extract language code (e.g., "en" from "en_US.UTF-8")
    std::string detected_lang_code = "en"; // Default if detection fails
    if (system_locale_str.length() >= 2) {
        detected_lang_code = system_locale_str.substr(0, 2);
    }

    SetLanguage(detected_lang_code);
}

const std::string& Localization::Get(const std::string& key) {
    auto it = translations_.find(key);
    if (it != translations_.end()) {
        return it->second;
    }
    // Fallback: return the key itself if translation not found
    std::cerr << "Warning: Translation key '" << key << "' not found for language '" << current_lang_code_ << "'" << std::endl;
    return key;
}

void Localization::SetLanguage(const std::string& lang_code) {
    translations_.clear(); // Always clear before loading new translations

    if (LoadTranslations(lang_code)) {
        current_lang_code_ = lang_code;
        std::cout << "Language set to: " << lang_code << std::endl;
    } else {
        std::cerr << "Error: Could not load translations for language '" << lang_code << "'. Attempting to fall back to default (en)." << std::endl;
        // Only try to load English if it's not already the current language
        // or if it failed to load previously.
        if (lang_code != "en" && LoadTranslations("en")) {
            current_lang_code_ = "en";
            std::cout << "Language set to: en (fallback)." << std::endl;
        } else if (lang_code == "en") {
            std::cerr << "Error: Could not load default English translations. Localization will not function correctly." << std::endl;
        } else {
            std::cerr << "Error: Fallback to English failed. Localization will not function correctly." << std::endl;
        }
    }
}

bool Localization::LoadTranslations(const std::string& lang_code) {
    std::string file_path = "lang/" + lang_code + ".json";
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open translation file: " << file_path << std::endl;
        return false;
    }

    try {
        nlohmann::json data = nlohmann::json::parse(file);
        if (data.is_object()) {
            translations_.clear();
            for (auto& [key, value] : data.items()) {
                if (value.is_string()) {
                    translations_[key] = value.get<std::string>();
                }
            }
            return true;
        }
    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "Error parsing translation file '" << file_path << "': " << e.what() << std::endl;
    }
    return false;
}

} // namespace modtoolbox::core
