#ifndef LOCALIZATION_MANAGER_H
#define LOCALIZATION_MANAGER_H

#include <unordered_map>
#include <string>

class LocalizationManager {
public:
    static LocalizationManager& Instance();

    bool LoadFile(const std::string& filename);
    bool SetLanguage(const std::string& langCode);
    std::string Translate(const char* source) const;
    void Clear();
    void SetLangDirectory(const std::string& dir);

    const std::string& GetCurrentLanguage() const { return m_lang; }

private:
    LocalizationManager() = default;

    std::unordered_map<std::string, std::string> m_exact_dict;
    std::unordered_map<std::string, std::string> m_dict;
    std::string m_lang = "es";
    std::string m_langDir;
};

void SetExecutablePath(const char* argv0);

#endif