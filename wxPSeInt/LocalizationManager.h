#ifndef LOCALIZATION_MANAGER_H
#define LOCALIZATION_MANAGER_H

#include <wx/string.h>
#include <unordered_map>
#include <string>

class LocalizationManager {
public:
    static LocalizationManager& Instance();

    bool LoadFile(const wxString& filename);
    bool SetLanguage(const char* langCode);
    wxString Translate(const char* source) const;
    void Clear();
    void SetLangDirectory(const wxString& dir);

    const std::string& GetCurrentLanguage() const { return m_lang; }

private:
    LocalizationManager() = default;

    std::unordered_map<std::string, std::string> m_exact_dict;
    std::unordered_map<std::string, std::string> m_dict;
    std::string m_lang = "es";
    wxString m_langDir;
};

#endif
