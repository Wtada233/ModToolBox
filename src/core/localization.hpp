#pragma once
#include <string>
#include <map>
#include <vector>

namespace modtoolbox::core {

class Localization {
public:
    // Initializes the localization system, detecting system locale and loading translations.
    static void Init();

    // Gets the translated string for a given key.
    static const std::string& Get(const std::string& key);

    // Sets the current language (e.g., "en", "zh"). Reloads translations.
    static void SetLanguage(const std::string& lang_code);

private:
    static std::map<std::string, std::string> translations_;
    static std::string current_lang_code_;

    // Loads translations from a JSON file.
    static bool LoadTranslations(const std::string& lang_code);
};

} // namespace modtoolbox::core